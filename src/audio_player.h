#pragma once

#include <string>

#ifdef __APPLE__
class AudioPlayer {
private:
    void* player;  // AVAudioPlayer* (hidden to avoid Objective-C in C++ header)
    std::string currentFile;
    bool isLooping;
    
public:
    AudioPlayer();
    ~AudioPlayer();
    
    bool loadFile(const std::string& filePath);
    void play();
    void stop();
    void pause();
    void setLooping(bool loop);
    bool isPlaying() const;
    void setVolume(float volume); // 0.0 to 1.0
};

#else
// Placeholder for other platforms - you can implement OpenAL or other libraries here
class AudioPlayer {
private:
    bool isLooping;
    
public:
    AudioPlayer() : isLooping(false) {}
    ~AudioPlayer() {}
    
    bool loadFile(const std::string& filePath) { return false; }
    void play() {}
    void stop() {}
    void pause() {}
    void setLooping(bool loop) { isLooping = loop; }
    bool isPlaying() const { return false; }
    void setVolume(float volume) {}
};
#endif

