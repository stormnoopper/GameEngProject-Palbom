# Game Engine Project - PalBomb

โปรเจคเกม Bomber-Style Top-Down Map ที่ใช้ OpenGL สำหรับแสดงแผนที่แบบ 3D พร้อมระบบ texture mapping, lighting, และ skybox

## 📋 คำอธิบายโปรเจค

โปรเจคนี้เป็นเกม Bomber-Style Top-Down Map ที่แสดงแผนที่ขนาด 15x15 tiles โดยมี:
- **พื้น (Floor)**: พื้นที่เล่นเกมที่แสดงด้วย texture คอนกรีต
- **ขอบแผนที่ (Border Blocks)**: บล็อกที่ทำลายไม่ได้รอบๆ แผนที่ สูง 1.0 unit
- **บล็อกทำลายไม่ได้ (Red Blocks)**: บล็อกที่วางเป็นแพทเทิร์นในแผนที่ สูง 0.75 unit (75% ของขอบ)
- **บล็อกทำลายได้ (Breakable Blocks)**: บล็อกที่สร้างแบบสุ่มในพื้นที่ว่าง สูง 0.75 unit
- **Skybox**: พื้นหลัง 3D รอบๆ แผนที่
- **Lighting System**: ระบบแสงแบบ Phong (Ambient + Diffuse + Specular)

ใช้ระบบ Perspective Camera แบบ top-down view มุมมองจากด้านบน

## ✨ คุณสมบัติ

- **3D Tile Rendering**: แสดงแผนที่แบบ 3D ด้วย tiles พร้อม normal vectors
- **Texture Mapping**: ใช้ texture สำหรับพื้น, ขอบ, และบล็อกทำลายได้
- **Phong Lighting**: ระบบแสงแบบ Phong (Ambient + Diffuse + Specular) สำหรับความสมจริง
- **Skybox Rendering**: พื้นหลัง 3D ด้วย cubemap texture
- **Top-Down Camera**: กล้องมองจากด้านบนด้วย perspective projection
- **Border System**: แสดงขอบแผนที่ที่สูงกว่าพื้น (1.0 unit)
- **Pattern Blocks**: บล็อกทำลายไม่ได้ที่วางเป็นแพทเทิร์น (0.75 unit)
- **Random Breakable Blocks**: สร้างบล็อกทำลายได้แบบสุ่ม (60% probability)
- **Dynamic Regeneration**: กด R เพื่อสร้างบล็อกทำลายได้ใหม่
- **Dynamic Rendering**: เรนเดอร์ทุก frame ด้วย render loop

## 🎮 การควบคุม

- **ESC**: ปิดเกม
- **R**: สร้างบล็อกทำลายได้ใหม่แบบสุ่ม (Regenerate breakable blocks)

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
├── assets/                    # ไฟล์ texture และ resources
│   ├── Floor/                # Texture สำหรับพื้น (concrete)
│   ├── Unbreakable_Block/    # Texture สำหรับขอบและบล็อกทำลายไม่ได้
│   ├── Breakable_Block/      # Texture สำหรับบล็อกทำลายได้ (wood)
│   ├── Background/           # Skybox textures (6 faces: px, nx, py, ny, pz, nz)
│   └── Character/           # Character models และ textures (สำหรับอนาคต)
├── shaders/                  # Shader files
│   ├── tile.vs              # Vertex shader สำหรับ tiles (พร้อม lighting)
│   ├── tile.fs              # Fragment shader สำหรับ tiles (Phong lighting)
│   ├── skybox.vs            # Vertex shader สำหรับ skybox
│   └── skybox.fs            # Fragment shader สำหรับ skybox
├── src/                      # Source code
│   └── main.cpp             # Main game loop และ rendering logic
└── CMakeLists.txt           # CMake configuration
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
void processInput(GLFWwindow *window, std::vector<std::pair<int, int>>& breakableBlockPositions, 
                 std::mt19937& gen, const std::function<void(...)>& generateBlocks);
unsigned int loadCubemap(const std::vector<std::string>& faces);

