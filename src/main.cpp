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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, std::vector<std::pair<int, int>>& breakableBlockPositions, 
                 std::mt19937& gen, const std::function<void(std::vector<std::pair<int, int>>&, std::mt19937&)>& generateBlocks);
unsigned int loadCubemap(const std::vector<std::string>& faces);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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

int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

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
        std::cout << "Failed to load border texture. Trying alternative path..." << std::endl;
        borderTexture = loadTexture("assets/Unbreakable_Block/tudor_wall_01_basecolor_1k.png");
    }

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

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window, breakableBlockPositions, gen, generateBreakableBlocks);

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

        // glfw: swap buffers and poll IO events

        // draw skybox last
        glDepthFunc(GL_LEQUAL);
        skyboxShader.use();
        glm::mat4 viewNoTranslation = glm::mat4(glm::mat3(view));
        skyboxShader.setMat4("view", viewNoTranslation);
        skyboxShader.setMat4("projection", projection);
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);

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

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window, std::vector<std::pair<int, int>>& breakableBlockPositions, 
                 std::mt19937& gen, const std::function<void(std::vector<std::pair<int, int>>&, std::mt19937&)>& generateBlocks)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    // R key to regenerate breakable blocks
    static bool rKeyPressed = false;
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && !rKeyPressed)
    {
        rKeyPressed = true;
        generateBlocks(breakableBlockPositions, gen);
        std::cout << "Breakable blocks regenerated! (" << breakableBlockPositions.size() << " blocks)" << std::endl;
    }
    else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE)
    {
        rKeyPressed = false;
    }
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
