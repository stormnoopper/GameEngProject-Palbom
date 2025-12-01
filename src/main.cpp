#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <functional>

#include <learnopengl/animator.h>
#include <learnopengl/model_animation.h>
#include <learnopengl/model.h>

#include "audio_player.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>

// Text Rendering Structures
struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

// Character System Structs
struct CharacterPose
{
    int gridX = 1;  // Grid position (1-13 for playable area)
    int gridY = 1;  // Grid position (1-13 for playable area)
    glm::vec3 position;  // World position (calculated from grid)
    float rotation;
    enum class State { Idle, Walk } state = State::Idle;
    bool isMoving = false;
    float moveProgress = 0.0f;  // 0.0 to 1.0 for smooth movement
    int targetGridX = 1;
    int targetGridY = 1;
    
    // Health system
    int health = 3;  // 3 hearts
    float invulnerabilityTimer = 0.0f;  // Prevent multiple hits from one explosion
    
    // Bomb limit system
    int activeBombCount = 0;  // Track active bombs
    int maxBombCount = 3;     // Maximum bombs (increases with red bomb power-up)
    
    // Power-up system
    float bombRangeBoostTimer = 0.0f;  // Remaining time for +1 bomb range boost
    float shieldTimer = 0.0f;           // Remaining time for damage shield
    float speedBoostTimer = 0.0f;       // Remaining time for speed boost
};

struct CharacterTextures
{
    unsigned int diffuse  = 0;
    unsigned int specular = 0;
    unsigned int normal   = 0;
    unsigned int gloss    = 0;
};

// Bomb System Struct
struct Bomb
{
    int gridX;
    int gridY;
    float timer;  // Countdown timer in seconds
    int owner;    // 1 for P1, 2 for P2
    bool exploded = false;
    
    Bomb(int x, int y, int ownerId) : gridX(x), gridY(y), timer(3.0f), owner(ownerId) {}
};

// Power-Up Type Enum
enum class PowerUpType {
    RANGE_BOOST,      // Blue power - increases bomb range for 10 seconds
    BOMB_CAPACITY,    // Orange bomb - increases max bomb count permanently
    SHIELD,           // White shield - protects from damage for 5 seconds
    SPEED_BOOST,      // Green speed - increases movement speed for 10 seconds
    HEART             // Red heart - restores 1 health point (max 3)
};

// Power-Up System Struct
struct PowerUp
{
    int gridX;
    int gridY;
    glm::vec3 position;
    float rotation = 0.0f;  // For spinning animation
    PowerUpType type;       // Type of power-up
    
    PowerUp(int x, int y, PowerUpType t) : gridX(x), gridY(y), type(t) {
        const float MAP_OFFSET = -(15 - 1) * 1.0f / 2.0f;
        position = glm::vec3(
            MAP_OFFSET + x * 1.0f,
            0.3f,  // Slightly above ground
            MAP_OFFSET + y * 1.0f
        );
    }
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, std::vector<std::pair<int, int>>& breakableBlockPositions, 
                 std::mt19937& gen, const std::function<void(std::vector<std::pair<int, int>>&, std::mt19937&)>& generateBlocks,
                 CharacterPose& leftCharacter, CharacterPose& rightCharacter,
                 std::vector<Bomb>& bombs, bool gameOver, std::vector<AudioPlayer>& bombSounds, int& bombSoundIndex);
unsigned int loadCubemap(const std::vector<std::string>& faces);
bool HasBomb(int gridX, int gridY, const std::vector<Bomb>& bombs);
bool CanPlaceBomb(int gridX, int gridY, const std::vector<std::pair<int, int>>& breakableBlocks);
std::vector<std::pair<int, int>> GetExplosionTiles(const Bomb& bomb, const std::vector<std::pair<int, int>>& breakableBlocks, int explosionRange);
void ExplodeBomb(const Bomb& bomb, std::vector<std::pair<int, int>>& breakableBlocks, 
                 CharacterPose& leftPlayer, CharacterPose& rightPlayer, std::vector<PowerUp>& powerUps);
void UpdateBombs(std::vector<Bomb>& bombs, std::vector<std::pair<int, int>>& breakableBlocks, 
                 CharacterPose& leftPlayer, CharacterPose& rightPlayer, float deltaTime, std::vector<PowerUp>& powerUps);
void UpdatePowerUps(std::vector<PowerUp>& powerUps, CharacterPose& leftPlayer, CharacterPose& rightPlayer, float deltaTime, std::vector<AudioPlayer>& pickupSounds, int& pickupSoundIndex);
void RenderText(Shader& shader, const std::string& text, float x, float y, float scale, 
                glm::vec3 color, const std::map<char, Character>& Characters, unsigned int VAO, unsigned int VBO);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
bool isPointInRect(float px, float py, float rx, float ry, float rw, float rh);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
const int MAP_SIZE = 15;
const float TILE_SIZE = 1.0f;
const float BLOCK_HEIGHT = 0.2f;

// Settings UI globals
bool showSettingsPopup = false;
float currentVolume = 0.5f;
AudioPlayer* globalBackgroundMusic = nullptr;

// Game restart globals (pointers to game state for restart functionality)
CharacterPose* globalLeftPose = nullptr;
CharacterPose* globalRightPose = nullptr;
std::vector<std::pair<int, int>>* globalBreakableBlocks = nullptr;
std::vector<Bomb>* globalBombs = nullptr;
std::mt19937* globalGen = nullptr;
bool* globalGameOver = nullptr;
bool* globalGameStarted = nullptr;  // For intro screen
int* globalWinnerPlayer = nullptr;
bool requestGameRestart = false;

// Global pointers for sound pools
std::vector<AudioPlayer>* globalItemSounds = nullptr;
std::vector<AudioPlayer>* globalBombSounds = nullptr;



// Helper function to check if a position is a red block (unbreakable pattern)
bool isRedBlock(int x, int z) {
    // Red blocks are placed at even x and even z (2,4,6,8,10,12)
    return (x >= 2 && x <= 12 && x % 2 == 0 && 
            z >= 2 && z <= 12 && z % 2 == 0);
}

// Helper function to check if a position is a green cell (player spawn)
bool isGreenCell(int x, int z) {
    // Top-left 2x2 cluster
    if ((x == 1 || x == 2) && (z == 1 || z == 2))
        return true;
    // Bottom-right 2x2 cluster
    if ((x == 12 || x == 13) && (z == 12 || z == 13))
        return true;
    return false;
}

// Helper function to check if a position is a white cell (where breakable blocks can be placed)
bool isWhiteCell(int x, int z) {
    if (x == 0 || x == MAP_SIZE - 1 || z == 0 || z == MAP_SIZE - 1) return false;
    if (isRedBlock(x, z)) return false;
    if (isGreenCell(x, z)) return false;
    return true;
}

bool IsWalkable(int gridX, int gridY, const std::vector<std::pair<int, int>>& breakableBlocks, const CharacterPose& otherChar, const std::vector<Bomb>& bombs)
{
    if(gridX < 0 || gridX >= MAP_SIZE || gridY < 0 || gridY >= MAP_SIZE)
        return false;
    
    // Check walls (border)
    if (gridX == 0 || gridX == MAP_SIZE - 1 || gridY == 0 || gridY == MAP_SIZE - 1)
        return false;

    // Check red blocks
    if (isRedBlock(gridX, gridY))
        return false;

    // Check breakable blocks
    for (const auto& pos : breakableBlocks)
    {
        if (pos.first == gridX && pos.second == gridY)
            return false;
    }

    // Check bombs - players cannot walk through bombs
    for (const auto& bomb : bombs)
    {
        if (!bomb.exploded && bomb.gridX == gridX && bomb.gridY == gridY)
            return false;
    }

    // Check other character collision
    // Note: This is a simple check. Ideally, we should check if the other character is moving into this tile too.
    // But for turn-based-like movement, checking current and target position is safer.
    if ((otherChar.gridX == gridX && otherChar.gridY == gridY) || 
        (otherChar.isMoving && otherChar.targetGridX == gridX && otherChar.targetGridY == gridY))
    {
        return false;
    }

    return true;
}