const unsigned int SCR_WIDTH = 800;   // ความกว้างหน้าต่าง
const unsigned int SCR_HEIGHT = 600;   // ความสูงหน้าต่าง
```

**คำอธิบาย:**
- **framebuffer_size_callback**: Callback function ที่จะถูกเรียกเมื่อผู้ใช้เปลี่ยนขนาดหน้าต่าง
- **processInput**: ฟังก์ชันสำหรับตรวจสอบ keyboard input (รองรับ ESC และ R key)
  - รับ `breakableBlockPositions` เพื่อจัดการบล็อกทำลายได้
  - รับ `gen` (random generator) และ `generateBlocks` function สำหรับสร้างบล็อกใหม่
- **loadCubemap**: ฟังก์ชันสำหรับโหลด cubemap texture สำหรับ skybox
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
Shader skyboxShader("shaders/skybox.vs", "shaders/skybox.fs");
```

**คำอธิบาย:**
- **shader**: Shader program สำหรับ render tiles (พื้น, ขอบ, บล็อก) พร้อมระบบ lighting
- **skyboxShader**: Shader program สำหรับ render skybox (พื้นหลัง 3D)
- แต่ละ shader จะ compile vertex และ fragment shader จากไฟล์ที่ระบุ

#### ส่วนที่ 6: Vertex Data Setup

**Block Geometry:**
```cpp
float blockHeight = 0.2f;  // ความสูงของบล็อกพื้น (0.2 units)
float blockSize = 1.0f;    // ขนาดของบล็อก (1.0 unit)
```

**Vertex Array:**
```cpp
float blockHeight = 0.2f;  // ความสูงของบล็อกพื้น (0.2 units)
float blockSize = 1.0f;    // ขนาดของบล็อก (1.0 unit)

float vertices[] = {
    // positions (x, y, z)    // normals (nx, ny, nz)    // texture coords (u, v)
    // Top face (normal: 0, 1, 0)
     0.5f,  blockHeight,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
     0.5f,  blockHeight, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
    -0.5f,  blockHeight, -0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
    -0.5f,  blockHeight,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
    // Bottom, Front, Back, Right, Left faces...
};
```

**คำอธิบาย Vertex Data:**
- แต่ละ vertex มี **8 floats**: 
  - 3 สำหรับ position (x, y, z)
  - 3 สำหรับ normal vector (nx, ny, nz) - ใช้สำหรับคำนวณ lighting
  - 2 สำหรับ texture coordinates (u, v)
- Cube มี 6 faces: Top, Bottom, Front, Back, Right, Left
- แต่ละ face มี 4 vertices (quad) ซึ่งจะถูกวาดเป็น 2 triangles
- **Normal vectors**: จำเป็นสำหรับการคำนวณ diffuse และ specular lighting
  - Top face: (0, 1, 0) - ชี้ขึ้น
  - Bottom face: (0, -1, 0) - ชี้ลง
  - Front/Back/Right/Left: ชี้ตามทิศทางของ face

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
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

// Normal attribute (location = 1)
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
glEnableVertexAttribArray(1);

// Texture coordinate attribute (location = 2)
glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
glEnableVertexAttribArray(2);
```

**คำอธิบาย glVertexAttribPointer():**
- **0/1/2**: Attribute location (ต้องตรงกับ shader)
  - Location 0: Position (vec3)
  - Location 1: Normal (vec3) - สำหรับ lighting calculation
  - Location 2: Texture coordinates (vec2)
- **3/3/2**: จำนวน components (position=3, normal=3, texCoord=2)
- **GL_FLOAT**: ประเภทข้อมูล
- **GL_FALSE**: ไม่ normalize
- **8 * sizeof(float)**: Stride (ระยะห่างระหว่าง vertices) = 8 floats
- **offset**: ตำแหน่งเริ่มต้นของ attribute ใน vertex
  - Position: offset = 0 (เริ่มที่ float แรก)
  - Normal: offset = 3 * sizeof(float) (ข้าม position 3 floats)
  - TexCoord: offset = 6 * sizeof(float) (ข้าม position + normal = 6 floats)

#### ส่วนที่ 7: Texture Loading

```cpp
stbi_set_flip_vertically_on_load(true);  // พลิกรูปภาพตามแนวตั้ง
std::string texturePath = FileSystem::getPath("assets/Floor/concrete_wall_07_basecolor_1k.png");
unsigned int floorTexture = loadTexture(texturePath.c_str());
```

**คำอธิบาย:**
- **stbi_set_flip_vertically_on_load(true)**: พลิกรูปภาพเพราะ OpenGL coordinate system มี origin ที่มุมล่างซ้าย แต่รูปภาพมีที่มุมบนซ้าย
- **FileSystem::getPath()**: หา path ที่ถูกต้องของไฟล์ (รองรับทั้ง build directory และ source directory)
- **loadTexture()**: โหลด texture และได้ texture ID
- มี fallback mechanism ถ้าโหลดไม่สำเร็จจะลอง path โดยตรง

```cpp
// โหลด texture สำหรับขอบแผนที่ (Unbreakable blocks)
unsigned int borderTexture = loadTexture("assets/Unbreakable_Block/tudor_wall_01_basecolor_1k.png");

