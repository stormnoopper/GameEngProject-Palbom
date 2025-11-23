#ifdef __APPLE__
#import <AVFoundation/AVFoundation.h>
#import <Foundation/Foundation.h>
#include "audio_player.h"
#include <iostream>

AudioPlayer::AudioPlayer() : player(nullptr), isLooping(false) {
}

AudioPlayer::~AudioPlayer() {
    if (player) {
        AVAudioPlayer* avPlayer = reinterpret_cast<AVAudioPlayer*>(player);
        [avPlayer stop];
        [avPlayer release];
        player = nullptr;
    }
}

bool AudioPlayer::loadFile(const std::string& filePath) {
    // Stop and release previous player if exists
    if (player) {
        AVAudioPlayer* avPlayer = reinterpret_cast<AVAudioPlayer*>(player);
        [avPlayer stop];
        [avPlayer release];
        player = nullptr;
    }
    
    // Convert C++ string to NSString
    NSString* nsPath = [NSString stringWithUTF8String:filePath.c_str()];
    NSURL* url = [NSURL fileURLWithPath:nsPath];
    
    if (!url) {
        std::cout << "Failed to create URL from path: " << filePath << std::endl;
        return false;
    }
    
    NSError* error = nil;
    AVAudioPlayer* avPlayer = [[AVAudioPlayer alloc] initWithContentsOfURL:url error:&error];
    
    if (error) {
        std::cout << "Failed to load audio file: " << filePath << std::endl;
        std::cout << "Error: " << [[error localizedDescription] UTF8String] << std::endl;
        return false;
    }
    
    if (!avPlayer) {
        std::cout << "Failed to create AVAudioPlayer for: " << filePath << std::endl;
        return false;
    }
    
    // Set looping
    avPlayer.numberOfLoops = isLooping ? -1 : 0; // -1 means infinite loop
    
    // Prepare to play
    [avPlayer prepareToPlay];
    
    // Store as void*
    player = reinterpret_cast<void*>(avPlayer);
    currentFile = filePath;
    std::cout << "Audio file loaded successfully: " << filePath << std::endl;
    return true;
}

void AudioPlayer::play() {
    if (player) {
        AVAudioPlayer* avPlayer = reinterpret_cast<AVAudioPlayer*>(player);
        if (![avPlayer isPlaying]) {
            [avPlayer play];
        }
    }
}

void AudioPlayer::stop() {
    if (player) {
        AVAudioPlayer* avPlayer = reinterpret_cast<AVAudioPlayer*>(player);
        [avPlayer stop];
        [avPlayer setCurrentTime:0]; // Reset to beginning
    }
}

void AudioPlayer::pause() {
    if (player) {
        AVAudioPlayer* avPlayer = reinterpret_cast<AVAudioPlayer*>(player);
        if ([avPlayer isPlaying]) {
            [avPlayer pause];
        }
    }
}

void AudioPlayer::setLooping(bool loop) {
    isLooping = loop;
    if (player) {
        AVAudioPlayer* avPlayer = reinterpret_cast<AVAudioPlayer*>(player);
        avPlayer.numberOfLoops = loop ? -1 : 0;
    }
}

bool AudioPlayer::isPlaying() const {
    if (player) {
        AVAudioPlayer* avPlayer = reinterpret_cast<AVAudioPlayer*>(player);
        return [avPlayer isPlaying];
    }
    return false;
}

void AudioPlayer::setVolume(float volume) {
    if (player) {
        AVAudioPlayer* avPlayer = reinterpret_cast<AVAudioPlayer*>(player);
        avPlayer.volume = fmaxf(0.0f, fminf(1.0f, volume));
    }
}

#endif