glm::vec3 GridToWorld(int gridX, int gridY)
{
    const float MAP_OFFSET = -(MAP_SIZE - 1) * TILE_SIZE / 2.0f;
    return glm::vec3(
        MAP_OFFSET + gridX * TILE_SIZE,
        0.0f,
        MAP_OFFSET + gridY * TILE_SIZE
    );
}

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false)
{
    std::string filename = directory + "/" + path;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format = GL_RGB;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << filename << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

CharacterTextures LoadCharacterTextures(const std::string& directory)
{
    CharacterTextures textures;
    textures.diffuse  = TextureFromFile("Ch32_1001_Diffuse.png", directory);
    textures.specular = TextureFromFile("Ch32_1001_Specular.png", directory);
    textures.normal   = TextureFromFile("Ch32_1001_Normal.png", directory);
    textures.gloss    = TextureFromFile("Ch32_1001_Glossiness.png", directory);
    return textures;
}

void ApplyTexturesToModel(Model& model, const CharacterTextures& textures, const std::string& label)
{
    auto overrideTexture = [&](Texture& textureSlot)
    {
        if(textureSlot.type == "texture_diffuse" && textures.diffuse != 0)
        {
            textureSlot.id = textures.diffuse;
            textureSlot.path = label + "_diffuse";
        }
        else if(textureSlot.type == "texture_specular" && textures.specular != 0)
        {
            textureSlot.id = textures.specular;
            textureSlot.path = label + "_specular";
        }
        else if(textureSlot.type == "texture_normal" && textures.normal != 0)
        {
            textureSlot.id = textures.normal;
            textureSlot.path = label + "_normal";
        }
        else if(textureSlot.type == "texture_height" && textures.gloss != 0)
        {
            textureSlot.id = textures.gloss;
            textureSlot.path = label + "_gloss";
        }
    };

    for(auto& textureSlot : model.textures_loaded)
    {
        overrideTexture(textureSlot);
    }
    for(auto& mesh : model.meshes)
    {
        for(auto& textureSlot : mesh.textures)
        {
            overrideTexture(textureSlot);
        }
    }
}

void SetAnimation(CharacterPose& pose, Animator& animator, CharacterPose::State targetState,
                  Animation* idleAnimation, Animation* walkAnimation)
{
    if(pose.state == targetState)
    {
        return;
    }

    pose.state = targetState;
    switch(targetState)
    {
        case CharacterPose::State::Idle:
            animator.PlayAnimation(idleAnimation);
            break;
        case CharacterPose::State::Walk:
            animator.PlayAnimation(walkAnimation);
            break;
    }
}

bool TryMoveCharacter(CharacterPose& character, int dx, int dy, const std::vector<std::pair<int, int>>& breakableBlocks, const CharacterPose& otherChar, const std::vector<Bomb>& bombs)
{
    if(character.isMoving)
        return false;  // Already moving, can't start new movement

    int newX = character.gridX + dx;
    int newY = character.gridY + dy;

    if(IsWalkable(newX, newY, breakableBlocks, otherChar, bombs))
    {
        character.targetGridX = newX;
        character.targetGridY = newY;
        character.isMoving = true;
        character.moveProgress = 0.0f;
        
        // Set rotation based on direction (in radians)
        if(dx > 0) character.rotation = glm::radians(90.0f);  // Right - face right
        else if(dx < 0) character.rotation = glm::radians(-90.0f);  // Left - face left
        else if(dy > 0) character.rotation = 0.0f;  // Down - face down
        else if(dy < 0) character.rotation = glm::radians(180.0f);  // Up - face up
        
        return true;
    }
    return false;
}

void UpdateCharacterMovement(CharacterPose& character, float deltaTime)
{
    if(character.isMoving)
    {
        // Double movement speed when speed boost is active
        const float baseSpeed = 3.0f;  // Blocks per second
        const float moveSpeed = (character.speedBoostTimer > 0.0f) ? baseSpeed * 2.0f : baseSpeed;
        character.moveProgress += moveSpeed * deltaTime;

        if(character.moveProgress >= 1.0f)
        {
            // Movement complete
            character.moveProgress = 1.0f;
            character.gridX = character.targetGridX;
            character.gridY = character.targetGridY;
            character.isMoving = false;
        }

        // Interpolate position
        glm::vec3 startPos = GridToWorld(character.gridX, character.gridY);
        glm::vec3 endPos = GridToWorld(character.targetGridX, character.targetGridY);
        character.position = glm::mix(startPos, endPos, character.moveProgress);
        character.position.y = BLOCK_HEIGHT + 0.3f;  // Above block height + offset
    }
    else
    {
        // Snap to grid position
        character.position = GridToWorld(character.gridX, character.gridY);
        character.position.y = BLOCK_HEIGHT + 0.3f;  // Above block height + offset
    }
}

bool ProcessCharacterInput(GLFWwindow* window, CharacterPose& character, 
                           int forwardKey, int backwardKey, int leftKey, int rightKey,
                           const std::vector<std::pair<int, int>>& breakableBlocks, const CharacterPose& otherChar, const std::vector<Bomb>& bombs)
{
    const bool forwardPressed  = glfwGetKey(window, forwardKey)  == GLFW_PRESS;
    const bool backwardPressed = glfwGetKey(window, backwardKey) == GLFW_PRESS;
    const bool leftPressed     = glfwGetKey(window, leftKey)     == GLFW_PRESS;
    const bool rightPressed    = glfwGetKey(window, rightKey)    == GLFW_PRESS;

    bool moved = false;
    if(forwardPressed && !character.isMoving)
    {
        moved = TryMoveCharacter(character, 0, -1, breakableBlocks, otherChar, bombs);  // Up (decrease Y)
    }
    else if(backwardPressed && !character.isMoving)
    {
        moved = TryMoveCharacter(character, 0, 1, breakableBlocks, otherChar, bombs);  // Down (increase Y)
    }
    else if(leftPressed && !character.isMoving)
    {
        moved = TryMoveCharacter(character, -1, 0, breakableBlocks, otherChar, bombs);  // Left (decrease X)
    }
    else if(rightPressed && !character.isMoving)
    {
        moved = TryMoveCharacter(character, 1, 0, breakableBlocks, otherChar, bombs);  // Right (increase X)
    }

    return character.isMoving;
}

// Function to load texture
unsigned int loadTexture(const char* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

// Helper function to check if a point is inside a rectangle
bool isPointInRect(float px, float py, float rx, float ry, float rw, float rh)
{
    return px >= rx && px <= rx + rw && py >= ry && py <= ry + rh;
}

// Mouse button callback for UI interactions
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        
        // Convert to screen coordinates (GLFW Y is top-down, OpenGL Y is bottom-up)
        float screenX = (float)xpos;
        float screenY = (float)(SCR_HEIGHT - ypos);
        
        // Check if game is over - handle quit and restart buttons
        if (globalGameOver && *globalGameOver)
        {
            float gameOverWidth = 600.0f;
            float gameOverHeight = 400.0f;
            float gameOverY = (SCR_HEIGHT - gameOverHeight) / 2.0f;
            
            float btnWidth = 200.0f;
            float btnHeight = 60.0f;
            float btnSpacing = 40.0f;
            float btnY = gameOverY - btnHeight - 30.0f;
            
            // Quit button
            float quitBtnX = (SCR_WIDTH / 2.0f) - btnWidth - (btnSpacing / 2.0f);
            if (isPointInRect(screenX, screenY, quitBtnX, btnY, btnWidth, btnHeight))
            {
                std::cout << "Game Over - Quit button clicked" << std::endl;
                glfwSetWindowShouldClose(window, true);
                return;
            }
            
            // Restart button
            float restartBtnX = (SCR_WIDTH / 2.0f) + (btnSpacing / 2.0f);
            if (isPointInRect(screenX, screenY, restartBtnX, btnY, btnWidth, btnHeight))
            {
                std::cout << "Game Over - Restart button clicked" << std::endl;
                requestGameRestart = true;
                return;
            }
        }
        
        // Check if game hasn't started - handle start button click
        if (globalGameStarted && !(*globalGameStarted))
        {
            float introWidth = 600.0f;
            float introHeight = 300.0f;
            float introX = (SCR_WIDTH - introWidth) / 2.0f;
            float introY = SCR_HEIGHT / 2.0f - 50.0f;
            
            float startButtonWidth = 200.0f;
            float startButtonHeight = 80.0f;
            float startButtonX = (SCR_WIDTH - startButtonWidth) / 2.0f;
            float startButtonY = introY - startButtonHeight - 30.0f;
            
            if (isPointInRect(screenX, screenY, startButtonX, startButtonY, startButtonWidth, startButtonHeight))
            {
                *globalGameStarted = true;
                std::cout << "Start button clicked - game started!" << std::endl;
                return;
            }
        }
        
        // Settings button bounds (bottom-left, 60x60)
        float settingsBtnX = 20.0f;
        float settingsBtnY = 20.0f;
        float settingsBtnSize = 60.0f;
        
        if (!showSettingsPopup)
        {
            // Check if clicked on settings button
            if (isPointInRect(screenX, screenY, settingsBtnX, settingsBtnY, settingsBtnSize, settingsBtnSize))
            {
                showSettingsPopup = true;
                std::cout << "Settings popup opened" << std::endl;
                return;
            }
        }
        else
        {
            // Popup is showing - handle popup interactions
            float popupWidth = 600.0f;  // Updated to match new background size
            float popupHeight = 400.0f;  // Updated to match new background size
            float popupX = (SCR_WIDTH - popupWidth) / 2.0f;
            float popupY = (SCR_HEIGHT - popupHeight) / 2.0f;
            
            // Close button at top-right of popup
            float closeBtnSize = 60.0f;  // Size of close button
            float closeBtnX = popupX + popupWidth - closeBtnSize - 20.0f;
            float closeBtnY = popupY + popupHeight - closeBtnSize - 20.0f;
            
            if (isPointInRect(screenX, screenY, closeBtnX, closeBtnY, closeBtnSize, closeBtnSize))
            {
                showSettingsPopup = false;
                std::cout << "Settings popup closed" << std::endl;
                return;
            }
            
            // Mute checkbox - positioned on LEFT side to match rendering
            float checkboxSize = 60.0f;
            float checkboxLeftX = popupX + 100.0f;  // Match rendering position
            float checkboxLeftY = popupY + popupHeight / 2.0f + 20.0f;
            
            if (isPointInRect(screenX, screenY, checkboxLeftX, checkboxLeftY, checkboxSize, checkboxSize))
            {
                if (currentVolume > 0.0f) {
                    currentVolume = 0.0f;  // Mute
                    std::cout << "Muted" << std::endl;
                } else {
                    currentVolume = 0.5f;  // Unmute to 50%
                    std::cout << "Unmuted (50%)" << std::endl;
                }
                
                // Update background music
                if (globalBackgroundMusic) {
                    globalBackgroundMusic->setVolume(currentVolume);
                }
                
                // Update item sounds (30% volume when unmuted)
                if (globalItemSounds) {
                    float itemVolume = (currentVolume > 0.0f) ? 0.3f : 0.0f;
                    for (auto& sound : *globalItemSounds) {
                        sound.setVolume(itemVolume);
                    }
                }
                
                // Update bomb sounds (30% volume when unmuted)
                if (globalBombSounds) {
                    float bombVolume = (currentVolume > 0.0f) ? 0.3f : 0.0f;
                    for (auto& sound : *globalBombSounds) {
                        sound.setVolume(bombVolume);
                    }
                }
                return;
            }

            
            // Quit button (left of center) - updated size to 200x60 with spacing
            float quitBtnWidth = 200.0f;
            float quitBtnHeight = 60.0f;
            float buttonSpacing = 40.0f;
            float quitBtnX = popupX + popupWidth / 2.0f - quitBtnWidth - buttonSpacing / 2.0f;
            float quitBtnY = popupY + 50.0f;
            
            if (isPointInRect(screenX, screenY, quitBtnX, quitBtnY, quitBtnWidth, quitBtnHeight))
            {
                std::cout << "Quit button clicked - exiting game" << std::endl;
                glfwSetWindowShouldClose(window, true);
                return;
            }
            
            // Restart button (right of center) - updated size to 200x60 with spacing
            float restartBtnWidth = 200.0f;
            float restartBtnHeight = 60.0f;
            float restartBtnX = popupX + popupWidth / 2.0f + buttonSpacing / 2.0f;
            float restartBtnY = popupY + 50.0f;

            
            if (isPointInRect(screenX, screenY, restartBtnX, restartBtnY, restartBtnWidth, restartBtnHeight))
            {
                std::cout << "Restart button clicked - resetting game" << std::endl;
                requestGameRestart = true;
                showSettingsPopup = false;
                return;
            }
            
            // Click outside popup closes it
            if (!isPointInRect(screenX, screenY, popupX, popupY, popupWidth, popupHeight))
            {
                showSettingsPopup = false;
                std::cout << "Settings popup closed (clicked outside)" << std::endl;
            }
        }
    }
}

