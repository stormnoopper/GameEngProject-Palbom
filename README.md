# Game Engine Project - PalBomb

โปรเจคเกม Bomber-Style Top-Down Map ที่ใช้ OpenGL สำหรับแสดงแผนที่แบบ 3D พร้อมระบบ texture mapping

## 📋 คำอธิบายโปรเจค

โปรเจคนี้เป็นเกม Bomber-Style Top-Down Map ที่แสดงแผนที่ขนาด 15x15 tiles โดยมีพื้นและขอบที่สามารถทำลายได้และทำลายไม่ได้ ใช้ระบบ Perspective Camera แบบ top-down view

## ✨ คุณสมบัติ

- **3D Tile Rendering**: แสดงแผนที่แบบ 3D ด้วย tiles
- **Texture Mapping**: ใช้ texture สำหรับพื้นและขอบ
- **Top-Down Camera**: กล้องมองจากด้านบน
- **Border System**: แสดงขอบแผนที่ที่สูงกว่าพื้น
- **Dynamic Rendering**: เรนเดอร์ทุก frame ด้วย render loop

## 🎮 การควบคุม

- **ESC**: ปิดเกม

## 🛠️ เทคโนโลยีที่ใช้

- **OpenGL 3.3**: สำหรับการเรนเดอร์กราฟิก 3D
- **GLFW**: สำหรับการจัดการหน้าต่างและ input
- **GLAD**: สำหรับ OpenGL function loader
- **GLM**: สำหรับการคำนวณทางคณิตศาสตร์ (vectors, matrices)
- **STB Image**: สำหรับโหลดไฟล์ texture (PNG)
- **CMake**: สำหรับ build system

## 📁 โครงสร้างโปรเจค

```
GameEngProject-Palbom/
├── assets/              # ไฟล์ texture
│   ├── floor/          # Texture สำหรับพื้น
│   └── Unbreakable_Block/  # Texture สำหรับขอบ
├── shaders/            # Shader files
│   ├── tile.vs        # Vertex shader
│   └── tile.fs        # Fragment shader
├── src/                # Source code
│   └── main.cpp       # Main game loop
└── CMakeLists.txt     # CMake configuration
```

## 🚀 วิธีติดตั้งและรัน

### ความต้องการของระบบ

- CMake 3.16 หรือสูงกว่า
- C++17 compiler
- OpenGL 3.3 หรือสูงกว่า

### ขั้นตอนการ Build

1. **Clone หรือดาวน์โหลดโปรเจค**

2. **สร้าง build directory และ build โปรเจค**:
```bash
cmake -S . -B build
cmake --build build
```

3. **รันโปรแกรม**:
```bash
cd build
./PlayableCharacter
```

### สำหรับ Windows

```bash
cmake -S . -B build
cmake --build build --config Release
cd build/Release
PlayableCharacter.exe
```

## 💻 คำอธิบายโค้ด (Code Explanation)

### 📄 main.cpp - โค้ดหลักของโปรแกรม

#### ส่วนที่ 1: Includes และ Headers

```cpp
#include <glad/glad.h>      // โหลด OpenGL functions
#include <GLFW/glfw3.h>     // จัดการหน้าต่างและ input
#include <stb_image.h>      // โหลดรูปภาพ (texture)
#include <glm/glm.hpp>      // Math library สำหรับ vectors และ matrices
#include <learnopengl/shader_m.h>  // Shader class จาก LearnOpenGL
```

**คำอธิบาย:**
- **glad/glad.h**: โหลด OpenGL function pointers ที่จำเป็นสำหรับการเรียกใช้ OpenGL API
- **GLFW/glfw3.h**: Library สำหรับจัดการหน้าต่าง, keyboard input, และ events
- **stb_image.h**: Single-header library สำหรับโหลดไฟล์รูปภาพ (PNG, JPG, BMP, etc.)
- **glm**: Math library สำหรับคำนวณ matrices, vectors, และ transformations
- **shader_m.h**: Class สำหรับจัดการ shader programs (compile, link, set uniforms)

#### ส่วนที่ 2: Function Declarations และ Constants

```cpp
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;   // ความกว้างหน้าต่าง
const unsigned int SCR_HEIGHT = 600;  // ความสูงหน้าต่าง
```