// โหลด texture สำหรับบล็อกทำลายได้ (Breakable blocks)
unsigned int breakableTexture = loadTexture("assets/Breakable_Block/wood_05_baseColor_1k.png");
```

**Set Shader Uniforms:**
```cpp
shader.use();
shader.setInt("texture1", 0);  // กำหนด texture unit 0 สำหรับ tile shader

skyboxShader.use();
skyboxShader.setInt("skybox", 0);  // กำหนด texture unit 0 สำหรับ skybox shader
```

**คำอธิบาย:**
- **shader.setInt("texture1", 0)**: กำหนด texture unit 0 ให้กับ uniform "texture1" ใน tile shader
- **skyboxShader.setInt("skybox", 0)**: กำหนด texture unit 0 ให้กับ uniform "skybox" ใน skybox shader
- Texture unit เป็นตัวระบุว่า shader จะใช้ texture ไหน (สามารถใช้หลาย texture พร้อมกันได้)

#### ส่วนที่ 8: Map Configuration และ Block Logic

```cpp
const int MAP_SIZE = 15;           // ขนาดแผนที่ 15x15 tiles
const float TILE_SIZE = 1.0f;      // ขนาดแต่ละ tile (1.0 unit)
const float MAP_OFFSET = -(MAP_SIZE - 1) * TILE_SIZE / 2.0f;  // จัดกึ่งกลางแผนที่
```

**คำอธิบาย MAP_OFFSET:**
- คำนวณเพื่อให้แผนที่อยู่กึ่งกลางที่ origin (0, 0, 0)
- สำหรับ 15 tiles: offset = -(15-1) * 1.0 / 2.0 = -7.0
- ดังนั้น tiles จะอยู่ที่ x = -7, -6, ..., 0, ..., 6, 7

**Block Type Logic:**

```cpp
// Helper function: ตรวจสอบว่าเป็น Red Block (บล็อกทำลายไม่ได้แบบแพทเทิร์น)
auto isRedBlock = [](int x, int z) -> bool {
    // Red blocks อยู่ที่ตำแหน่ง x และ z เป็นเลขคู่ (2, 4, 6, 8, 10, 12)
    return (x >= 2 && x <= 12 && x % 2 == 0 && 
            z >= 2 && z <= 12 && z % 2 == 0);
};

// Helper function: ตรวจสอบว่าเป็น Green Cell (จุด spawn ผู้เล่น)
auto isGreenCell = [](int x, int z) -> bool {
    // Green cells อยู่ที่ (1,1) และ (13,13)
    return (x == 1 && z == 1) || (x == 13 && z == 13);
};

// Helper function: ตรวจสอบว่าเป็น White Cell (ที่สามารถวางบล็อกทำลายได้)
auto isWhiteCell = [&](int x, int z) -> bool {
    // ไม่ใช่ขอบ
    if (x == 0 || x == MAP_SIZE - 1 || z == 0 || z == MAP_SIZE - 1) return false;
    // ไม่ใช่ Red Block
    if (isRedBlock(x, z)) return false;
    // ไม่ใช่ Green Cell
    if (isGreenCell(x, z)) return false;
    return true;
};
```

**คำอธิบาย Block Types:**
1. **Border Blocks**: ขอบแผนที่ (x=0, x=14, z=0, z=14) - ทำลายไม่ได้, สูง 1.0 unit
2. **Red Blocks**: บล็อกทำลายไม่ได้แบบแพทเทิร์น (x และ z เป็นเลขคู่ 2-12) - สูง 0.75 unit
3. **Green Cells**: จุด spawn ผู้เล่น (1,1) และ (13,13) - ไม่มีบล็อก
4. **White Cells**: พื้นที่ว่างที่สามารถวางบล็อกทำลายได้ - สร้างแบบสุ่ม

**Random Breakable Block Generation:**

```cpp
auto generateBreakableBlocks = [&](std::vector<std::pair<int, int>>& positions, std::mt19937& generator) {
    positions.clear();
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);
    const float BREAKABLE_BLOCK_PROBABILITY = 0.6f; // 60% โอกาส
    
    for (int x = 1; x < MAP_SIZE - 1; x++)
    {
        for (int z = 1; z < MAP_SIZE - 1; z++)
        {
            if (isWhiteCell(x, z) && dis(generator) < BREAKABLE_BLOCK_PROBABILITY)
            {
                positions.push_back({x, z});
            }
        }
    }
};
```

**คำอธิบาย:**
- ใช้ `std::mt19937` (Mersenne Twister) สำหรับ random number generation
- ตรวจสอบทุกตำแหน่งในแผนที่ (ยกเว้นขอบ)
- ถ้าเป็น White Cell และ random value < 0.6 (60%) จะสร้างบล็อกทำลายได้
- เก็บตำแหน่งไว้ใน `breakableBlockPositions` vector

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
glm::vec3 cameraPos(0.0f, MAP_SIZE * 1.2f, MAP_SIZE * 1.1f);
glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
```