int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);  // Enable 4x MSAA for anti-aliasing

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Bomber-Style Top-Down Map", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);  // Enable MSAA

    // Initialize background music
    AudioPlayer backgroundMusic;
    std::string musicPath = FileSystem::getPath("assets/sound/background.mp3");
    if (backgroundMusic.loadFile(musicPath)) {
        backgroundMusic.setLooping(true);
        backgroundMusic.setVolume(0.5f); // 50% volume
        backgroundMusic.play();
        std::cout << "Background music started playing (looping)" << std::endl;
    } else {
        std::cout << "Warning: Could not load background music from: " << musicPath << std::endl;
        std::cout << "Trying alternative path..." << std::endl;
        // Try direct path as fallback
        if (backgroundMusic.loadFile("assets/sound/background.mp3")) {
            backgroundMusic.setLooping(true);
            backgroundMusic.setVolume(0.5f);
            backgroundMusic.play();
            std::cout << "Background music started playing (looping) from alternative path" << std::endl;
        }
    }
    
    // Set global pointer for volume control from settings UI
    globalBackgroundMusic = &backgroundMusic;
    
    // Initialize item pickup sound effect with multiple instances for stacking
    const int ITEM_SOUND_POOL_SIZE = 5;
    std::vector<AudioPlayer> itemPickupSounds(ITEM_SOUND_POOL_SIZE);
    int currentItemSoundIndex = 0;
    
    std::string itemSoundPath = FileSystem::getPath("assets/sound/GetItem.wav");
    for (int i = 0; i < ITEM_SOUND_POOL_SIZE; i++) {
        if (itemPickupSounds[i].loadFile(itemSoundPath)) {
            itemPickupSounds[i].setVolume(0.3f);  // 30% volume - lighter sound
        } else {
            std::cout << "Warning: Could not load item pickup sound " << i << " from: " << itemSoundPath << std::endl;
        }
    }
    
    // Initialize bomb placement sound effect with multiple instances for stacking
    const int BOMB_SOUND_POOL_SIZE = 5;
    std::vector<AudioPlayer> bombPlaceSounds(BOMB_SOUND_POOL_SIZE);
    int currentBombSoundIndex = 0;
    
    std::string bombSoundPath = FileSystem::getPath("assets/sound/BombEffect.MP3");
    for (int i = 0; i < BOMB_SOUND_POOL_SIZE; i++) {
        if (bombPlaceSounds[i].loadFile(bombSoundPath)) {
            bombPlaceSounds[i].setVolume(0.3f);  // 30% volume - lighter sound
        } else {
            std::cout << "Warning: Could not load bomb placement sound " << i << " from: " << bombSoundPath << std::endl;
        }
    }

    // build and compile shaders
    Shader shader("shaders/tile.vs", "shaders/tile.fs");
    Shader skyboxShader("shaders/skybox.vs", "shaders/skybox.fs");

    // set up vertex data for a 3D block (cube)
    float blockHeight = 0.2f; // Height of each block
    float blockSize = 1.0f;
    // positions          // normals           // texture coords
    float vertices[] = {
        // Top face (normal: 0, 1, 0)
         0.5f,  blockHeight,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
         0.5f,  blockHeight, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
        -0.5f,  blockHeight, -0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
        -0.5f,  blockHeight,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
        // Bottom face (normal: 0, -1, 0)
         0.5f,  0.0f, -0.5f,   0.0f, -1.0f, 0.0f,   1.0f, 1.0f,
         0.5f,  0.0f,  0.5f,   0.0f, -1.0f, 0.0f,   1.0f, 0.0f,
        -0.5f,  0.0f,  0.5f,   0.0f, -1.0f, 0.0f,   0.0f, 0.0f,
        -0.5f,  0.0f, -0.5f,   0.0f, -1.0f, 0.0f,   0.0f, 1.0f,
        // Front face (normal: 0, 0, 1)
         0.5f,  0.0f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  blockHeight,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  blockHeight,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f,  0.0f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
        // Back face (normal: 0, 0, -1)
         0.5f,  blockHeight, -0.5f,   0.0f, 0.0f, -1.0f,   1.0f, 1.0f,
         0.5f,  0.0f, -0.5f,   0.0f, 0.0f, -1.0f,   1.0f, 0.0f,
        -0.5f,  0.0f, -0.5f,   0.0f, 0.0f, -1.0f,   0.0f, 0.0f,
        -0.5f,  blockHeight, -0.5f,   0.0f, 0.0f, -1.0f,   0.0f, 1.0f,
        // Right face (normal: 1, 0, 0)
         0.5f,  0.0f, -0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
         0.5f,  blockHeight, -0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
         0.5f,  blockHeight,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
         0.5f,  0.0f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
        // Left face (normal: -1, 0, 0)
        -0.5f,  blockHeight, -0.5f,   -1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
        -0.5f,  0.0f, -0.5f,   -1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
        -0.5f,  0.0f,  0.5f,   -1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
        -0.5f,  blockHeight,  0.5f,   -1.0f, 0.0f, 0.0f,   1.0f, 1.0f
    };
    unsigned int indices[] = {
        // Top face
        0, 1, 2,  2, 3, 0,
        // Bottom face
        4, 5, 6,  6, 7, 4,
        // Front face
        8, 9, 10,  10, 11, 8,
        // Back face
        12, 13, 14,  14, 15, 12,
        // Right face
        16, 17, 18,  18, 19, 16,
        // Left face
        20, 21, 22,  22, 23, 20
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // load and create texture
    stbi_set_flip_vertically_on_load(true);
    std::string texturePath = FileSystem::getPath("assets/Floor/concrete_wall_07_basecolor_1k.png");
    unsigned int floorTexture = loadTexture(texturePath.c_str());
    if (floorTexture == 0)
    {
        std::cout << "Failed to load floor texture. Trying alternative path..." << std::endl;
        // Try direct path as fallback
        floorTexture = loadTexture("assets/Floor/concrete_wall_07_basecolor_1k.png");
    }

    std::string borderTexturePath = FileSystem::getPath("assets/Unbreakable_Block/tudor_wall_01_basecolor_1k.png");
    unsigned int borderTexture = loadTexture(borderTexturePath.c_str());
    if (borderTexture == 0)
    {
        std::cerr << "Warning: Failed to load borderTexture from " << borderTexturePath << ", trying relative path..." << std::endl;
        borderTexture = loadTexture("assets/Unbreakable_Block/tudor_wall_01_basecolor_1k.png");
    }
    
    // Load bomb range texture (lava rocks for dramatic effect)
    std::string bombRangeTexturePath = FileSystem::getPath("assets/BombRange/lava_rocks_01_color_1k.png");
    unsigned int bombRangeTexture = loadTexture(bombRangeTexturePath.c_str());
    if (bombRangeTexture == 0)
    {
        std::cerr << "Warning: Failed to load bombRangeTexture from " << bombRangeTexturePath << ", trying relative path..." << std::endl;
        bombRangeTexture = loadTexture("assets/BombRange/lava_rocks_01_color_1k.png");
    }
    std::cout << "Bomb range texture loaded successfully" << std::endl;


    std::string breakableTexturePath = FileSystem::getPath("assets/Breakable_Block/wood_05_baseColor_1k.png");
    unsigned int breakableTexture = loadTexture(breakableTexturePath.c_str());
    if (breakableTexture == 0)
    {
        std::cout << "Failed to load breakable texture. Trying alternative path..." << std::endl;
        breakableTexture = loadTexture("assets/Breakable_Block/wood_05_baseColor_1k.png");
    }

    shader.use();
    shader.setInt("texture1", 0);
    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    // Skybox setup
    float skyboxVertices[] = {
        // positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    std::vector<std::string> skyboxFaces = {
        FileSystem::getPath("assets/Background/px.png"),
        FileSystem::getPath("assets/Background/nx.png"),
        FileSystem::getPath("assets/Background/py.png"),
        FileSystem::getPath("assets/Background/ny.png"),
        FileSystem::getPath("assets/Background/pz.png"),
        FileSystem::getPath("assets/Background/nz.png")
    };
    stbi_set_flip_vertically_on_load(false);
    unsigned int cubemapTexture = loadCubemap(skyboxFaces);
    stbi_set_flip_vertically_on_load(true);

    // Map dimensions
    const int MAP_SIZE = 15;
    const float TILE_SIZE = 1.0f;
    const float MAP_OFFSET = -(MAP_SIZE - 1) * TILE_SIZE / 2.0f;

    // Helper function to check if a position is a red block (unbreakable pattern)
    auto isRedBlock = [](int x, int z) -> bool {
        // Red blocks are placed at even x and even z (2,4,6,8,10,12)
        // This matches the current rendering pattern
        return (x >= 2 && x <= 12 && x % 2 == 0 && 
                z >= 2 && z <= 12 && z % 2 == 0);
    };

    // Helper function to check if a position is a green cell (player spawn)
    // Green cells are 2x2 clusters at top-left and bottom-right
    // Top-left cluster: (1,1), (1,2), (2,1), (2,2)
    // Bottom-right cluster: (12,12), (12,13), (13,12), (13,13)
    auto isGreenCell = [](int x, int z) -> bool {
        // Top-left 2x2 cluster
        if ((x == 1 || x == 2) && (z == 1 || z == 2))
            return true;
        // Bottom-right 2x2 cluster
        if ((x == 12 || x == 13) && (z == 12 || z == 13))
            return true;
        return false;
    };

    // Helper function to check if a position is a white cell (where breakable blocks can be placed)
    auto isWhiteCell = [&](int x, int z) -> bool {
        // Not border (x=0, x=14, z=0, z=14)
        if (x == 0 || x == MAP_SIZE - 1 || z == 0 || z == MAP_SIZE - 1) 
            return false;
        // Not red block (pattern blocks)
        if (isRedBlock(x, z)) 
            return false;
        // Not green cell (player spawn areas) - IMPORTANT: No breakable blocks here!
        if (isGreenCell(x, z)) 
            return false;
        return true;
    };

    // Function to generate random breakable block positions
    auto generateBreakableBlocks = [&](std::vector<std::pair<int, int>>& positions, std::mt19937& generator) {
        positions.clear();
        std::uniform_real_distribution<float> dis(0.0f, 1.0f);
        const float BREAKABLE_BLOCK_PROBABILITY = 0.6f; // 60% chance
        
        for (int x = 1; x < MAP_SIZE - 1; x++)
        {
            for (int z = 1; z < MAP_SIZE - 1; z++)
            {
                // Only generate breakable blocks in white cells (not green, not red, not border)
                if (isWhiteCell(x, z) && dis(generator) < BREAKABLE_BLOCK_PROBABILITY)
                {
                    positions.push_back({x, z});
                }
            }
        }
    };

    // Generate random breakable block positions in white cells
    std::vector<std::pair<int, int>> breakableBlockPositions;
    std::random_device rd;
    std::mt19937 gen(rd());
    generateBreakableBlocks(breakableBlockPositions, gen);

    // ------------------------------------------------------------------
    // Bomb Setup
    // ------------------------------------------------------------------
    std::vector<Bomb> bombs;
    const std::string bombModelPath = FileSystem::getPath("assets/item/bomb.glb");
    Model bombModel(bombModelPath);
    std::cout << "Bomb model loaded from: " << bombModelPath << std::endl;

    // ------------------------------------------------------------------
    // Power-Up Setup
    // ------------------------------------------------------------------
    std::vector<PowerUp> powerUps;
    const std::string powerUpModelPath = FileSystem::getPath("assets/item/Power.glb");
    Model powerUpModel(powerUpModelPath);
    std::cout << "Power-up model loaded from: " << powerUpModelPath << std::endl;
    
    const std::string shieldModelPath = FileSystem::getPath("assets/item/Shield.glb");
    Model shieldModel(shieldModelPath);
    std::cout << "Shield model loaded from: " << shieldModelPath << std::endl;
    
    const std::string speedModelPath = FileSystem::getPath("assets/item/Speed.glb");
    Model speedModel(speedModelPath);
    std::cout << "Speed model loaded from: " << speedModelPath << std::endl;
    
    const std::string heartModelPath = FileSystem::getPath("assets/item/Heart.glb");
    Model heartModel(heartModelPath);
    std::cout << "Heart model loaded from: " << heartModelPath << std::endl;


    // ------------------------------------------------------------------
    // Character Setup
    // ------------------------------------------------------------------
    const std::string shaderVSPath = FileSystem::getPath("shaders/anim_model.vs");
    const std::string shaderFSPath = FileSystem::getPath("shaders/anim_model.fs");
    Shader characterShader(shaderVSPath.c_str(), shaderFSPath.c_str());

    const std::string idleAnimationPath = FileSystem::getPath("assets/Character/Movement/Idle.dae");
    const std::string walkAnimationPath = FileSystem::getPath("assets/Character/Movement/walk.dae");

    Model characterModelP1(idleAnimationPath);
    Model characterModelP2(idleAnimationPath);

    std::cout << "Model P1 Meshes: " << characterModelP1.meshes.size() << std::endl;

    const std::string p1TextureDir = FileSystem::getPath("assets/Character/P1");
    const std::string p2TextureDir = FileSystem::getPath("assets/Character/P2");

    std::cout << "P1 Texture Dir: " << p1TextureDir << std::endl;
    std::cout << "P2 Texture Dir: " << p2TextureDir << std::endl;

    const CharacterTextures p1Textures = LoadCharacterTextures(p1TextureDir);
    const CharacterTextures p2Textures = LoadCharacterTextures(p2TextureDir);
    ApplyTexturesToModel(characterModelP1, p1Textures, "P1");
    ApplyTexturesToModel(characterModelP2, p2Textures, "P2");

    std::cout << "Loading Animations..." << std::endl;
    std::cout << "Idle Path: " << idleAnimationPath << std::endl;
    Animation idleAnimation(idleAnimationPath, &characterModelP1);
    Animation walkAnimation(walkAnimationPath, &characterModelP1);
    std::cout << "Animations Loaded." << std::endl;
    std::cout << "Idle Duration: " << idleAnimation.GetDuration() << " Ticks: " << idleAnimation.GetTicksPerSecond() << std::endl;
    std::cout << "Walk Duration: " << walkAnimation.GetDuration() << " Ticks: " << walkAnimation.GetTicksPerSecond() << std::endl;

    Animator animatorP1(&idleAnimation);
    Animator animatorP2(&idleAnimation);

    // Initialize characters at spawn positions
    // P1 at (1,1) - Top-Left
    CharacterPose leftPose;
    leftPose.gridX = 1;
    leftPose.gridY = 1;
    leftPose.position = GridToWorld(leftPose.gridX, leftPose.gridY);
    leftPose.position.y = BLOCK_HEIGHT + 0.3f;
    leftPose.rotation = 0.0f;
    leftPose.targetGridX = 1;
    leftPose.targetGridY = 1;
    leftPose.activeBombCount = 0;

    // P2 at (13,13) - Bottom-Right
    CharacterPose rightPose;
    rightPose.gridX = 13;
    rightPose.gridY = 13;
    rightPose.position = GridToWorld(rightPose.gridX, rightPose.gridY);
    rightPose.position.y = BLOCK_HEIGHT + 0.3f;
    rightPose.rotation = glm::radians(180.0f);
    rightPose.targetGridX = 13;
    rightPose.targetGridY = 13;
    rightPose.activeBombCount = 0;

    // ------------------------------------------------------------------
    // UI Setup
    // ------------------------------------------------------------------
    const std::string uiVSPath = FileSystem::getPath("shaders/ui.vs");
    const std::string uiFSPath = FileSystem::getPath("shaders/ui.fs");
    Shader uiShader(uiVSPath.c_str(), uiFSPath.c_str());
    
    // Load UI textures
    stbi_set_flip_vertically_on_load(true);  // UI textures need to be flipped
    const std::string heartFullPath = FileSystem::getPath("assets/UI/heart_1.png");
    const std::string heartEmptyPath = FileSystem::getPath("assets/UI/heart_0.png");
    const std::string gameOverPath = FileSystem::getPath("assets/UI/gameover.png");
    const std::string introPath = FileSystem::getPath("assets/UI/Intro.png");
    const std::string startButtonPath = FileSystem::getPath("assets/UI/Start.png");
    const std::string settingsPath = FileSystem::getPath("assets/UI/setting.png");
    const std::string settingBackgroundPath = FileSystem::getPath("assets/UI/SettingBackground.png");
    const std::string closeButtonPath = FileSystem::getPath("assets/UI/CloseButton.png");
    const std::string barPath = FileSystem::getPath("assets/UI/Bar.png");
    const std::string emptyBarPath = FileSystem::getPath("assets/UI/EmptyBar.png");
    const std::string quitButtonPath = FileSystem::getPath("assets/UI/Quit.png");
    const std::string restartButtonPath = FileSystem::getPath("assets/UI/Restart.png");
    const std::string checkboxCheckedPath = FileSystem::getPath("assets/UI/Check.png");
    const std::string checkboxUncheckedPath = FileSystem::getPath("assets/UI/Uncheck.png");
    
    
    unsigned int heartFullTex = loadTexture(heartFullPath.c_str());
    unsigned int heartEmptyTex = loadTexture(heartEmptyPath.c_str());
    unsigned int gameOverTex = loadTexture(gameOverPath.c_str());
    unsigned int introTex = loadTexture(introPath.c_str());
    unsigned int startButtonTex = loadTexture(startButtonPath.c_str());
    unsigned int settingsTex = loadTexture(settingsPath.c_str());
    unsigned int settingBackgroundTex = loadTexture(settingBackgroundPath.c_str());
    unsigned int closeButtonTex = loadTexture(closeButtonPath.c_str());
    unsigned int barTex = loadTexture(barPath.c_str());
    unsigned int emptyBarTex = loadTexture(emptyBarPath.c_str());
    unsigned int quitButtonTex = loadTexture(quitButtonPath.c_str());
    unsigned int restartButtonTex = loadTexture(restartButtonPath.c_str());
    unsigned int checkboxCheckedTex = loadTexture(checkboxCheckedPath.c_str());
    unsigned int checkboxUncheckedTex = loadTexture(checkboxUncheckedPath.c_str());
    
    
    // Create UI quad VAO/VBO
    float quadVertices[] = {
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 
    
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindVertexArray(0);
    
    // Load player profile textures (swapped: P1 gets P2 image, P2 gets P1 image)
    stbi_set_flip_vertically_on_load(true);  // Ensure textures are flipped for OpenGL
    const std::string player1ProfilePath = FileSystem::getPath("assets/UI/Player2-Profile.png");  // Swapped
    const std::string player2ProfilePath = FileSystem::getPath("assets/UI/Player1-Profile.png");  // Swapped
    unsigned int player1ProfileTex = loadTexture(player1ProfilePath.c_str());
    unsigned int player2ProfileTex = loadTexture(player2ProfilePath.c_str());
    
    std::cout << "Player 1 Profile Texture ID: " << player1ProfileTex << std::endl;
    std::cout << "Player 2 Profile Texture ID: " << player2ProfileTex << std::endl;
    
    if (player1ProfileTex == 0 || player2ProfileTex == 0) {
        std::cout << "ERROR: Failed to load player profile textures!" << std::endl;
    }

    
    // ------------------------------------------------------------------
    // Text Rendering Setup with FreeType
    // ------------------------------------------------------------------
    const std::string textVSPath = FileSystem::getPath("shaders/text.vs");
    const std::string textFSPath = FileSystem::getPath("shaders/text.fs");
    Shader textShader(textVSPath.c_str(), textFSPath.c_str());
    
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return -1;
    }
    
    // Load font - try to use a system font
    FT_Face face;
    // Try common font paths for macOS
    const char* fontPaths[] = {
        "/System/Library/Fonts/Supplemental/Arial Bold.ttf",
        "/System/Library/Fonts/Supplemental/Arial.ttf",
        "/Library/Fonts/Arial.ttf"
    };
    
    bool fontLoaded = false;
    for (const char* fontPath : fontPaths)
    {
        if (FT_New_Face(ft, fontPath, 0, &face) == 0)
        {
            fontLoaded = true;
            std::cout << "Loaded font: " << fontPath << std::endl;
            break;
        }
    }
    
    if (!fontLoaded)
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return -1;
    }
    
    FT_Set_Pixel_Sizes(face, 0, 48);
    
    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    // Load first 128 characters of ASCII set
    std::map<char, Character> Characters;
    for (unsigned char c = 0; c < 128; c++)
    {
        // Load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        
        // Generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        // Store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // Destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    
    // Configure VAO/VBO for text rendering
    unsigned int textVAO, textVBO;
    glGenVertexArrays(1, &textVAO);
    glGenBuffers(1, &textVBO);
    glBindVertexArray(textVAO);
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    // Set text shader projection
    glm::mat4 textProjection = glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT);
    textShader.use();
    textShader.setMat4("projection", textProjection);

    
    // Game state
    bool gameOver = false;
    bool gameStarted = false;  // Game starts after clicking start button
    int winnerPlayer = 0;

    // Set global pointers for game restart functionality
    globalLeftPose = &leftPose;
    globalRightPose = &rightPose;
    globalBreakableBlocks = &breakableBlockPositions;
    globalBombs = &bombs;
    globalGen = &gen;
    globalGameOver = &gameOver;
    globalGameStarted = &gameStarted;
    globalWinnerPlayer = &winnerPlayer;
    globalItemSounds = &itemPickupSounds;
    globalBombSounds = &bombPlaceSounds;


    float deltaTime = 0.0f;
    float lastFrame = glfwGetTime(); // Initialize with current time to avoid large delta on first frame
    float skyboxRotation = 0.0f; // Skybox rotation angle in radians


    // render loop
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Cap deltaTime to avoid huge jumps (e.g. during debugging or lag)
        if (deltaTime > 0.1f) deltaTime = 0.1f;

        // Check for game restart request
        if (requestGameRestart)
        {
            std::cout << "Restarting game..." << std::endl;
            
            // Reset player 1 (left) position and health
            leftPose.gridX = 1;
            leftPose.gridY = 1;
            leftPose.targetGridX = 1;
            leftPose.targetGridY = 1;
            leftPose.position = GridToWorld(1, 1);
            leftPose.position.y = BLOCK_HEIGHT + 0.3f;
            leftPose.rotation = 0.0f;
            leftPose.health = 3;
            leftPose.isMoving = false;
            leftPose.moveProgress = 0.0f;
            leftPose.invulnerabilityTimer = 0.0f;
            
            // Reset player 2 (right) position and health
            rightPose.gridX = 13;
            rightPose.gridY = 13;
            rightPose.targetGridX = 13;
            rightPose.targetGridY = 13;
            rightPose.position = GridToWorld(13, 13);
            rightPose.position.y = BLOCK_HEIGHT + 0.3f;
            rightPose.rotation = glm::radians(180.0f);
            rightPose.health = 3;
            rightPose.isMoving = false;
            rightPose.moveProgress = 0.0f;
            rightPose.invulnerabilityTimer = 0.0f;
            
            // Clear all bombs
            bombs.clear();
            
            // Clear all power-ups
            powerUps.clear();
            
            // Regenerate breakable blocks
            generateBreakableBlocks(breakableBlockPositions, gen);
            
            // Reset game over state
            gameOver = false;
            winnerPlayer = 0;
            
            // Reset animations to idle
            SetAnimation(leftPose, animatorP1, CharacterPose::State::Idle, &idleAnimation, &walkAnimation);
            SetAnimation(rightPose, animatorP2, CharacterPose::State::Idle, &idleAnimation, &walkAnimation);
            
            requestGameRestart = false;
            std::cout << "Game restarted successfully!" << std::endl;
        }

        // Update skybox rotation (slow rotation: 3 degrees per second)
        skyboxRotation += glm::radians(3.0f) * deltaTime;
        if (skyboxRotation > glm::two_pi<float>()) {
            skyboxRotation -= glm::two_pi<float>();
        }

        // input (only if game has started)
        if (gameStarted) {
            processInput(window, breakableBlockPositions, gen, generateBreakableBlocks, leftPose, rightPose, bombs, gameOver, bombPlaceSounds, currentBombSoundIndex);
        }

        // Update bombs (only if game is not over and has started)
        if (!gameOver && gameStarted)
        {
            UpdateBombs(bombs, breakableBlockPositions, leftPose, rightPose, deltaTime, powerUps);
            UpdatePowerUps(powerUps, leftPose, rightPose, deltaTime, itemPickupSounds, currentItemSoundIndex);
        }

        // Update Character Animations
        UpdateCharacterMovement(leftPose, deltaTime);
        UpdateCharacterMovement(rightPose, deltaTime);

        if(leftPose.isMoving)
            SetAnimation(leftPose, animatorP1, CharacterPose::State::Walk, &idleAnimation, &walkAnimation);
        else
            SetAnimation(leftPose, animatorP1, CharacterPose::State::Idle, &idleAnimation, &walkAnimation);
        
        if(rightPose.isMoving)
            SetAnimation(rightPose, animatorP2, CharacterPose::State::Walk, &idleAnimation, &walkAnimation);
        else
            SetAnimation(rightPose, animatorP2, CharacterPose::State::Idle, &idleAnimation, &walkAnimation);

        animatorP1.UpdateAnimation(deltaTime);
        animatorP2.UpdateAnimation(deltaTime);
        
        // Check for game over
        if (!gameOver)
        {
            if (leftPose.health <= 0)
            {
                gameOver = true;
                winnerPlayer = 2;
                std::cout << "Game Over! Player 2 Wins!" << std::endl;
            }
            else if (rightPose.health <= 0)
            {
                gameOver = true;
                winnerPlayer = 1;
                std::cout << "Game Over! Player 1 Wins!" << std::endl;
            }
        }

        // render
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // activate shader
        shader.use();

        // create orthographic projection
        // Adjust size to fit the 15x15 map nicely in view
        float aspect = static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 200.0f);

        // position camera above board looking toward origin (tilted ~60°)
        glm::vec3 cameraPos(0.0f, MAP_SIZE * 1.2f, MAP_SIZE * 1.1f);
        glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);
        glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);

        // Lighting setup
        glm::vec3 lightPos(MAP_SIZE * 0.5f, MAP_SIZE * 1.5f, MAP_SIZE * 0.5f); // Light above the map
        glm::vec3 lightColor(1.0f, 1.0f, 0.95f); // Slightly warm white light

        // set uniforms
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        shader.setVec3("lightPos", lightPos);
        shader.setVec3("lightColor", lightColor);
        shader.setVec3("viewPos", cameraPos);

        // bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floorTexture);

        // render all tiles in the 15x15 grid
        glBindVertexArray(VAO);
        for (int x = 0; x < MAP_SIZE; x++)
        {
            for (int z = 0; z < MAP_SIZE; z++)
            {
                // calculate tile position
                float tileX = MAP_OFFSET + x * TILE_SIZE;
                float tileZ = MAP_OFFSET + z * TILE_SIZE;

                // create model matrix for this block
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(tileX, 0.0f, tileZ));
                shader.setMat4("model", model);

                // draw the block (36 indices for a cube: 6 faces * 2 triangles * 3 vertices)
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            }
        }

        // render raised border layer with unbreakable blocks
        glBindTexture(GL_TEXTURE_2D, borderTexture);
        const float fullBlockHeight = 1.0f;
        const float borderScaleY = fullBlockHeight / blockHeight;
        for (int x = 0; x < MAP_SIZE; x++)
        {
            for (int z = 0; z < MAP_SIZE; z++)
            {
                bool isBorder = (x == 0 || x == MAP_SIZE - 1 || z == 0 || z == MAP_SIZE - 1);
                if (!isBorder)
                    continue;

                float tileX = MAP_OFFSET + x * TILE_SIZE;
                float tileZ = MAP_OFFSET + z * TILE_SIZE;

                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(tileX, blockHeight, tileZ));
                model = glm::scale(model, glm::vec3(1.0f, borderScaleY, 1.0f));
                shader.setMat4("model", model);

                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            }
        }

        // render red blocks (shorter blocks with same texture as border)
        // Red blocks are 25% shorter than border blocks
        const float redBlockHeight = fullBlockHeight * 0.75f; // 75% of border height
        const float redBlockScaleY = redBlockHeight / blockHeight;
        // Pattern: red blocks at columns C, E, G, I, K, M (indices 2, 4, 6, 8, 10, 12)
        // and rows 3, 5, 7, 9, 11, 13 (indices 2, 4, 6, 8, 10, 12)
        for (int x = 2; x <= 12; x += 2)
        {
            for (int z = 2; z <= 12; z += 2)
            {
                float tileX = MAP_OFFSET + x * TILE_SIZE;
                float tileZ = MAP_OFFSET + z * TILE_SIZE;

                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(tileX, blockHeight, tileZ));
                model = glm::scale(model, glm::vec3(1.0f, redBlockScaleY, 1.0f));
                shader.setMat4("model", model);

                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            }
        }

        // render breakable blocks (randomly placed in white sections)
        // Breakable blocks are 75% of unbreakable block height (same as red blocks)
        const float breakableBlockHeight = fullBlockHeight * 0.75f; // 75% of border height
        const float breakableBlockScaleY = breakableBlockHeight / blockHeight;
        glBindTexture(GL_TEXTURE_2D, breakableTexture);
        for (const auto& pos : breakableBlockPositions)
        {
            int x = pos.first;
            int z = pos.second;
            
            // Double-check: Skip green cells (should not happen, but safety check)
            if (isGreenCell(x, z))
                continue;
            
            float tileX = MAP_OFFSET + x * TILE_SIZE;
            float tileZ = MAP_OFFSET + z * TILE_SIZE;
            
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(tileX, blockHeight, tileZ));
            model = glm::scale(model, glm::vec3(1.0f, breakableBlockScaleY, 1.0f));
            shader.setMat4("model", model);
            
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }

        // Render bomb explosion range indicators (semi-transparent overlays)
        if (!bombs.empty())
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            
            // Use a simple colored shader or reuse existing shader
            // We'll render using the current shader with a solid red color texture
            // Since we don't have a solid color texture loaded, we'll use the existing shader
            // and render slightly above ground to show the effect
            
            for (const auto& bomb : bombs)
            {
                if (!bomb.exploded)
                {
                    // Calculate explosion range for preview
                    int explosionRange = 2;  // Base range
                    if (bomb.owner == 1 && leftPose.bombRangeBoostTimer > 0.0f) {
                        explosionRange = 3;
                    } else if (bomb.owner == 2 && rightPose.bombRangeBoostTimer > 0.0f) {
                        explosionRange = 3;
                    }
                    
                    // Get all tiles that will be affected by this bomb
                    std::vector<std::pair<int, int>> explosionTiles = GetExplosionTiles(bomb, breakableBlockPositions, explosionRange);
                    
                    // Calculate pulsing alpha based on bomb timer
                    // More transparent as timer gets closer to 0 (more urgent)
                    float timerRatio = bomb.timer / 3.0f;  // 3.0f is the initial timer value
                    float pulseFrequency = 3.0f;  // Pulse faster as time runs out
                    float pulsePhase = glfwGetTime() * pulseFrequency * (1.0f + (1.0f - timerRatio));
                    float baseAlpha = 0.3f + 0.2f * (1.0f - timerRatio);  // Increase base alpha as timer decreases
                    float pulseAlpha = baseAlpha + 0.15f * sin(pulsePhase);
                    
                    // Render semi-transparent quads on each affected tile
                    for (const auto& tile : explosionTiles)
                    {
                        int x = tile.first;
                        int y = tile.second;
                        
                        float tileX = MAP_OFFSET + x * TILE_SIZE;
                        float tileZ = MAP_OFFSET + y * TILE_SIZE;
                        
                        // Render a flat quad slightly above the ground
                        glm::mat4 model = glm::mat4(1.0f);
                        model = glm::translate(model, glm::vec3(tileX, BLOCK_HEIGHT + 0.01f, tileZ));
                        // Scale to cover the tile and make it flat (very thin in Y direction)
                        model = glm::scale(model, glm::vec3(TILE_SIZE * 0.9f, 0.001f, TILE_SIZE * 0.9f));
                        shader.setMat4("model", model);
                        
                        // Use the lava rocks bomb range texture for dramatic effect
                        glBindTexture(GL_TEXTURE_2D, bombRangeTexture);
                        
                        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
                    }
                }
            }
            
            glDisable(GL_BLEND);
        }

        // glfw: swap buffers and poll IO events

        // Render characters
        characterShader.use();
        characterShader.setMat4("view", view);
        characterShader.setMat4("projection", projection);

        auto uploadBones = [&](Animator& animator)
        {
            const auto& transforms = animator.GetFinalBoneMatrices();
            for(int i = 0; i < transforms.size(); ++i)
            {
                characterShader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
            }
        };

        auto drawCharacter = [&](const CharacterPose& pose, Model& model)
        {
            glm::mat4 modelMatrix = glm::mat4(1.0f);
            modelMatrix = glm::translate(modelMatrix, pose.position);
            modelMatrix = glm::rotate(modelMatrix, pose.rotation, glm::vec3(0.0f, 1.0f, 0.0f));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
            characterShader.setMat4("model", modelMatrix);
            characterShader.setBool("isBomb", false);  // Not a bomb
            model.Draw(characterShader);
        };

        uploadBones(animatorP1);
        drawCharacter(leftPose, characterModelP1);

        uploadBones(animatorP2);
        drawCharacter(rightPose, characterModelP2);

        // Render bombs
        // Upload identity bone matrices for static bomb model (no animation)
        for(int i = 0; i < 100; ++i)
        {
            characterShader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", glm::mat4(1.0f));
        }
        
        // Set lighting uniforms for bombs
        characterShader.setVec3("lightPos", lightPos);
        characterShader.setVec3("lightColor", lightColor);
        characterShader.setVec3("viewPos", cameraPos);
        characterShader.setBool("isBomb", true);  // This is a bomb
        
        for (const auto& bomb : bombs)
        {
            if (!bomb.exploded)
            {
                glm::vec3 bombPos = GridToWorld(bomb.gridX, bomb.gridY);
                bombPos.y = BLOCK_HEIGHT + 0.15f;  // Slightly above ground
                
                glm::mat4 bombModelMatrix = glm::mat4(1.0f);
                bombModelMatrix = glm::translate(bombModelMatrix, bombPos);
                // Rotate 90 degrees around X axis to make bomb vertical (if it's lying down)
                bombModelMatrix = glm::rotate(bombModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                bombModelMatrix = glm::scale(bombModelMatrix, glm::vec3(0.3f));  // Scale bomb to appropriate size
                characterShader.setMat4("model", bombModelMatrix);
                bombModel.Draw(characterShader);
            }
        }
        
        
        // Render power-ups using different models based on type
        characterShader.use();
        
        // Upload identity bone matrices for static power-up models (no animation)
        for(int i = 0; i < 100; ++i)
        {
            characterShader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", glm::mat4(1.0f));
        }
        
        // Set stable blue color for power-ups
        characterShader.setVec3("lightPos", lightPos);
        characterShader.setVec3("lightColor", glm::vec3(2.0f, 2.0f, 2.0f));  // White light (no color variation)
        characterShader.setVec3("viewPos", cameraPos);
        characterShader.setBool("isBomb", false);
        
        for (const auto& powerUp : powerUps)
        {
            glm::vec3 powerUpPos = powerUp.position;
            powerUpPos.y = BLOCK_HEIGHT + 0.5f;  // Much higher for visibility
            
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, powerUpPos);
            
            // All power-ups rotate around Y axis (vertical spin)
            model = glm::rotate(model, powerUp.rotation, glm::vec3(0.0f, 1.0f, 0.0f));
            
            // Set color, scale, rotation, and model based on power-up type
            if (powerUp.type == PowerUpType::RANGE_BOOST) {
                // Blue Power.glb model - orient then spin
                // Apply orientation rotation BEFORE the Y-spin to fix center point
                glm::mat4 orientation = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                model = model * orientation;
                model = glm::scale(model, glm::vec3(8.0f)); 
                characterShader.setMat4("model", model);
                
                // Use BLUE LIGHT to force it to look blue
                characterShader.setVec3("lightColor", glm::vec3(0.2f, 0.5f, 2.0f));
                characterShader.setVec3("objectColor", glm::vec3(0.2f, 0.5f, 1.0f));
                
                powerUpModel.Draw(characterShader);
            } else if (powerUp.type == PowerUpType::BOMB_CAPACITY) {
                // Orange bomb.glb model - NEEDS TO BE SMALL
                // Apply orientation to make fuse point up
                glm::mat4 orientation = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                model = model * orientation;
                model = glm::scale(model, glm::vec3(0.3f)); 
                characterShader.setMat4("model", model);
                
                // Use ORANGE LIGHT for bright orange color
                characterShader.setVec3("lightColor", glm::vec3(3.0f, 0.6f, 0.0f));
                characterShader.setVec3("objectColor", glm::vec3(1.0f, 0.3f, 0.0f));
                
                bombModel.Draw(characterShader);
            } else if (powerUp.type == PowerUpType::SHIELD) {
                // White shield.glb model - orient then spin  
                // Apply same orientation as Power to spin from center
                glm::mat4 orientation = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                model = model * orientation;
                model = glm::scale(model, glm::vec3(5.0f));  // Larger size for visibility
                characterShader.setMat4("model", model);
                
                // Use VERY BRIGHT WHITE LIGHT to ensure pure white color
                characterShader.setVec3("lightColor", glm::vec3(5.0f, 5.0f, 5.0f));
                characterShader.setVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
                
                shieldModel.Draw(characterShader);
            } else if (powerUp.type == PowerUpType::SPEED_BOOST) {
                // Green speed.glb model
                // Rotate to make it stand upright with sole on ground
                model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Stand upright
                model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // Flip 180 to put sole down
                model = glm::scale(model, glm::vec3(8.0f));  // Bigger size for visibility
                characterShader.setMat4("model", model);
                
                // Use GREEN LIGHT for bright green speed boost
                characterShader.setVec3("lightColor", glm::vec3(0.2f, 4.0f, 0.2f));
                characterShader.setVec3("objectColor", glm::vec3(0.2f, 1.0f, 0.2f));
                
                speedModel.Draw(characterShader);
            } else if (powerUp.type == PowerUpType::HEART) {
                // Red heart.glb model
                // Just scale, no extra rotation needed for Y-spin
                model = glm::scale(model, glm::vec3(3.0f));  // Medium size
                characterShader.setMat4("model", model);
                
                // Use RED LIGHT for bright red heart
                characterShader.setVec3("lightColor", glm::vec3(4.0f, 0.0f, 0.0f));
                characterShader.setVec3("objectColor", glm::vec3(1.0f, 0.0f, 0.0f));
                
                heartModel.Draw(characterShader);
            }
        }
        
        // Reset light color
        characterShader.setVec3("lightColor", lightColor);




        // draw skybox last
        glDepthFunc(GL_LEQUAL);
        skyboxShader.use();
        glm::mat4 viewNoTranslation = glm::mat4(glm::mat3(view));
        // Rotate skybox around Y axis (vertical axis)
        viewNoTranslation = glm::rotate(viewNoTranslation, skyboxRotation, glm::vec3(0.0f, 1.0f, 0.0f));
        skyboxShader.setMat4("view", viewNoTranslation);
        skyboxShader.setMat4("projection", projection);
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
        
        // Render UI (hearts and game over)
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_DEPTH_TEST);  // UI always on top
        
        
        // ------------------------------------------------------------------
        // Render Player Profile UI
        // ------------------------------------------------------------------
        uiShader.use();
        glm::mat4 uiProjection = glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT);
        uiShader.setMat4("projection", uiProjection);
        uiShader.setInt("texture1", 0);
        glBindVertexArray(quadVAO);
        
        // Profile and heart dimensions
        float profileSize = 100.0f;
        float heartSize = 40.0f;
        float heartSpacing = 45.0f;
        float textPadding = 15.0f;  // Consistent padding between profile and text
        
        // ===== PLAYER 1 (Left side) =====
        float p1ProfileX = 20.0f;
        float p1ProfileY = SCR_HEIGHT - 20.0f - profileSize;
        
        // Player 1 Profile image
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(p1ProfileX, p1ProfileY, 0.0f));
        model = glm::scale(model, glm::vec3(profileSize, profileSize, 1.0f));
        uiShader.setMat4("model", model);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, player1ProfileTex);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        // Player 1 text and hearts start position (to the right of profile)
        float p1TextX = p1ProfileX + profileSize + textPadding;
        float p1HeartsY = p1ProfileY + 20.0f;
        
        // Player 1 hearts
        for (int i = 0; i < 3; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(p1TextX + i * heartSpacing, p1HeartsY, 0.0f));
            model = glm::scale(model, glm::vec3(heartSize, heartSize, 1.0f));
            uiShader.setMat4("model", model);
            
            glActiveTexture(GL_TEXTURE0);
            if (i < leftPose.health)
                glBindTexture(GL_TEXTURE_2D, heartFullTex);
            else
                glBindTexture(GL_TEXTURE_2D, heartEmptyTex);
            
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        
        // ===== PLAYER 2 (Right side - mirrored layout) =====
        float p2ProfileX = SCR_WIDTH - 20.0f - profileSize;
        float p2ProfileY = SCR_HEIGHT - 20.0f - profileSize;
        
        // Player 2 Profile image
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(p2ProfileX, p2ProfileY, 0.0f));
        model = glm::scale(model, glm::vec3(profileSize, profileSize, 1.0f));
        uiShader.setMat4("model", model);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, player2ProfileTex);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        // Player 2 text and hearts end position (to the left of profile, mirrored)
        // Hearts should end at the left edge of the profile with same padding
        float p2HeartsEndX = p2ProfileX - textPadding;  // Right edge of hearts area
        float p2TextX = p2HeartsEndX - (3 * heartSpacing - heartSpacing + heartSize);  // Start of first heart
        float p2HeartsY = p2ProfileY + 20.0f;
        
        // Player 2 hearts
        for (int i = 0; i < 3; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(p2TextX + i * heartSpacing, p2HeartsY, 0.0f));
            model = glm::scale(model, glm::vec3(heartSize, heartSize, 1.0f));
            uiShader.setMat4("model", model);
            
            glActiveTexture(GL_TEXTURE0);
            if (i < rightPose.health)
                glBindTexture(GL_TEXTURE_2D, heartFullTex);
            else
                glBindTexture(GL_TEXTURE_2D, heartEmptyTex);
            
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        
        // Unbind VAO before text rendering
        glBindVertexArray(0);
        
        // ===== TEXT RENDERING =====
        // Render text after all texture-based UI to avoid state conflicts
        // P1: To the right of profile, above hearts
        float p1TextY = p1HeartsY + heartSize + 5.0f;  // Above hearts
        float p1NameX = p1ProfileX + profileSize + 20.0f;  // To the right of profile
        RenderText(textShader, "PLAYER 1", p1NameX, p1TextY, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f), Characters, textVAO, textVBO);
        
        // P2: To the right of profile, above hearts (both players go right)
        float p2TextY = p2HeartsY + heartSize + 5.0f;  // Above hearts
        float p2TextWidth = 90.0f;  // Approximate width of "PLAYER 2" text
        float p2NameX = p2ProfileX - p2TextWidth - 50.0f;  // To the left of profile
        RenderText(textShader, "PLAYER 2", p2NameX, p2TextY, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f), Characters, textVAO, textVBO);
        
        
        // ===== BOMB COUNTER DISPLAY =====
        // Display remaining bombs below each player's profile
        int p1RemainingBombs = leftPose.maxBombCount - leftPose.activeBombCount;
        int p2RemainingBombs = rightPose.maxBombCount - rightPose.activeBombCount;
        
        std::string p1BombText = "X" + std::to_string(p1RemainingBombs) + "/" + std::to_string(leftPose.maxBombCount);
        std::string p2BombText = "X" + std::to_string(p2RemainingBombs) + "/" + std::to_string(rightPose.maxBombCount);
        
        // P1: Left-aligned bomb counter
        float p1BombCounterY = p1ProfileY - 35.0f;  // Below profile
        RenderText(textShader, p1BombText, p1ProfileX, p1BombCounterY, 0.7f, glm::vec3(1.0f, 0.8f, 0.2f), Characters, textVAO, textVBO);
        
        // P2: Left-aligned bomb counter (same as P1, starts from left edge of profile)
        float p2BombCounterY = p2ProfileY - 35.0f;
        RenderText(textShader, p2BombText, p2ProfileX, p2BombCounterY, 0.7f, glm::vec3(1.0f, 0.8f, 0.2f), Characters, textVAO, textVBO);
        
        // ===== POWER-UP INDICATOR =====
        // Display "POWER" text below bomb counter when power-up is active
        if (leftPose.bombRangeBoostTimer > 0.0f)
        {
            float p1PowerY = p1BombCounterY - 30.0f;  // Below bomb counter
            RenderText(textShader, "POWER", p1ProfileX, p1PowerY, 0.5f, glm::vec3(0.2f, 1.0f, 0.3f), Characters, textVAO, textVBO);
        }
        
        if (rightPose.bombRangeBoostTimer > 0.0f)
        {
            float p2PowerY = p2BombCounterY - 30.0f;  // Below bomb counter
            RenderText(textShader, "POWER", p2ProfileX, p2PowerY, 0.5f, glm::vec3(0.2f, 1.0f, 0.3f), Characters, textVAO, textVBO);
        }
        
        // ===== SHIELD INDICATOR =====
        // Display "SHIELD" text below POWER when shield is active
        if (leftPose.shieldTimer > 0.0f)
        {
            float p1ShieldY = p1BombCounterY - 60.0f;  // Below POWER text
            RenderText(textShader, "SHIELD", p1ProfileX, p1ShieldY, 0.5f, glm::vec3(0.5f, 1.0f, 1.0f), Characters, textVAO, textVBO);
        }
        
        if (rightPose.shieldTimer > 0.0f)
        {
            float p2ShieldY = p2BombCounterY - 60.0f;  // Below POWER text
            RenderText(textShader, "SHIELD", p2ProfileX, p2ShieldY, 0.5f, glm::vec3(0.5f, 1.0f, 1.0f), Characters, textVAO, textVBO);
        }
        
        // ===== SPEED INDICATOR =====
        // Display "SPEED" text below SHIELD when speed boost is active
        if (leftPose.speedBoostTimer > 0.0f)
        {
            float p1SpeedY = p1BombCounterY - 90.0f;  // Below SHIELD text
            RenderText(textShader, "SPEED", p1ProfileX, p1SpeedY, 0.5f, glm::vec3(0.2f, 1.0f, 0.3f), Characters, textVAO, textVBO);
        }
        
        if (rightPose.speedBoostTimer > 0.0f)
        {
            float p2SpeedY = p2BombCounterY - 90.0f;  // Below SHIELD text
            RenderText(textShader, "SPEED", p2ProfileX, p2SpeedY, 0.5f, glm::vec3(0.2f, 1.0f, 0.3f), Characters, textVAO, textVBO);
        }

        
        // Render intro screen if game hasn't started
        if (!gameStarted)
        {
            uiShader.use();
            glBindVertexArray(quadVAO);
            
            // Render intro image (centered)
            float introWidth = 600.0f;
            float introHeight = 300.0f;
            float introX = (SCR_WIDTH - introWidth) / 2.0f;
            float introY = SCR_HEIGHT / 2.0f - 50.0f;  // Centered vertically
            
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(introX, introY, 0.0f));
            model = glm::scale(model, glm::vec3(introWidth, introHeight, 1.0f));
            uiShader.setMat4("model", model);
            
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, introTex);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            
            // Render start button below intro
            float startButtonWidth = 200.0f;
            float startButtonHeight = 80.0f;
            float startButtonX = (SCR_WIDTH - startButtonWidth) / 2.0f;
            float startButtonY = introY - startButtonHeight - 30.0f;  // Just below intro with gap
            
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(startButtonX, startButtonY, 0.0f));
            model = glm::scale(model, glm::vec3(startButtonWidth, startButtonHeight, 1.0f));
            uiShader.setMat4("model", model);
            
            glBindTexture(GL_TEXTURE_2D, startButtonTex);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        
        // Render game over screen if game is over
        if (gameOver)
        {
            uiShader.use();  // Ensure UI shader is active
            glBindVertexArray(quadVAO);
            
            float gameOverWidth = 600.0f;
            float gameOverHeight = 400.0f;
            float gameOverX = (SCR_WIDTH - gameOverWidth) / 2.0f;
            float gameOverY = (SCR_HEIGHT - gameOverHeight) / 2.0f;
            
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(gameOverX, gameOverY, 0.0f));
            model = glm::scale(model, glm::vec3(gameOverWidth, gameOverHeight, 1.0f));
            uiShader.setMat4("model", model);
            
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, gameOverTex);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            
            // Add Quit and Restart buttons below the game over image
            float btnWidth = 200.0f;
            float btnHeight = 60.0f;
            float btnSpacing = 40.0f;
            float btnY = gameOverY - btnHeight - 30.0f;  // Below game over image
            
            // Quit button (left)
            float quitBtnX = (SCR_WIDTH / 2.0f) - btnWidth - (btnSpacing / 2.0f);
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(quitBtnX, btnY, 0.0f));
            model = glm::scale(model, glm::vec3(btnWidth, btnHeight, 1.0f));
            uiShader.setMat4("model", model);
            glBindTexture(GL_TEXTURE_2D, quitButtonTex);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            
            // Restart button (right)
            float restartBtnX = (SCR_WIDTH / 2.0f) + (btnSpacing / 2.0f);
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(restartBtnX, btnY, 0.0f));
            model = glm::scale(model, glm::vec3(btnWidth, btnHeight, 1.0f));
            uiShader.setMat4("model", model);
            glBindTexture(GL_TEXTURE_2D, restartButtonTex);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        
        // ===== SETTINGS BUTTON =====
        // Render settings button at bottom-left
        uiShader.use();
        glBindVertexArray(quadVAO);
        
        float settingsBtnSize = 60.0f;
        float settingsBtnX = 20.0f;
        float settingsBtnY = 20.0f;
        
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(settingsBtnX, settingsBtnY, 0.0f));
        model = glm::scale(model, glm::vec3(settingsBtnSize, settingsBtnSize, 1.0f));
        uiShader.setMat4("model", model);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, settingsTex);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        // ===== SETTINGS POPUP =====
        if (showSettingsPopup)
        {
            // Popup panel with new background
            float popupWidth = 600.0f;  // Updated size to match new background
            float popupHeight = 400.0f;
            float popupX = (SCR_WIDTH - popupWidth) / 2.0f;
            float popupY = (SCR_HEIGHT - popupHeight) / 2.0f;
            
            // Draw popup background using SettingBackground.png
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(popupX, popupY, 0.0f));
            model = glm::scale(model, glm::vec3(popupWidth, popupHeight, 1.0f));
            uiShader.setMat4("model", model);
            
            glBindTexture(GL_TEXTURE_2D, settingBackgroundTex);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            
            // Calculate checkbox size for later use
            float checkboxSize = 60.0f;
            float checkboxY = popupY + popupHeight / 2.0f + 20.0f;
            
            // Note: Checkbox will be rendered later on the left side with text


            
            // Close button at top-right
            float closeBtnSize = 60.0f;
            float closeBtnX = popupX + popupWidth - closeBtnSize - 20.0f;
            float closeBtnY = popupY + popupHeight - closeBtnSize - 20.0f;
            
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(closeBtnX, closeBtnY, 0.0f));
            model = glm::scale(model, glm::vec3(closeBtnSize, closeBtnSize, 1.0f));
            uiShader.setMat4("model", model);
            
            glBindTexture(GL_TEXTURE_2D, closeButtonTex);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            
            // Quit button (left of center) - reduced size with more spacing
            float quitBtnWidth = 200.0f;  // Reduced from 250
            float quitBtnHeight = 60.0f;  // Reduced from 70
            float buttonSpacing = 40.0f;  // Gap between buttons
            float quitBtnX = popupX + popupWidth / 2.0f - quitBtnWidth - buttonSpacing / 2.0f;
            float quitBtnY = popupY + 50.0f;
            
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(quitBtnX, quitBtnY, 0.0f));
            model = glm::scale(model, glm::vec3(quitBtnWidth, quitBtnHeight, 1.0f));
            uiShader.setMat4("model", model);
            
            glBindTexture(GL_TEXTURE_2D, quitButtonTex);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            
            // Restart button (right of center) - reduced size with more spacing
            float restartBtnWidth = 200.0f;  // Reduced from 250
            float restartBtnHeight = 60.0f;  // Reduced from 70
            float restartBtnX = popupX + popupWidth / 2.0f + buttonSpacing / 2.0f;
            float restartBtnY = popupY + 50.0f;


            
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(restartBtnX, restartBtnY, 0.0f));
            model = glm::scale(model, glm::vec3(restartBtnWidth, restartBtnHeight, 1.0f));
            uiShader.setMat4("model", model);
            
            glBindTexture(GL_TEXTURE_2D, restartButtonTex);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            
            // Render text labels (after UI elements)
            RenderText(textShader, "SETTING", popupX + popupWidth / 2.0f - 65.0f, popupY + popupHeight - 80.0f, 0.8f, glm::vec3(0.0f, 0.0f, 0.0f), Characters, textVAO, textVBO);
            
            // Mute status - positioned on the LEFT side
            std::string muteStatus = (currentVolume > 0.0f) ? "UNMUTED" : "MUTED";
            float checkboxLeftX = popupX + 100.0f;  // Position from left edge
            float checkboxLeftY = checkboxY;
            float muteTextX = checkboxLeftX + checkboxSize + 10.0f;  // Text to the right of checkbox
            float muteTextY = checkboxY + 15.0f;
            
            // Switch back to UI shader and render checkbox on the left
            uiShader.use();
            glBindVertexArray(quadVAO);
            
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(checkboxLeftX, checkboxLeftY, 0.0f));
            model = glm::scale(model, glm::vec3(checkboxSize, checkboxSize, 1.0f));
            uiShader.setMat4("model", model);
            
            if (currentVolume > 0.0f) {
                glBindTexture(GL_TEXTURE_2D, checkboxCheckedTex);
            } else {
                glBindTexture(GL_TEXTURE_2D, checkboxUncheckedTex);
            }
            glDrawArrays(GL_TRIANGLES, 0, 6);
            
            // Now render text to the right of checkbox
            RenderText(textShader, muteStatus, muteTextX, muteTextY, 0.6f, glm::vec3(0.0f, 0.0f, 0.0f), Characters, textVAO, textVBO);
        }


        

        
        
        glBindVertexArray(0);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVBO);

    // glfw: terminate, clearing all previously allocated GLFW resources
    glfwTerminate();
    return 0;
}