**คำอธิบาย:**
- **framebuffer_size_callback**: Callback function ที่จะถูกเรียกเมื่อผู้ใช้เปลี่ยนขนาดหน้าต่าง
- **processInput**: ฟังก์ชันสำหรับตรวจสอบ keyboard input
- **SCR_WIDTH/SCR_HEIGHT**: กำหนดขนาดหน้าต่างเริ่มต้นเป็น 800x600 pixels

#### ส่วนที่ 3: loadTexture() Function

```cpp
unsigned int loadTexture(const char* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);  // สร้าง texture ID
    
    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    // โหลดรูปภาพจากไฟล์
}
```

**คำอธิบายทีละขั้นตอน:**

1. **glGenTextures()**: สร้าง texture object และได้ texture ID
2. **stbi_load()**: โหลดรูปภาพจากไฟล์และได้ข้อมูล pixel data
   - `path`: path ของไฟล์รูปภาพ
   - `width, height`: ขนาดของรูปภาพ
   - `nrComponents`: จำนวน channels (1=grayscale, 3=RGB, 4=RGBA)

3. **Format Detection**:
```cpp
if (nrComponents == 1)
    format = GL_RED;      // Grayscale
else if (nrComponents == 3)
    format = GL_RGB;      // RGB (ไม่มี alpha)
else if (nrComponents == 4)
    format = GL_RGBA;     // RGBA (มี alpha channel)
```

4. **Upload to GPU**:
```cpp
glBindTexture(GL_TEXTURE_2D, textureID);
glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
```
- **glBindTexture()**: กำหนด texture ที่จะใช้งาน
- **glTexImage2D()**: อัปโหลดข้อมูล pixel ไปยัง GPU

5. **Generate Mipmaps**:
```cpp
glGenerateMipmap(GL_TEXTURE_2D);
```
สร้าง mipmaps สำหรับ texture filtering เมื่อ texture ถูก scale

6. **Texture Parameters**:
```cpp
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
```
- **GL_TEXTURE_WRAP_S/T**: กำหนดการ wrap texture (GL_REPEAT = ซ้ำ texture)
- **GL_TEXTURE_MIN_FILTER**: Filtering เมื่อ texture เล็กลง (ใช้ mipmap)
- **GL_TEXTURE_MAG_FILTER**: Filtering เมื่อ texture ใหญ่ขึ้น (ใช้ linear)

#### ส่วนที่ 4: main() Function - Initialization

**GLFW Initialization:**
```cpp
glfwInit();
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
```
- **glfwInit()**: เริ่มต้น GLFW library
- **glfwWindowHint()**: กำหนด OpenGL version เป็น 3.3 และใช้ Core Profile

**macOS Compatibility:**
```cpp
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
```
สำหรับ macOS ต้องเปิด forward compatibility

**Window Creation:**
```cpp
GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Bomber-Style Top-Down Map", NULL, NULL);
glfwMakeContextCurrent(window);
glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
```
- สร้างหน้าต่างขนาด 800x600
- กำหนดให้เป็น current context
- ตั้งค่า callback สำหรับเมื่อเปลี่ยนขนาดหน้าต่าง

**GLAD Initialization:**
```cpp
if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
{
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
}
```
โหลด OpenGL function pointers ผ่าน GLAD

**OpenGL State Configuration:**
```cpp
glEnable(GL_DEPTH_TEST);
```
เปิดใช้งาน depth testing เพื่อให้เรนเดอร์ 3D objects ถูกต้อง (objects ที่อยู่ใกล้จะบัง objects ที่อยู่ไกล)

#### ส่วนที่ 5: Shader Setup

```cpp
Shader shader("shaders/tile.vs", "shaders/tile.fs");
```
โหลดและ compile vertex shader และ fragment shader จากไฟล์

#### ส่วนที่ 6: Vertex Data Setup

**Block Geometry:**
```cpp
float blockHeight = 0.2f;  // ความสูงของบล็อกพื้น (0.2 units)
float blockSize = 1.0f;    // ขนาดของบล็อก (1.0 unit)
```