**คำอธิบาย:**
- **cameraPos**: ตำแหน่งกล้อง (0, 18.0, 16.5) - อยู่ด้านบนและด้านหลัง
  - `MAP_SIZE * 1.2f` = 15 * 1.2 = 18.0 (ความสูง)
  - `MAP_SIZE * 1.1f` = 15 * 1.1 = 16.5 (ระยะห่างด้านหลัง)
- **cameraTarget**: จุดที่กล้องมอง (0, 0, 0) - กึ่งกลางแผนที่
- **cameraUp**: ทิศทางขึ้น (0, 1, 0) - แกน Y
- **glm::lookAt()**: สร้าง view matrix จากตำแหน่งกล้อง, จุดมอง, และทิศทางขึ้น
- มุมมองประมาณ 60° จากแนวนอน (top-down view)

**Lighting Setup:**
```cpp
glm::vec3 lightPos(MAP_SIZE * 0.5f, MAP_SIZE * 1.5f, MAP_SIZE * 0.5f);
glm::vec3 lightColor(1.0f, 1.0f, 0.95f); // สีขาวอุ่นนิดหน่อย
```

**คำอธิบาย:**
- **lightPos**: ตำแหน่งแสง (7.5, 22.5, 7.5) - อยู่เหนือแผนที่
- **lightColor**: สีแสง (1.0, 1.0, 0.95) - สีขาวอุ่น (warm white)
- แสงจะถูกใช้ใน fragment shader สำหรับคำนวณ Phong lighting

**Set Shader Uniforms:**
```cpp
shader.setMat4("projection", projection);
shader.setMat4("view", view);
shader.setVec3("lightPos", lightPos);
shader.setVec3("lightColor", lightColor);
shader.setVec3("viewPos", cameraPos);
```

**คำอธิบาย:**
- **projection**: Projection matrix สำหรับ perspective transformation
- **view**: View matrix สำหรับ camera transformation
- **lightPos**: ตำแหน่งแสง (ใช้คำนวณ diffuse และ specular)
- **lightColor**: สีแสง (ใช้คำนวณ ambient, diffuse, specular)
- **viewPos**: ตำแหน่งกล้อง (ใช้คำนวณ specular reflection)

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

**Rendering Red Blocks (Pattern Blocks):**
```cpp
const float redBlockHeight = fullBlockHeight * 0.75f;  // 75% ของขอบ (0.75 unit)
const float redBlockScaleY = redBlockHeight / blockHeight;  // 3.75

// แพทเทิร์น: บล็อกที่ x และ z เป็นเลขคู่ (2, 4, 6, 8, 10, 12)
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
```

**คำอธิบาย:**
- Red Blocks เป็นบล็อกทำลายไม่ได้ที่วางเป็นแพทเทิร์น
- สูง 0.75 unit (75% ของขอบ)
- วางที่ตำแหน่ง x และ z เป็นเลขคู่ (2, 4, 6, 8, 10, 12)
- ใช้ texture เดียวกับขอบ (borderTexture)

**Rendering Breakable Blocks:**
```cpp
const float breakableBlockHeight = fullBlockHeight * 0.75f;  // 0.75 unit
const float breakableBlockScaleY = breakableBlockHeight / blockHeight;  // 3.75
glBindTexture(GL_TEXTURE_2D, breakableTexture);

for (const auto& pos : breakableBlockPositions)
{
    int x = pos.first;
    int z = pos.second;
    
    float tileX = MAP_OFFSET + x * TILE_SIZE;
    float tileZ = MAP_OFFSET + z * TILE_SIZE;
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(tileX, blockHeight, tileZ));
    model = glm::scale(model, glm::vec3(1.0f, breakableBlockScaleY, 1.0f));
    shader.setMat4("model", model);
    
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
```