// Helper function to check if a position already has a bomb
bool HasBomb(int gridX, int gridY, const std::vector<Bomb>& bombs)
{
    for (const auto& bomb : bombs)
    {
        if (!bomb.exploded && bomb.gridX == gridX && bomb.gridY == gridY)
            return true;
    }
    return false;
}

// Helper function to check if a position can have a bomb placed
bool CanPlaceBomb(int gridX, int gridY, const std::vector<std::pair<int, int>>& breakableBlocks)
{
    // Can't place on border
    if (gridX == 0 || gridX == MAP_SIZE - 1 || gridY == 0 || gridY == MAP_SIZE - 1)
        return false;
    // Can't place on red blocks
    if (isRedBlock(gridX, gridY))
        return false;
    // Can't place on breakable blocks
    for (const auto& pos : breakableBlocks)
    {
        if (pos.first == gridX && pos.second == gridY)
            return false;
    }
    return true;
}

// Helper function to get all tiles that will be affected by bomb explosion
std::vector<std::pair<int, int>> GetExplosionTiles(const Bomb& bomb, const std::vector<std::pair<int, int>>& breakableBlocks, int explosionRange)
{
    std::vector<std::pair<int, int>> explosionTiles;
    
    // Add center (bomb position)
    explosionTiles.push_back({bomb.gridX, bomb.gridY});
    
    // Up (decrease Y)
    for (int i = 1; i <= explosionRange; i++)
    {
        int x = bomb.gridX;
        int y = bomb.gridY - i;
        
        // Stop if hit unbreakable block or border
        if (y < 0 || y >= MAP_SIZE || isRedBlock(x, y))
            break;
        
        explosionTiles.push_back({x, y});
        
        // Check if there's a breakable block here (stop after this tile if so)
        bool hitBlock = false;
        for (const auto& pos : breakableBlocks)
        {
            if (pos.first == x && pos.second == y)
            {
                hitBlock = true;
                break;
            }
        }
        
        if (hitBlock) break;
    }
    
    // Down (increase Y)
    for (int i = 1; i <= explosionRange; i++)
    {
        int x = bomb.gridX;
        int y = bomb.gridY + i;
        
        // Stop if hit unbreakable block or border
        if (y < 0 || y >= MAP_SIZE || isRedBlock(x, y))
            break;
        
        explosionTiles.push_back({x, y});
        
        // Check if there's a breakable block here
        bool hitBlock = false;
        for (const auto& pos : breakableBlocks)
        {
            if (pos.first == x && pos.second == y)
            {
                hitBlock = true;
                break;
            }
        }
        
        if (hitBlock) break;
    }
    
    // Left (decrease X)
    for (int i = 1; i <= explosionRange; i++)
    {
        int x = bomb.gridX - i;
        int y = bomb.gridY;
        
        // Stop if hit unbreakable block or border
        if (x < 0 || x >= MAP_SIZE || isRedBlock(x, y))
            break;
        
        explosionTiles.push_back({x, y});
        
        // Check if there's a breakable block here
        bool hitBlock = false;
        for (const auto& pos : breakableBlocks)
        {
            if (pos.first == x && pos.second == y)
            {
                hitBlock = true;
                break;
            }
        }
        
        if (hitBlock) break;
    }
    
    // Right (increase X)
    for (int i = 1; i <= explosionRange; i++)
    {
        int x = bomb.gridX + i;
        int y = bomb.gridY;
        
        // Stop if hit unbreakable block or border
        if (x < 0 || x >= MAP_SIZE || isRedBlock(x, y))
            break;
        
        explosionTiles.push_back({x, y});
        
        // Check if there's a breakable block here
        bool hitBlock = false;
        for (const auto& pos : breakableBlocks)
        {
            if (pos.first == x && pos.second == y)
            {
                hitBlock = true;
                break;
            }
        }
        
        if (hitBlock) break;
    }
    
    return explosionTiles;
}