**Vertex Array:**
```cpp
float vertices[] = {
    // positions (x, y, z)    // texture coords (u, v)
    // Top face
     0.5f,  blockHeight,  0.5f,   1.0f, 1.0f,  // Top-right
     0.5f,  blockHeight, -0.5f,   1.0f, 0.0f,  // Bottom-right
    -0.5f,  blockHeight, -0.5f,   0.0f, 0.0f,  // Bottom-left
    -0.5f,  blockHeight,  0.5f,   0.0f, 1.0f,  // Top-left
    // ... (faces อื่นๆ)
};
```

**คำอธิบาย Vertex Data:**
- แต่ละ vertex มี 5 floats: 3 สำหรับ position (x, y, z) และ 2 สำหรับ texture coordinates (u, v)
- Cube มี 6 faces: Top, Bottom, Front, Back, Right, Left
- แต่ละ face มี 4 vertices (quad) ซึ่งจะถูกวาดเป็น 2 triangles

**Index Array:**
```cpp
unsigned int indices[] = {
    // Top face (2 triangles)
    0, 1, 2,  2, 3, 0,  // Triangle 1: (0,1,2), Triangle 2: (2,3,0)
    // Bottom face
    4, 5, 6,  6, 7, 4,
    // ... (faces อื่นๆ)
};
```
- ใช้ indices เพื่อลดจำนวน vertices ที่ซ้ำกัน
- แต่ละ face ใช้ 6 indices (2 triangles × 3 vertices)
- ทั้งหมด 36 indices (6 faces × 6 indices)

**VAO/VBO/EBO Setup:**
```cpp
unsigned int VBO, VAO, EBO;
glGenVertexArrays(1, &VAO);  // สร้าง Vertex Array Object
glGenBuffers(1, &VBO);       // สร้าง Vertex Buffer Object
glGenBuffers(1, &EBO);       // สร้าง Element Buffer Object
```

**คำอธิบาย:**
- **VAO (Vertex Array Object)**: เก็บการตั้งค่า vertex attributes ทั้งหมด
- **VBO (Vertex Buffer Object)**: เก็บข้อมูล vertex (positions + texture coords)
- **EBO (Element Buffer Object)**: เก็บข้อมูล indices

**Upload Data to GPU:**
```cpp
glBindVertexArray(VAO);

glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
```
- **glBufferData()**: อัปโหลดข้อมูลไปยัง GPU
- **GL_STATIC_DRAW**: ข้อมูลจะไม่เปลี่ยนแปลง (เหมาะสำหรับ geometry)

**Vertex Attributes:**
```cpp
// Position attribute (location = 0)
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

// Texture coordinate attribute (location = 1)
glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
glEnableVertexAttribArray(1);
```

**คำอธิบาย glVertexAttribPointer():**
- **0/1**: Attribute location (ต้องตรงกับ shader)
- **3/2**: จำนวน components (position=3, texCoord=2)
- **GL_FLOAT**: ประเภทข้อมูล
- **GL_FALSE**: ไม่ normalize
- **5 * sizeof(float)**: Stride (ระยะห่างระหว่าง vertices)
- **offset**: ตำแหน่งเริ่มต้นของ attribute ใน vertex

#### ส่วนที่ 7: Texture Loading

```cpp
stbi_set_flip_vertically_on_load(true);  // พลิกรูปภาพตามแนวตั้ง
std::string texturePath = FileSystem::getPath("assets/floor/ground_tiles_06_color_1k.png");
unsigned int floorTexture = loadTexture(texturePath.c_str());
```

**คำอธิบาย:**
- **stbi_set_flip_vertically_on_load()**: พลิกรูปภาพเพราะ OpenGL coordinate system มี origin ที่มุมล่างซ้าย แต่รูปภาพมีที่มุมบนซ้าย
- **FileSystem::getPath()**: หา path ที่ถูกต้องของไฟล์ (รองรับทั้ง build directory และ source directory)
- **loadTexture()**: โหลด texture และได้ texture ID

```cpp
unsigned int borderTexture = loadTexture("assets/Unbreakable_Block/tudor_wall_01_basecolor_1k.png");
```
โหลด texture สำหรับขอบแผนที่

**Set Shader Uniform:**
```cpp
shader.use();
shader.setInt("texture1", 0);
```
- กำหนด texture unit 0 ให้กับ uniform "texture1" ใน shader

#### ส่วนที่ 8: Map Configuration