**คำอธิบาย:**
- Breakable Blocks สร้างแบบสุ่มใน White Cells
- สูง 0.75 unit (เท่ากับ Red Blocks)
- ใช้ texture ไม้ (wood texture)
- Loop ผ่าน `breakableBlockPositions` vector ที่สร้างไว้ตอนเริ่มต้น

**Skybox Rendering:**
```cpp
// วาด skybox สุดท้าย (หลังทุกอย่าง)
glDepthFunc(GL_LEQUAL);  // เปลี่ยน depth function เป็น <= แทน <
skyboxShader.use();

// ลบ translation ออกจาก view matrix (skybox ต้องอยู่ที่ origin เสมอ)
glm::mat4 viewNoTranslation = glm::mat4(glm::mat3(view));
skyboxShader.setMat4("view", viewNoTranslation);
skyboxShader.setMat4("projection", projection);

glBindVertexArray(skyboxVAO);
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
glDrawArrays(GL_TRIANGLES, 0, 36);  // Skybox มี 36 vertices (6 faces × 6 vertices)
glBindVertexArray(0);

glDepthFunc(GL_LESS);  // เปลี่ยนกลับเป็น depth function ปกติ
```

**คำอธิบาย:**
- **glDepthFunc(GL_LEQUAL)**: เปลี่ยน depth test เป็น <= เพื่อให้ skybox ผ่าน depth test เสมอ (เพราะอยู่ไกลสุด)
- **viewNoTranslation**: ลบ translation ออกจาก view matrix (ใช้แค่ rotation) เพื่อให้ skybox หมุนตามกล้องแต่ไม่เคลื่อนที่
- **GL_TEXTURE_CUBE_MAP**: ใช้ cubemap texture (6 faces) แทน 2D texture
- **glDrawArrays()**: วาด skybox โดยตรง (ไม่ใช้ indices)

**Buffer Swap:**
```cpp
glfwSwapBuffers(window);  // สลับ front และ back buffer (double buffering)
glfwPollEvents();         // ตรวจสอบ events (keyboard, mouse, window)
```
- **Double Buffering**: วาดที่ back buffer แล้วสลับมาแสดงที่หน้าจอ เพื่อป้องกัน flickering
- **Poll Events**: อัปเดต input และ window events

#### ส่วนที่ 10: Skybox Setup

```cpp
// Skybox vertices (cube ที่ใหญ่ที่สุด)
float skyboxVertices[] = {
    // positions (6 faces, แต่ละ face มี 2 triangles = 6 vertices)
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    // ... (ทั้งหมด 36 vertices)
};

// สร้าง VAO/VBO สำหรับ skybox
unsigned int skyboxVAO, skyboxVBO;
glGenVertexArrays(1, &skyboxVAO);
glGenBuffers(1, &skyboxVBO);
glBindVertexArray(skyboxVAO);
glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
glEnableVertexAttribArray(0);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

// โหลด cubemap texture
std::vector<std::string> skyboxFaces = {
    FileSystem::getPath("assets/Background/px.png"),  // Positive X
    FileSystem::getPath("assets/Background/nx.png"),  // Negative X
    FileSystem::getPath("assets/Background/py.png"),  // Positive Y
    FileSystem::getPath("assets/Background/ny.png"),  // Negative Y
    FileSystem::getPath("assets/Background/pz.png"),  // Positive Z
    FileSystem::getPath("assets/Background/nz.png")   // Negative Z
};
stbi_set_flip_vertically_on_load(false);  // ไม่ต้องพลิกสำหรับ cubemap
unsigned int cubemapTexture = loadCubemap(skyboxFaces);
stbi_set_flip_vertically_on_load(true);   // เปลี่ยนกลับ
```

**คำอธิบาย:**
- **Skybox vertices**: Cube ที่มีขนาด 1.0 unit (จะถูก scale ด้วย projection)
- **Cubemap**: Texture ที่มี 6 faces (แต่ละ face เป็นรูปภาพ 2D)
- **loadCubemap()**: โหลด 6 รูปภาพและสร้าง cubemap texture
- **GL_TEXTURE_CUBE_MAP**: ประเภท texture พิเศษสำหรับ cubemap