// Function to explode bomb and destroy breakable blocks in cross pattern
void ExplodeBomb(const Bomb& bomb, std::vector<std::pair<int, int>>& breakableBlocks,
                 CharacterPose& leftPlayer, CharacterPose& rightPlayer, std::vector<PowerUp>& powerUps)
{
    // Determine explosion range based on bomb owner's boost
    int explosionRange = 2;  // Base range
    if (bomb.owner == 1 && leftPlayer.bombRangeBoostTimer > 0.0f) {
        explosionRange = 3;  // Boosted range
    } else if (bomb.owner ==2 && rightPlayer.bombRangeBoostTimer > 0.0f) {
        explosionRange = 3;  // Boosted range
    }
    
    // Random number generator for power-up drops
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dropChance(0.0f, 1.0f);
    const float POWERUP_DROP_RATE = 0.3f;  // 30% chance
    
    // Helper lambda to check and damage player
    auto checkPlayerDamage = [](CharacterPose& player, int x, int y) {
        if (player.gridX == x && player.gridY == y && player.invulnerabilityTimer <= 0.0f) {
            // Check if player has shield active
            if (player.shieldTimer > 0.0f) {
                std::cout << "Shield blocked damage!" << std::endl;
                return;  // Shield protects from damage
            }
            
            player.health--;
            player.invulnerabilityTimer = 1.0f;  // 1 second invulnerability after hit
            std::cout << "Player hit! Health remaining: " << player.health << std::endl;
        }
    };
    
    // Helper lambda to destroy block and possibly spawn power-up
    auto destroyBlock = [&](int x, int y) {
        bool blockDestroyed = false;
        
        // Check if there's a block to destroy
        for (const auto& pos : breakableBlocks) {
            if (pos.first == x && pos.second == y) {
                blockDestroyed = true;
                break;
            }
        }
        
        // Remove the block
        breakableBlocks.erase(
            std::remove_if(breakableBlocks.begin(), breakableBlocks.end(),
                [x, y](const std::pair<int, int>& pos) { return pos.first == x && pos.second == y; }),
            breakableBlocks.end()
        );
        
        // Randomly spawn power-up if block was destroyed
        if (blockDestroyed && dropChance(gen) < POWERUP_DROP_RATE) {
            // Randomly choose power-up type (20% each for 5 types)
            float typeRoll = dropChance(gen);
            PowerUpType type;
            if (typeRoll < 0.20f) {
                type = PowerUpType::RANGE_BOOST;
            } else if (typeRoll < 0.40f) {
                type = PowerUpType::BOMB_CAPACITY;
            } else if (typeRoll < 0.60f) {
                type = PowerUpType::SHIELD;
            } else if (typeRoll < 0.80f) {
                type = PowerUpType::SPEED_BOOST;
            } else {
                type = PowerUpType::HEART;
            }
            
            powerUps.push_back(PowerUp(x, y, type));
            std::string typeName = (type == PowerUpType::RANGE_BOOST) ? "RANGE_BOOST" :
                                  (type == PowerUpType::BOMB_CAPACITY) ? "BOMB_CAPACITY" :
                                  (type == PowerUpType::SHIELD) ? "SHIELD" :
                                  (type == PowerUpType::SPEED_BOOST) ? "SPEED_BOOST" : "HEART";
            std::cout << "Power-up spawned at (" << x << ", " << y << ") - Type: " << typeName << std::endl;
        }
        
        return blockDestroyed;
    };
    
    // Check center (bomb position)
    checkPlayerDamage(leftPlayer, bomb.gridX, bomb.gridY);
    checkPlayerDamage(rightPlayer, bomb.gridX, bomb.gridY);
    
    // Up (decrease Y)
    for (int i = 1; i <= explosionRange; i++)
    {
        int x = bomb.gridX;
        int y = bomb.gridY - i;
        
        // Stop if hit unbreakable block or border
        if (y < 0 || y >= MAP_SIZE || isRedBlock(x, y))
            break;
        
        // Check player damage
        checkPlayerDamage(leftPlayer, x, y);
        checkPlayerDamage(rightPlayer, x, y);
        
        // Destroy block and possibly spawn power-up
        if (destroyBlock(x, y)) break;  // Stop explosion after destroying block
    }
    
    // Down (increase Y)
    for (int i = 1; i <= explosionRange; i++)
    {
        int x = bomb.gridX;
        int y = bomb.gridY + i;
        
        // Stop if hit unbreakable block or border
        if (y < 0 || y >= MAP_SIZE || isRedBlock(x, y))
            break;
        
        // Check player damage
        checkPlayerDamage(leftPlayer, x, y);
        checkPlayerDamage(rightPlayer, x, y);
        
        // Destroy block and possibly spawn power-up
        if (destroyBlock(x, y)) break;
    }
    
    // Left (decrease X)
    for (int i = 1; i <= explosionRange; i++)
    {
        int x = bomb.gridX - i;
        int y = bomb.gridY;
        
        // Stop if hit unbreakable block or border
        if (x < 0 || x >= MAP_SIZE || isRedBlock(x, y))
            break;
        
        // Check player damage
        checkPlayerDamage(leftPlayer, x, y);
        checkPlayerDamage(rightPlayer, x, y);
        
        // Destroy block and possibly spawn power-up
        if (destroyBlock(x, y)) break;
    }
    
    // Right (increase X)
    for (int i = 1; i <= explosionRange; i++)
    {
        int x = bomb.gridX + i;
        int y = bomb.gridY;
        
        // Stop if hit unbreakable block or border
        if (x < 0 || x >= MAP_SIZE || isRedBlock(x, y))
            break;
        
        // Check player damage
        checkPlayerDamage(leftPlayer, x, y);
        checkPlayerDamage(rightPlayer, x, y);
        
        // Destroy block and possibly spawn power-up
        if (destroyBlock(x, y)) break;
    }
}