```cpp
const int MAP_SIZE = 15;           // ขนาดแผนที่ 15x15 tiles
const float TILE_SIZE = 1.0f;      // ขนาดแต่ละ tile (1.0 unit)
const float MAP_OFFSET = -(MAP_SIZE - 1) * TILE_SIZE / 2.0f;  // จัดกึ่งกลางแผนที่
```

**คำอธิบาย MAP_OFFSET:**
- คำนวณเพื่อให้แผนที่อยู่กึ่งกลางที่ origin (0, 0, 0)
- สำหรับ 15 tiles: offset = -(15-1) * 1.0 / 2.0 = -7.0
- ดังนั้น tiles จะอยู่ที่ x = -7, -6, ..., 0, ..., 6, 7

#### ส่วนที่ 9: Render Loop

```cpp
while (!glfwWindowShouldClose(window))
{
    // ... rendering code ...
}
```
Loop หลักที่ทำงานจนกว่าผู้ใช้จะปิดหน้าต่าง

**Input Processing:**
```cpp
processInput(window);
```
ตรวจสอบ keyboard input (เช่น ESC เพื่อปิดเกม)

**Clear Screen:**
```cpp
glClearColor(0.1f, 0.1f, 0.15f, 1.0f);  // สีพื้นหลัง (R, G, B, Alpha)
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
```
- **glClearColor()**: กำหนดสีพื้นหลัง (สีน้ำเงินเข้ม)
- **glClear()**: ล้าง color buffer และ depth buffer

**Activate Shader:**
```cpp
shader.use();
```
เปิดใช้งาน shader program

**Projection Matrix:**
```cpp
float aspect = static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT);
glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 200.0f);
```

**คำอธิบาย:**
- **aspect**: อัตราส่วนความกว้าง/สูงของหน้าต่าง (800/600 = 1.333)
- **glm::perspective()**: สร้าง perspective projection matrix
  - **45.0f**: Field of View (FOV) เป็นองศา
  - **aspect**: Aspect ratio
  - **0.1f**: Near plane (ระยะใกล้สุดที่เห็น)
  - **200.0f**: Far plane (ระยะไกลสุดที่เห็น)

**View Matrix (Camera):**
```cpp
glm::vec3 cameraPos(0.0f, MAP_SIZE * 0.9f, MAP_SIZE * 1.1f);
glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
```

**คำอธิบาย:**
- **cameraPos**: ตำแหน่งกล้อง (0, 13.5, 16.5) - อยู่ด้านบนและด้านหลัง
- **cameraTarget**: จุดที่กล้องมอง (0, 0, 0) - กึ่งกลางแผนที่
- **cameraUp**: ทิศทางขึ้น (0, 1, 0) - แกน Y
- **glm::lookAt()**: สร้าง view matrix จากตำแหน่งกล้อง, จุดมอง, และทิศทางขึ้น

**Set Shader Uniforms:**
```cpp
shader.setMat4("projection", projection);
shader.setMat4("view", view);
```
ส่ง matrices ไปยัง shader

**Rendering Floor Tiles:**
```cpp
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, floorTexture);
glBindVertexArray(VAO);

for (int x = 0; x < MAP_SIZE; x++)
{
    for (int z = 0; z < MAP_SIZE; z++)
    {
        float tileX = MAP_OFFSET + x * TILE_SIZE;
        float tileZ = MAP_OFFSET + z * TILE_SIZE;
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(tileX, 0.0f, tileZ));
        shader.setMat4("model", model);
        
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
}
```

**คำอธิบายทีละขั้นตอน:**

1. **Bind Texture**: กำหนด texture ที่จะใช้ (floorTexture)
2. **Bind VAO**: กำหนด vertex array ที่จะใช้
3. **Nested Loop**: Loop ผ่านทุกตำแหน่งในแผนที่ (15×15 = 225 tiles)
4. **Calculate Position**: คำนวณตำแหน่งแต่ละ tile
   - `tileX = -7.0 + x * 1.0` (x = 0..14)
   - `tileZ = -7.0 + z * 1.0` (z = 0..14)
5. **Model Matrix**: สร้าง transformation matrix
   - `glm::mat4(1.0f)`: Identity matrix
   - `glm::translate()`: ย้ายตำแหน่งไปที่ (tileX, 0, tileZ)