**loadCubemap() Function:**

```cpp
unsigned int loadCubemap(const std::vector<std::string>& faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    
    // โหลดแต่ละ face
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            GLenum format = nrChannels == 4 ? GL_RGBA : GL_RGB;
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
    }
    
    // ตั้งค่า texture parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    return textureID;
}
```

**คำอธิบาย:**
- **GL_TEXTURE_CUBE_MAP_POSITIVE_X + i**: แต่ละ face มี target แยกกัน (POSITIVE_X, NEGATIVE_X, POSITIVE_Y, ...)
- **GL_CLAMP_TO_EDGE**: ใช้ edge clamping เพื่อป้องกัน seams ระหว่าง faces
- **GL_LINEAR**: ใช้ linear filtering สำหรับ smooth texture

#### ส่วนที่ 11: Cleanup

```cpp
glDeleteVertexArrays(1, &VAO);
glDeleteBuffers(1, &VBO);
glDeleteBuffers(1, &EBO);
glDeleteVertexArrays(1, &skyboxVAO);
glDeleteBuffers(1, &skyboxVBO);
glfwTerminate();
return 0;
```

**คำอธิบาย:**
- **glDeleteVertexArrays()**: ลบ VAO ทั้ง tile และ skybox
- **glDeleteBuffers()**: ลบ VBO และ EBO ทั้งหมด
- **glfwTerminate()**: ปิด GLFW และล้าง resources ทั้งหมด

#### ส่วนที่ 12: Helper Functions

**processInput():**
```cpp
void processInput(GLFWwindow *window, std::vector<std::pair<int, int>>& breakableBlockPositions, 
                 std::mt19937& gen, const std::function<void(...)>& generateBlocks)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    // R key เพื่อสร้างบล็อกทำลายได้ใหม่
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
```

**คำอธิบาย:**
- **glfwGetKey()**: ตรวจสอบว่าปุ่มถูกกดหรือไม่
- **GLFW_KEY_ESCAPE**: ปุ่ม ESC - ปิดเกม
- **GLFW_KEY_R**: ปุ่ม R - สร้างบล็อกทำลายได้ใหม่
- **rKeyPressed flag**: ใช้ป้องกันการกดค้าง (key repeat) - จะทำงานแค่ครั้งเดียวเมื่อกด
- **generateBlocks()**: เรียกฟังก์ชันเพื่อสร้างบล็อกทำลายได้ใหม่แบบสุ่ม
- **GLFW_PRESS/GLFW_RELEASE**: ตรวจสอบสถานะการกดปุ่ม
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

#### tile.vs และ tile.fs (Tile Shaders)

ใช้สำหรับ render tiles ทั้งหมด (พื้น, ขอบ, บล็อก) พร้อมระบบ Phong lighting

#### tile.vs (Vertex Shader)

```glsl
#version 330 core
layout (location = 0) in vec3 aPos;        // รับ position จาก CPU
layout (location = 1) in vec3 aNormal;     // รับ normal vector จาก CPU
layout (location = 2) in vec2 aTexCoord;   // รับ texture coordinates จาก CPU

out vec3 FragPos;      // ส่ง world position ไปยัง fragment shader
out vec3 Normal;       // ส่ง normal vector ไปยัง fragment shader
out vec2 TexCoord;     // ส่ง texture coordinates ไปยัง fragment shader

uniform mat4 model;       // Model matrix (transform object)
uniform mat4 view;        // View matrix (camera)
uniform mat4 projection;  // Projection matrix (perspective)

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));  // Transform position ไปยัง world space
    Normal = mat3(transpose(inverse(model))) * aNormal;  // Transform normal ไปยัง world space
    TexCoord = aTexCoord;
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
```

**คำอธิบายทีละบรรทัด:**

1. **#version 330 core**: ใช้ GLSL version 3.30 core profile
2. **layout (location = 0) in vec3 aPos**: 
   - รับ vertex position (3 floats: x, y, z)
   - location = 0 ต้องตรงกับ `glVertexAttribPointer(0, ...)`
3. **layout (location = 1) in vec3 aNormal**: 
   - รับ normal vector (3 floats: nx, ny, nz)
   - location = 1 ต้องตรงกับ `glVertexAttribPointer(1, ...)`
   - ใช้สำหรับคำนวณ lighting