// Update bombs: countdown timers and explosions
void UpdateBombs(std::vector<Bomb>& bombs, std::vector<std::pair<int, int>>& breakableBlocks,
                 CharacterPose& leftPlayer, CharacterPose& rightPlayer, float deltaTime, std::vector<PowerUp>& powerUps)
{
    // Update timers
    if (leftPlayer.invulnerabilityTimer > 0) leftPlayer.invulnerabilityTimer -= deltaTime;
    if (rightPlayer.invulnerabilityTimer > 0) rightPlayer.invulnerabilityTimer -= deltaTime;
    if (leftPlayer.bombRangeBoostTimer > 0) leftPlayer.bombRangeBoostTimer -= deltaTime;
    if (rightPlayer.bombRangeBoostTimer > 0) rightPlayer.bombRangeBoostTimer -= deltaTime;
    if (leftPlayer.shieldTimer > 0) leftPlayer.shieldTimer -= deltaTime;
    if (rightPlayer.shieldTimer > 0) rightPlayer.shieldTimer -= deltaTime;
    if (leftPlayer.speedBoostTimer > 0) leftPlayer.speedBoostTimer -= deltaTime;
    if (rightPlayer.speedBoostTimer > 0) rightPlayer.speedBoostTimer -= deltaTime;
    
    for (auto& bomb : bombs)
    {
        if (!bomb.exploded)
        {
            bomb.timer -= deltaTime;
            
            if (bomb.timer <= 0.0f)
            {
                // Explode!
                ExplodeBomb(bomb, breakableBlocks, leftPlayer, rightPlayer, powerUps);
                bomb.exploded = true;
                
                // Decrement active bomb count for the owner
                if (bomb.owner == 1)
                {
                    leftPlayer.activeBombCount--;
                    std::cout << "P1 bomb exploded. Active bombs: " << leftPlayer.activeBombCount << "/" << leftPlayer.maxBombCount << std::endl;
                }
                else if (bomb.owner == 2)
                {
                    rightPlayer.activeBombCount--;
                    std::cout << "P2 bomb exploded. Active bombs: " << rightPlayer.activeBombCount << "/" << rightPlayer.maxBombCount << std::endl;
                }
            }
        }
    }
    
    // Remove exploded bombs (optional - can keep them for visual effects)
    // For now, we'll keep them but mark as exploded
}