6. **Set Uniform**: ส่ง model matrix ไปยัง shader
7. **Draw**: วาด cube ด้วย `glDrawElements()`
   - **GL_TRIANGLES**: วาดเป็น triangles
   - **36**: จำนวน indices (6 faces × 6 indices)

**Rendering Border Blocks:**
```cpp
glBindTexture(GL_TEXTURE_2D, borderTexture);
const float fullBlockHeight = 1.0f;
const float borderScaleY = fullBlockHeight / blockHeight;  // 5.0 (1.0 / 0.2)

for (int x = 0; x < MAP_SIZE; x++)
{
    for (int z = 0; z < MAP_SIZE; z++)
    {
        bool isBorder = (x == 0 || x == MAP_SIZE - 1 || z == 0 || z == MAP_SIZE - 1);
        if (!isBorder)
            continue;  // ข้าม tiles ที่ไม่ใช่ขอบ
        
        float tileX = MAP_OFFSET + x * TILE_SIZE;
        float tileZ = MAP_OFFSET + z * TILE_SIZE;
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(tileX, blockHeight, tileZ));
        model = glm::scale(model, glm::vec3(1.0f, borderScaleY, 1.0f));
        shader.setMat4("model", model);
        
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
}
```

**คำอธิบาย:**
- **isBorder**: ตรวจสอบว่าอยู่ที่ขอบแผนที่หรือไม่ (แถวแรก/สุดท้าย หรือคอลัมน์แรก/สุดท้าย)
- **Translate**: ย้ายตำแหน่งขึ้นไปที่ `blockHeight` (0.2) เพื่อวางบนพื้น
- **Scale**: Scale ในแนว Y ด้วย `borderScaleY` (5.0) เพื่อให้สูงขึ้นเป็น 1.0 unit
- **Result**: บล็อกขอบจะสูง 1.0 unit และอยู่บนพื้นที่สูง 0.2 unit

**Buffer Swap:**
```cpp
glfwSwapBuffers(window);  // สลับ front และ back buffer (double buffering)
glfwPollEvents();         // ตรวจสอบ events (keyboard, mouse, window)
```
- **Double Buffering**: วาดที่ back buffer แล้วสลับมาแสดงที่หน้าจอ เพื่อป้องกัน flickering
- **Poll Events**: อัปเดต input และ window events

#### ส่วนที่ 10: Cleanup

```cpp
glDeleteVertexArrays(1, &VAO);
glDeleteBuffers(1, &VBO);
glDeleteBuffers(1, &EBO);
glfwTerminate();
return 0;
```

**คำอธิบาย:**
- **glDeleteVertexArrays()**: ลบ VAO
- **glDeleteBuffers()**: ลบ VBO และ EBO
- **glfwTerminate()**: ปิด GLFW และล้าง resources ทั้งหมด

#### ส่วนที่ 11: Helper Functions

**processInput():**
```cpp
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
```

**คำอธิบาย:**
- **glfwGetKey()**: ตรวจสอบว่าปุ่มถูกกดหรือไม่
- **GLFW_KEY_ESCAPE**: ปุ่ม ESC
- **GLFW_PRESS**: กำลังกดอยู่
- **glfwSetWindowShouldClose()**: ตั้งค่าให้ render loop หยุดทำงาน

**framebuffer_size_callback():**
```cpp
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
```

**คำอธิบาย:**
- ถูกเรียกเมื่อผู้ใช้เปลี่ยนขนาดหน้าต่าง
- **glViewport()**: กำหนดพื้นที่ที่ OpenGL จะวาด (0, 0 ถึง width, height)
- จำเป็นเพื่อให้ภาพไม่บิดเบี้ยวเมื่อเปลี่ยนขนาดหน้าต่าง

### 🎨 Shader Files

#### tile.vs (Vertex Shader)

```glsl
#version 330 core
layout (location = 0) in vec3 aPos;        // รับ position จาก CPU
layout (location = 1) in vec2 aTexCoord;   // รับ texture coordinates จาก CPU

out vec2 TexCoord;  // ส่ง texture coordinates ไปยัง fragment shader

uniform mat4 model;       // Model matrix (transform object)
uniform mat4 view;        // View matrix (camera)
uniform mat4 projection;  // Projection matrix (perspective)

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}
```

**คำอธิบายทีละบรรทัด:**