4. **layout (location = 2) in vec2 aTexCoord**: 
   - รับ texture coordinates (2 floats: u, v)
   - location = 2 ต้องตรงกับ `glVertexAttribPointer(2, ...)`
5. **out vec3 FragPos**: ส่ง world position ไปยัง fragment shader (ใช้คำนวณ lighting)
6. **out vec3 Normal**: ส่ง normal vector ไปยัง fragment shader (ใช้คำนวณ lighting)
7. **out vec2 TexCoord**: ส่ง texture coordinates ไปยัง fragment shader
8. **uniform mat4**: รับ matrices จาก CPU (ไม่เปลี่ยนแปลงระหว่าง vertices)
9. **FragPos calculation**: Transform position จาก local space → world space
10. **Normal transformation**: 
    - `transpose(inverse(model))` = Normal Matrix
    - Transform normal vector ไปยัง world space โดยรักษา orientation
    - ใช้ inverse transpose เพื่อให้ normal ถูกต้องแม้มีการ scale ที่ไม่เท่ากัน
11. **MVP Transformation**: 
    - `projection × view × FragPos`
    - Transform จาก world space → view space → clip space
12. **gl_Position**: Output position ใน clip space (ต้องเป็น vec4)

**MVP Transformation Pipeline:**
- **Model Matrix**: Transform จาก local space (object) → world space
- **View Matrix**: Transform จาก world space → view space (camera space)
- **Projection Matrix**: Transform จาก view space → clip space (perspective)

**Normal Matrix Explanation:**
- เมื่อมีการ scale ที่ไม่เท่ากัน (non-uniform scaling) normal vector จะผิดเพี้ยน
- ใช้ `transpose(inverse(model))` เพื่อแก้ไข normal ให้ถูกต้อง
- ถ้าใช้แค่ `model` จะทำให้ lighting คำนวณผิด

#### tile.fs (Fragment Shader)

```glsl
#version 330 core
out vec4 FragColor;           // สี output ของ pixel

in vec3 FragPos;              // รับ world position จาก vertex shader
in vec3 Normal;               // รับ normal vector จาก vertex shader
in vec2 TexCoord;             // รับ texture coordinates จาก vertex shader

uniform sampler2D texture1;   // Texture sampler
uniform vec3 lightPos;        // ตำแหน่งแสง
uniform vec3 lightColor;      // สีแสง
uniform vec3 viewPos;         // ตำแหน่งกล้อง

void main()
{
    // Ambient lighting (แสงพื้นฐาน)
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;
    
    // Diffuse lighting (แสงสะท้อนแบบกระจาย)
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular lighting (แสงสะท้อนแบบเงา)
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;
    
    // รวมแสงทั้งหมดกับ texture color
    vec3 objectColor = texture(texture1, TexCoord).rgb;
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
```

**คำอธิบายทีละส่วน:**

1. **#version 330 core**: ใช้ GLSL version 3.30 core profile
2. **out vec4 FragColor**: สี output ของ pixel (R, G, B, Alpha)
3. **in vec3 FragPos**: รับ world position จาก vertex shader (interpolated)
4. **in vec3 Normal**: รับ normal vector จาก vertex shader (interpolated)
5. **in vec2 TexCoord**: รับ texture coordinates จาก vertex shader (interpolated)
6. **uniform sampler2D texture1**: Texture sampler สำหรับอ่านสีจาก texture
7. **uniform vec3 lightPos/viewPos/lightColor**: ข้อมูลแสงและกล้องจาก CPU

**Phong Lighting Model:**

**1. Ambient Lighting (แสงพื้นฐาน):**
```glsl
float ambientStrength = 0.3;
vec3 ambient = ambientStrength * lightColor;
```
- แสงพื้นฐานที่ส่องทุกที่เท่ากัน (ไม่ขึ้นกับตำแหน่ง)
- `ambientStrength = 0.3` = 30% ของแสง
- ทำให้ส่วนที่ไม่มีแสงโดยตรงไม่มืดสนิท

**2. Diffuse Lighting (แสงสะท้อนแบบกระจาย):**
```glsl
vec3 norm = normalize(Normal);
vec3 lightDir = normalize(lightPos - FragPos);
float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse = diff * lightColor;
```
- แสงสะท้อนตามมุมที่แสงตกกระทบ
- `dot(norm, lightDir)`: คำนวณมุมระหว่าง normal กับทิศทางแสง
- `max(..., 0.0)`: ป้องกันค่าลบ (ด้านที่หันออกจากแสงจะมืด)
- ยิ่งหันเข้าหาแสงมาก ยิ่งสว่างมาก