// Update power-ups: handle spinning animation and pickup detection
void UpdatePowerUps(std::vector<PowerUp>& powerUps, CharacterPose& leftPlayer, CharacterPose& rightPlayer, float deltaTime, std::vector<AudioPlayer>& pickupSounds, int& pickupSoundIndex)
{
    const float SPIN_SPEED = 2.0f;  // Rotations per second
    const float BOOST_DURATION = 10.0f;  // 10 seconds of boost
    
    //Update spin animation for all power-ups
    for (auto& powerUp : powerUps)
    {
        powerUp.rotation += SPIN_SPEED * deltaTime * 2.0f * 3.14159f;  // Convert to radians
    }
    
    // Check for pickups
    for (auto it = powerUps.begin(); it != powerUps.end(); )
    {
        bool pickedUp = false;
        
        // Check P1 pickup
        if (leftPlayer.gridX == it->gridX && leftPlayer.gridY == it->gridY)
        {
            if (it->type == PowerUpType::RANGE_BOOST) {
                leftPlayer.bombRangeBoostTimer = BOOST_DURATION;
                std::cout << "P1 picked up RANGE BOOST! Bomb range +1 for 10 seconds" << std::endl;
            } else if (it->type == PowerUpType::BOMB_CAPACITY) {
                leftPlayer.maxBombCount++;
                std::cout << "P1 picked up BOMB CAPACITY! Max bombs: " << leftPlayer.maxBombCount << std::endl;
            } else if (it->type == PowerUpType::SHIELD) {
                leftPlayer.shieldTimer = 5.0f;  // Shield lasts 5 seconds
                std::cout << "P1 picked up SHIELD! Protected from damage for 5 seconds" << std::endl;
            } else if (it->type == PowerUpType::SPEED_BOOST) {
                leftPlayer.speedBoostTimer = BOOST_DURATION;  // Speed boost lasts 10 seconds
                std::cout << "P1 picked up SPEED BOOST! Movement speed increased for 10 seconds" << std::endl;
            } else if (it->type == PowerUpType::HEART) {
                if (leftPlayer.health < 3) {
                    leftPlayer.health++;
                    std::cout << "P1 picked up HEART! Health restored to " << leftPlayer.health << "/3" << std::endl;
                } else {
                    std::cout << "P1 picked up HEART but health is already full (3/3)" << std::endl;
                }
            }
            pickedUp = true;
        }
        // Check P2 pickup
        else if (rightPlayer.gridX == it->gridX && rightPlayer.gridY == it->gridY)
        {
            if (it->type == PowerUpType::RANGE_BOOST) {
                rightPlayer.bombRangeBoostTimer = BOOST_DURATION;
                std::cout << "P2 picked up RANGE BOOST! Bomb range +1 for 10 seconds" << std::endl;
            } else if (it->type == PowerUpType::BOMB_CAPACITY) {
                rightPlayer.maxBombCount++;
                std::cout << "P2 picked up BOMB CAPACITY! Max bombs: " << rightPlayer.maxBombCount << std::endl;
            } else if (it->type == PowerUpType::SHIELD) {
                rightPlayer.shieldTimer = 5.0f;  // Shield lasts 5 seconds
                std::cout << "P2 picked up SHIELD! Protected from damage for 5 seconds" << std::endl;
            } else if (it->type == PowerUpType::SPEED_BOOST) {
                rightPlayer.speedBoostTimer = BOOST_DURATION;  // Speed boost lasts 10 seconds
                std::cout << "P2 picked up SPEED BOOST! Movement speed increased for 10 seconds" << std::endl;
            } else if (it->type == PowerUpType::HEART) {
                if (rightPlayer.health < 3) {
                    rightPlayer.health++;
                    std::cout << "P2 picked up HEART! Health restored to " << rightPlayer.health << "/3" << std::endl;
                } else {
                    std::cout << "P2 picked up HEART but health is already full (3/3)" << std::endl;
                }
            }
            pickedUp = true;
        }
        
        if (pickedUp)
        {
            // Play pickup sound from pool (allows stacking)
            pickupSounds[pickupSoundIndex].play();
            pickupSoundIndex = (pickupSoundIndex + 1) % pickupSounds.size();
            
            it = powerUps.erase(it);  // Remove picked up power-up
        }
        else
        {
            ++it;
        }
    }
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window, std::vector<std::pair<int, int>>& breakableBlockPositions, 
                 std::mt19937& gen, const std::function<void(std::vector<std::pair<int, int>>&, std::mt19937&)>& generateBlocks,
                 CharacterPose& leftCharacter, CharacterPose& rightCharacter,
                 std::vector<Bomb>& bombs, bool gameOver, std::vector<AudioPlayer>& bombSounds, int& bombSoundIndex)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    // Disable all game controls if game is over
    if (gameOver)
        return;
    
    // Bomb placement: P1 (Q key) and P2 (M key)
    static bool qKeyPressed = false;
    static bool mKeyPressed = false;
    
    // P1 places bomb with Q
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && !qKeyPressed)
    {
        qKeyPressed = true;
        int bombX = leftCharacter.gridX;
        int bombY = leftCharacter.gridY;
        
        // Check bomb limit (max 3 active bombs)
        if (leftCharacter.activeBombCount < leftCharacter.maxBombCount &&
            CanPlaceBomb(bombX, bombY, breakableBlockPositions) && 
            !HasBomb(bombX, bombY, bombs))
        {
            bombs.push_back(Bomb(bombX, bombY, 1));
            leftCharacter.activeBombCount++;
            
            // Play bomb placement sound from pool (allows stacking)
            bombSounds[bombSoundIndex].play();
            bombSoundIndex = (bombSoundIndex + 1) % bombSounds.size();
            
            std::cout << "P1 placed bomb at (" << bombX << ", " << bombY << ") [" 
                      << leftCharacter.activeBombCount << "/3]" << std::endl;
        }
        else if (leftCharacter.activeBombCount >= leftCharacter.maxBombCount)
        {
            std::cout << "P1 bomb limit reached (" << leftCharacter.activeBombCount << "/" << leftCharacter.maxBombCount << ")" << std::endl;
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_RELEASE)
    {
        qKeyPressed = false;
    }
    
    // P2 places bomb with M
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS && !mKeyPressed)
    {
        mKeyPressed = true;
        int bombX = rightCharacter.gridX;
        int bombY = rightCharacter.gridY;
        
        // Check bomb limit (max 3 active bombs)
        if (rightCharacter.activeBombCount < rightCharacter.maxBombCount &&
            CanPlaceBomb(bombX, bombY, breakableBlockPositions) && 
            !HasBomb(bombX, bombY, bombs))
        {
            bombs.push_back(Bomb(bombX, bombY, 2));
            rightCharacter.activeBombCount++;
            
            // Play bomb placement sound from pool (allows stacking)
            bombSounds[bombSoundIndex].play();
            bombSoundIndex = (bombSoundIndex + 1) % bombSounds.size();
            
            std::cout << "P2 placed bomb at (" << bombX << ", " << bombY << ") [" 
                      << rightCharacter.activeBombCount << "/3]" << std::endl;
        }
        else if (rightCharacter.activeBombCount >= rightCharacter.maxBombCount)
        {
            std::cout << "P2 bomb limit reached (" << rightCharacter.activeBombCount << "/" << rightCharacter.maxBombCount << ")" << std::endl;
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_M) == GLFW_RELEASE)
    {
        mKeyPressed = false;
    }

    // Character Movement Input
    // Left character (P1) uses WASD
    bool leftMoving = ProcessCharacterInput(window, leftCharacter, 
                                            GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D,
                                            breakableBlockPositions, rightCharacter, bombs);
    
    // Right character (P2) uses Arrow keys
    bool rightMoving = ProcessCharacterInput(window, rightCharacter,
                                              GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_LEFT, GLFW_KEY_RIGHT,
                                              breakableBlockPositions, leftCharacter, bombs);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions
    glViewport(0, 0, width, height);
}

unsigned int loadCubemap(const std::vector<std::string>& faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            GLenum format = nrChannels == 4 ? GL_RGBA : GL_RGB;
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

// Function to render text
void RenderText(Shader& shader, const std::string& text, float x, float y, float scale, 
                glm::vec3 color, const std::map<char, Character>& Characters, unsigned int VAO, unsigned int VBO)
{
    shader.use();
    shader.setVec3("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        auto it = Characters.find(*c);
        if (it == Characters.end())
            continue;  // Skip characters not in our map
            
        Character ch = it->second;

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },            
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }           
        };
        
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        // advance cursors for next glyph
        x += (ch.Advance >> 6) * scale;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