1. **#version 330 core**: ใช้ GLSL version 3.30 core profile
2. **layout (location = 0) in vec3 aPos**: 
   - รับ vertex position (3 floats: x, y, z)
   - location = 0 ต้องตรงกับ `glVertexAttribPointer(0, ...)`
3. **layout (location = 1) in vec2 aTexCoord**: 
   - รับ texture coordinates (2 floats: u, v)
   - location = 1 ต้องตรงกับ `glVertexAttribPointer(1, ...)`
4. **out vec2 TexCoord**: ส่ง texture coordinates ไปยัง fragment shader
5. **uniform mat4**: รับ matrices จาก CPU (ไม่เปลี่ยนแปลงระหว่าง vertices)
6. **MVP Transformation**: 
   - `projection × view × model × position`
   - Transform จาก local space → world space → view space → clip space
7. **gl_Position**: Output position ใน clip space (ต้องเป็น vec4)
8. **TexCoord = aTexCoord**: ส่ง texture coordinates ต่อไป

**MVP Transformation Pipeline:**
- **Model Matrix**: Transform จาก local space (object) → world space
- **View Matrix**: Transform จาก world space → view space (camera space)
- **Projection Matrix**: Transform จาก view space → clip space (perspective)

#### tile.fs (Fragment Shader)

```glsl
#version 330 core
out vec4 FragColor;           // สี output ของ pixel

in vec2 TexCoord;             // รับ texture coordinates จาก vertex shader

uniform sampler2D texture1;   // Texture sampler

void main()
{
    FragColor = texture(texture1, TexCoord);
}
```

**คำอธิบายทีละบรรทัด:**

1. **#version 330 core**: ใช้ GLSL version 3.30 core profile
2. **out vec4 FragColor**: สี output ของ pixel (R, G, B, Alpha)
3. **in vec2 TexCoord**: รับ texture coordinates จาก vertex shader (interpolated)
4. **uniform sampler2D texture1**: Texture sampler สำหรับอ่านสีจาก texture
5. **texture()**: Function สำหรับอ่านสีจาก texture
   - `texture1`: Texture sampler
   - `TexCoord`: Texture coordinates (u, v) ที่จะอ่าน
6. **FragColor**: ส่งสีไปยัง framebuffer

**Fragment Shader Process:**
- Vertex shader ส่ง texture coordinates ไปยัง fragment shader
- GPU จะ interpolate texture coordinates สำหรับแต่ละ pixel
- Fragment shader อ่านสีจาก texture ตาม coordinates
- ส่งสีไปยัง framebuffer

### 🔧 CMakeLists.txt - Build Configuration

#### ส่วนที่ 1: Download LearnOpenGL Headers

```cmake
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/learnopengl)

file(WRITE ${CMAKE_BINARY_DIR}/learnopengl/root_directory.h "...")
file(WRITE ${CMAKE_BINARY_DIR}/root_directory.h "...")

file(DOWNLOAD
  https://raw.githubusercontent.com/JoeyDeVries/LearnOpenGL/master/includes/learnopengl/shader_m.h
  ${CMAKE_BINARY_DIR}/learnopengl/shader_m.h
)
```

**คำอธิบาย:**
- **file(MAKE_DIRECTORY)**: สร้าง directory สำหรับ headers
- **file(WRITE)**: สร้างไฟล์ root_directory.h ที่กำหนด root path
- **file(DOWNLOAD)**: ดาวน์โหลด utility headers จาก LearnOpenGL repository
- Headers ที่ดาวน์โหลด: shader_m.h, filesystem.h, model.h, animation.h, etc.

#### ส่วนที่ 2: Project Setup

```cmake
cmake_minimum_required(VERSION 3.16)
project(PlayableCharacter)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
```

**คำอธิบาย:**
- **cmake_minimum_required()**: กำหนด CMake version ต่ำสุดที่ต้องการ
- **project()**: กำหนดชื่อโปรเจค
- **set(CMAKE_CXX_STANDARD 17)**: ใช้ C++17 standard
- **CMAKE_CXX_STANDARD_REQUIRED ON**: บังคับใช้ C++17 (ไม่ fallback)

#### ส่วนที่ 3: Fetch External Libraries

**GLFW:**
```cmake
include(FetchContent)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG        3.3.8
)
FetchContent_MakeAvailable(glfw)
```