**3. Specular Lighting (แสงสะท้อนแบบเงา):**
```glsl
vec3 viewDir = normalize(viewPos - FragPos);
vec3 reflectDir = reflect(-lightDir, norm);
float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
vec3 specular = specularStrength * spec * lightColor;
```
- แสงสะท้อนที่เห็นเป็นเงา (highlight)
- `reflect()`: คำนวณทิศทางแสงสะท้อน
- `pow(..., 32)`: ทำให้ highlight แคบและคมชัด (shininess = 32)
- ยิ่งมองใกล้ทิศทางสะท้อน ยิ่งเห็นเงา

**4. Combine Lighting:**
```glsl
vec3 objectColor = texture(texture1, TexCoord).rgb;
vec3 result = (ambient + diffuse + specular) * objectColor;
```
- อ่านสีจาก texture
- รวม ambient + diffuse + specular
- คูณกับสี texture เพื่อให้ได้สีสุดท้าย

**Fragment Shader Process:**
- Vertex shader ส่ง FragPos, Normal, TexCoord ไปยัง fragment shader
- GPU จะ interpolate ค่าเหล่านี้สำหรับแต่ละ pixel
- Fragment shader คำนวณ Phong lighting และอ่านสีจาก texture
- ส่งสีสุดท้ายไปยัง framebuffer

#### skybox.vs และ skybox.fs (Skybox Shaders)

ใช้สำหรับ render skybox (พื้นหลัง 3D)

#### skybox.vs (Skybox Vertex Shader)

```glsl
#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;  // ส่ง texture coordinates (ใช้เป็น direction vector)

uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aPos;  // ใช้ position เป็น direction vector สำหรับ cubemap
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;  // ตั้ง z = w เพื่อให้ผ่าน depth test เสมอ
}
```

**คำอธิบาย:**
- **TexCoords = aPos**: ใช้ vertex position เป็น direction vector สำหรับ cubemap lookup
- **pos.xyww**: ตั้ง z = w เพื่อให้ z/w = 1.0 (ไกลสุด) ทำให้ผ่าน depth test เสมอ
- Skybox จะอยู่หลังทุกอย่างเสมอ

#### skybox.fs (Skybox Fragment Shader)

```glsl
#version 330 core
out vec4 FragColor;

in vec3 TexCoords;  // รับ direction vector จาก vertex shader

uniform samplerCube skybox;  // Cubemap sampler

void main()
{
    FragColor = texture(skybox, vec3(TexCoords.x, -TexCoords.y, TexCoords.z));
}
```

**คำอธิบาย:**
- **samplerCube**: ใช้ cubemap texture แทน 2D texture
- **texture(skybox, ...)**: อ่านสีจาก cubemap ตาม direction vector
- **-TexCoords.y**: พลิกแกน Y เพื่อให้ orientation ถูกต้อง
- Cubemap มี 6 faces: px, nx, py, ny, pz, nz (positive/negative X/Y/Z)

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
glm::vec3 cameraPos(0.0f, MAP_SIZE * 1.2f, MAP_SIZE * 1.1f);  // เปลี่ยนค่าตรงนี้
glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);  // เปลี่ยนจุดที่กล้องมอง
```

### ปรับระบบแสง
แก้ไขใน `src/main.cpp`:
```cpp
glm::vec3 lightPos(MAP_SIZE * 0.5f, MAP_SIZE * 1.5f, MAP_SIZE * 0.5f);  // ตำแหน่งแสง
glm::vec3 lightColor(1.0f, 1.0f, 0.95f);  // สีแสง (R, G, B)
```

แก้ไขใน `shaders/tile.fs`:
```glsl
float ambientStrength = 0.3;      // ปรับความสว่างพื้นฐาน (0.0-1.0)
float specularStrength = 0.5;      // ปรับความสว่างของเงา (0.0-1.0)
float spec = pow(..., 32);         // ปรับความคมของเงา (เลขยิ่งมากยิ่งคม)
```

### ปรับความน่าจะเป็นของบล็อกทำลายได้
แก้ไขใน `src/main.cpp`:
```cpp
const float BREAKABLE_BLOCK_PROBABILITY = 0.6f;  // เปลี่ยนเป็น 0.0-1.0 (0.6 = 60%)
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