**คำอธิบาย:**
- **FetchContent**: CMake module สำหรับดาวน์โหลด dependencies
- **FetchContent_Declare()**: ประกาศว่าจะดาวน์โหลด GLFW version 3.3.8
- **FetchContent_MakeAvailable()**: ดาวน์โหลดและทำให้ใช้ได้

**GLM:**
```cmake
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG        0.9.9.8
)
if(POLICY CMP0169)
  cmake_policy(SET CMP0169 OLD)
endif()
FetchContent_GetProperties(glm)
if(NOT glm_POPULATED)
  FetchContent_Populate(glm)
  if(NOT TARGET glm)
    add_library(glm INTERFACE)
    target_include_directories(glm INTERFACE ${glm_SOURCE_DIR})
  endif()
endif()
```

**คำอธิบาย:**
- ดาวน์โหลด GLM version 0.9.9.8
- **CMP0169 policy**: ตั้งค่า policy สำหรับ compatibility
- สร้าง INTERFACE library ถ้ายังไม่มี target

**STB:**
```cmake
FetchContent_Declare(
  stb
  GIT_REPOSITORY https://github.com/nothings/stb.git
  GIT_TAG        master
)
FetchContent_MakeAvailable(stb)
```

**คำอธิบาย:**
- ดาวน์โหลด STB library (single-header library)
- ใช้ master branch (latest version)

**Assimp:**
```cmake
FetchContent_Declare(
  assimp
  GIT_REPOSITORY https://github.com/assimp/assimp.git
  GIT_TAG        v5.2.5
)
FetchContent_MakeAvailable(assimp)
if(TARGET assimp)
  target_compile_definitions(assimp PRIVATE -D_CRT_SECURE_NO_WARNINGS)
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    target_compile_options(assimp PRIVATE -Wno-error=deprecated-declarations)
  endif()
endif()
```

**คำอธิบาย:**
- ดาวน์โหลด Assimp version 5.2.5
- **target_compile_definitions()**: เพิ่ม compile definitions เพื่อ suppress warnings
- **target_compile_options()**: เพิ่ม compile options สำหรับ macOS/Clang

#### ส่วนที่ 4: GLAD Setup

```cmake
set(LEARNOPENGL_PATH "${CMAKE_SOURCE_DIR}/../LearnOpenGL")
if(EXISTS "${LEARNOPENGL_PATH}/src/glad.c")
    message(STATUS "Using GLAD from LearnOpenGL")
    set(GLAD_INCLUDE_DIR "${LEARNOPENGL_PATH}/includes")
    set(GLAD_SOURCE_FILE "${LEARNOPENGL_PATH}/src/glad.c")
else()
    message(FATAL_ERROR "GLAD files not found...")
endif()
```

**คำอธิบาย:**
- ตรวจสอบว่า GLAD files อยู่ใน LearnOpenGL directory หรือไม่
- ตั้งค่า paths สำหรับ GLAD include และ source files
- ถ้าไม่พบจะแสดง error และหยุด build

#### ส่วนที่ 5: Include Directories

```cmake
include_directories(
  ${GLAD_INCLUDE_DIR}
  ${glfw_SOURCE_DIR}/include
  ${glm_SOURCE_DIR}
  ${stb_SOURCE_DIR}
  ${assimp_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/src
)
```

**คำอธิบาย:**
- เพิ่ม include paths สำหรับ libraries ทั้งหมด
- ทำให้สามารถ `#include <glad/glad.h>` ได้โดยไม่ต้องระบุ full path

#### ส่วนที่ 6: Create Executable

```cmake
set(SRC src/main.cpp)
add_executable(PlayableCharacter ${SRC})

target_include_directories(PlayableCharacter PRIVATE
  ${CMAKE_BINARY_DIR}
  ${CMAKE_BINARY_DIR}/learnopengl
)

add_library(glad STATIC ${GLAD_SOURCE_FILE})
target_include_directories(glad PUBLIC ${GLAD_INCLUDE_DIR})

target_link_libraries(PlayableCharacter PRIVATE glfw glad assimp glm)
```

**คำอธิบาย:**
- **add_executable()**: สร้าง executable จาก main.cpp
- **target_include_directories()**: เพิ่ม include paths สำหรับ target
- **add_library()**: สร้าง static library จาก GLAD source
- **target_link_libraries()**: Link libraries เข้ากับ executable

**OpenGL Linking:**
```cmake
if (WIN32)
    target_link_libraries(PlayableCharacter PRIVATE opengl32)
else()
    find_package(OpenGL REQUIRED)
    target_link_libraries(PlayableCharacter PRIVATE OpenGL::GL)
endif()
```

**คำอธิบาย:**
- Windows: Link กับ opengl32
- Linux/macOS: ใช้ find_package เพื่อหา OpenGL และ link

#### ส่วนที่ 7: Copy Resources

```cmake
add_custom_command(TARGET PlayableCharacter POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:PlayableCharacter>/assets
  COMMENT "Copying assets to the target binary directory"
)

add_custom_command(TARGET PlayableCharacter POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:PlayableCharacter>/shaders
  COMMENT "Copying shaders to the target binary directory"
)
```

**คำอธิบาย:**
- **add_custom_command()**: กำหนด command ที่จะรันหลัง build
- **POST_BUILD**: รันหลังจาก build เสร็จ
- **copy_directory**: คัดลอก assets และ shaders ไปยัง build directory
- **$<TARGET_FILE_DIR:...>**: Path ของ directory ที่มี executable

**Install Resources:**
```cmake
install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets
        DESTINATION .
        USE_SOURCE_PERMISSIONS)
```

**คำอธิบาย:**
- กำหนดให้ install assets เมื่อรัน `cmake --install`

**Visual Studio Debugger Setup:**
```cmake
set_target_properties(PlayableCharacter PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    VS_DEBUGGER_ENVIRONMENT "LOGL_ROOT_PATH=${CMAKE_SOURCE_DIR}"
)
```

**คำอธิบาย:**
- ตั้งค่า working directory สำหรับ Visual Studio debugger
- ตั้งค่า environment variable สำหรับ FileSystem::getPath()

## 🔧 การปรับแต่ง

### ปรับขนาดแผนที่
แก้ไขใน `src/main.cpp`:
```cpp
const int MAP_SIZE = 15;  // เปลี่ยนเป็นค่าที่ต้องการ (เช่น 20 สำหรับ 20x20)
```

### ปรับขนาดบล็อก
แก้ไขใน `src/main.cpp`:
```cpp
float blockHeight = 0.2f;  // เปลี่ยนความสูงของบล็อกพื้น
const float fullBlockHeight = 1.0f;  // เปลี่ยนความสูงของบล็อกขอบ
```

### ปรับตำแหน่งกล้อง
แก้ไขใน `src/main.cpp`:
```cpp
glm::vec3 cameraPos(0.0f, MAP_SIZE * 0.9f, MAP_SIZE * 1.1f);  // เปลี่ยนค่าตรงนี้
glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);  // เปลี่ยนจุดที่กล้องมอง
```

### ปรับมุมมอง (FOV)
แก้ไขใน `src/main.cpp`:
```cpp
glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 200.0f);
// เปลี่ยน 45.0f เป็นค่าอื่น (เช่น 60.0f สำหรับมุมกว้างขึ้น)
```

### ปรับสีพื้นหลัง
แก้ไขใน `src/main.cpp`:
```cpp
glClearColor(0.1f, 0.1f, 0.15f, 1.0f);  // (R, G, B, Alpha)
```

### ปรับขนาดหน้าต่าง
แก้ไขใน `src/main.cpp`:
```cpp
const unsigned int SCR_WIDTH = 800;   // เปลี่ยนความกว้าง
const unsigned int SCR_HEIGHT = 600;  // เปลี่ยนความสูง
```

## 📄 License

โปรเจคนี้ใช้ไลบรารีภายนอกหลายตัวที่มี license ต่างกัน:
- GLFW: Zlib/libpng license
- GLM: MIT license
- Assimp: BSD 3-Clause license
- GLAD: Public domain
- STB: Public domain

## 👨‍💻 ผู้พัฒนา

โปรเจคนี้เป็นส่วนหนึ่งของ Game Engine Project

## 🙏 ขอบคุณ

- LearnOpenGL สำหรับ tutorial และ code examples
- Joey de Vries สำหรับ LearnOpenGL resources
