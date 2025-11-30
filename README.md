# Game Engine Project - PalBomb

โปรเจคเกม Bomber-Style Top-Down Map ที่ใช้ OpenGL สำหรับแสดงแผนที่แบบ 3D พร้อมระบบ texture mapping, lighting, และ skybox

## 📋 คำอธิบายโปรเจค

โปรเจคนี้เป็นเกม Bomber-Style Top-Down Map ที่แสดงแผนที่ขนาด 15x15 tiles โดยมี:
- **พื้น (Floor)**: พื้นที่เล่นเกมที่แสดงด้วย texture คอนกรีต
- **ขอบแผนที่ (Border Blocks)**: บล็อกที่ทำลายไม่ได้รอบๆ แผนที่ สูง 1.0 unit
- **บล็อกทำลายไม่ได้ (Red Blocks)**: บล็อกที่วางเป็นแพทเทิร์นในแผนที่ สูง 0.75 unit (75% ของขอบ)
- **บล็อกทำลายได้ (Breakable Blocks)**: บล็อกที่สร้างแบบสุ่มในพื้นที่ว่าง สูง 0.75 unit
- **Skybox**: พื้นหลัง 3D รอบๆ แผนที่พร้อมการหมุนอัตโนมัติ
- **Lighting System**: ระบบแสงแบบ Phong (Ambient + Diffuse + Specular)
- **Character System**: ระบบตัวละคร 2 ตัว (P1 และ P2) พร้อมแอนิเมชัน
- **Animation System**: ระบบแอนิเมชันสำหรับตัวละคร (Idle และ Walk)
- **Grid-Based Movement**: ระบบการเคลื่อนที่แบบ grid-based พร้อม collision detection
- **Bomb System**: ระบบระเบิดที่สามารถวางและระเบิดทำลายบล็อกทำลายได้ พร้อม explosion preview
- **Power-Up System**: ระบบ Power-Up 5 ประเภท (Range Boost, Bomb Capacity, Shield, Speed Boost, Heart)
- **Health System**: ระบบ Health 3 จุดพร้อม invulnerability frames
- **Game Over System**: ระบบจบเกมเมื่อผู้เล่นคนใดคนหนึ่ง Health = 0
- **UI System**: ระบบ UI แสดง Health, Game Over screen, Settings menu
- **Audio System**: ระบบเสียงเพลงพื้นหลัง, เสียงระเบิด, และเสียงเก็บ Power-Up

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
- **Character System**: ระบบตัวละคร 2 ตัว (P1 และ P2) พร้อม texture mapping
- **Skeletal Animation**: ระบบแอนิเมชันแบบ skeletal animation ด้วย Assimp
- **Character Animations**: แอนิเมชัน Idle และ Walk สำหรับตัวละคร
- **Grid-Based Movement**: ระบบการเคลื่อนที่แบบ grid-based พร้อม smooth interpolation
- **Character Collision**: ระบบ collision detection ระหว่างตัวละครและสิ่งกีดขวาง
- **Multi-Player Controls**: รองรับการควบคุม 2 ผู้เล่นพร้อมกัน (WASD และ Arrow keys)
- **MSAA Anti-Aliasing**: ระบบ anti-aliasing แบบ 4x MSAA สำหรับภาพที่เรียบขึ้น
- **Bomb Placement**: วางระเบิดได้ (Q สำหรับ P1, M สำหรับ P2) พร้อมระบบจำกัดจำนวนระเบิด
- **Bomb Explosion**: ระเบิดทำลายบล็อกทำลายได้ในรูปแบบกากบาท (2 ช่องในแต่ละทิศทาง)
- **Bomb Timer**: ระเบิดจะทำงานหลังจาก 3 วินาที
- **Bomb Collision**: ตัวละครไม่สามารถเดินผ่านระเบิดได้
- **Explosion Preview**: แสดง preview บริเวณที่ระเบิดจะระเบิด (pulsing animation)
- **Power-Up System**: ระบบ Power-Up 5 ประเภทที่ดรอปจากบล็อกทำลายได้
- **Health System**: ผู้เล่นมี Health 3 จุด และแพ้เมื่อ Health = 0
- **Invulnerability Frames**: ผู้เล่นปลอดภัยจากการโดนระเบิดซ้ำเป็นเวลา 1 วินาที
- **Game Over Screen**: แสดงหน้าจอจบเกมพร้อมปุ่ม Restart และ Quit
- **Intro Screen**: หน้าจอเริ่มต้นเกมพร้อมปุ่ม Start และ Settings
- **Settings Menu**: เมนูตั้งค่าเสียงและปิดเกม
- **Animated Skybox**: Skybox หมุนอัตโนมัติช้าๆ (3 องศาต่อวินาที)
- **Background Music**: เพลงพื้นหลังที่เล่นวนซ้ำอัตโนมัติ (รองรับ macOS)

## 🎮 การควบคุม

### การควบคุมทั่วไป
- **ESC**: ปิดเกม
- **R**: สร้างบล็อกทำลายได้ใหม่แบบสุ่ม (Regenerate breakable blocks) - ใช้ได้ระหว่างเล่นเกม
- **Mouse Click**: คลิกปุ่มต่างๆ บน UI (Start, Settings, Restart, Quit, Close)

### การควบคุมตัวละคร
- **Player 1 (P1 - ซ้ายบน)**:
  - **W**: เดินขึ้น (Up)
  - **S**: เดินลง (Down)
  - **A**: เดินซ้าย (Left)
  - **D**: เดินขวา (Right)
  - **Q**: วางระเบิด (Place Bomb)

- **Player 2 (P2 - ขวาล่าง)**:
  - **↑ (Arrow Up)**: เดินขึ้น (Up)
  - **↓ (Arrow Down)**: เดินลง (Down)
  - **← (Arrow Left)**: เดินซ้าย (Left)
  - **→ (Arrow Right)**: เดินขวา (Right)
  - **M**: วางระเบิด (Place Bomb)

**หมายเหตุ**: 
- ตัวละครจะเคลื่อนที่แบบ grid-based และไม่สามารถเคลื่อนที่ผ่านบล็อก, ขอบแผนที่, ตัวละครอื่น, หรือระเบิดได้
- ระเบิดจะระเบิดหลังจาก 3 วินาที และทำลายบล็อกทำลายได้ในรูปแบบกากบาท (2 ช่องในแต่ละทิศทาง)
- ไม่สามารถวางระเบิดซ้ำที่ตำแหน่งเดิมได้
- ผู้เล่นแต่ละคนมีจำนวนระเบิดสูงสุดที่วางได้พร้อมกัน (เริ่มต้น: 3 ลูก)
- Power-Up ถูกเก็บอัตโนมัติเมื่อเดินเข้าไปที่ตำแหน่งเดียวกัน
- ผู้เล่นมี Health 3 จุด และแพ้เมื่อ Health = 0

## 🛠️ เทคโนโลยีที่ใช้

- **OpenGL 3.3**: สำหรับการเรนเดอร์กราฟิก 3D
- **GLFW**: สำหรับการจัดการหน้าต่างและ input
- **GLAD**: สำหรับ OpenGL function loader
- **GLM**: สำหรับการคำนวณทางคณิตศาสตร์ (vectors, matrices)
- **STB Image**: สำหรับโหลดไฟล์ texture (PNG)
- **Assimp**: สำหรับโหลดโมเดล 3D และแอนิเมชัน (.dae, .glb files)
- **AVFoundation** (macOS): สำหรับเล่นเสียงเพลงพื้นหลัง
- **CMake**: สำหรับ build system

## 📁 โครงสร้างโปรเจค

```
GameEngProject-Palbom/
├── assets/                    # ไฟล์ texture และ resources
│   ├── Floor/                # Texture สำหรับพื้น (concrete)
│   ├── Unbreakable_Block/    # Texture สำหรับขอบและบล็อกทำลายไม่ได้
│   ├── Breakable_Block/      # Texture สำหรับบล็อกทำลายได้ (wood)
│   ├── Background/           # Skybox textures (6 faces: px, nx, py, ny, pz, nz)
│   ├── item/                 # Item models
│   │   └── bomb.glb          # Bomb model
│   ├── sound/                # Audio files
│   │   └── background.mp3    # Background music
│   └── Character/           # Character models, animations และ textures
│       ├── Movement/        # Animation files (.dae)
│       │   ├── Idle.dae     # Idle animation
│       │   └── walk.dae     # Walk animation
│       ├── P1/              # Textures สำหรับ Player 1
│       │   ├── Ch32_1001_Diffuse.png
│       │   ├── Ch32_1001_Specular.png
│       │   ├── Ch32_1001_Normal.png
│       │   └── Ch32_1001_Glossiness.png
│       └── P2/              # Textures สำหรับ Player 2
│           ├── Ch32_1001_Diffuse.png
│           ├── Ch32_1001_Specular.png
│           ├── Ch32_1001_Normal.png
│           └── Ch32_1001_Glossiness.png
├── shaders/                  # Shader files
│   ├── tile.vs              # Vertex shader สำหรับ tiles (พร้อม lighting)
│   ├── tile.fs              # Fragment shader สำหรับ tiles (Phong lighting)
│   ├── skybox.vs            # Vertex shader สำหรับ skybox
│   ├── skybox.fs            # Fragment shader สำหรับ skybox
│   ├── anim_model.vs        # Vertex shader สำหรับตัวละคร (skeletal animation)
│   └── anim_model.fs        # Fragment shader สำหรับตัวละคร
├── src/                      # Source code
│   ├── main.cpp             # Main game loop, rendering logic, character system, และ bomb system
│   ├── audio_player.h       # Audio player header (macOS)
│   └── audio_player.mm     # Audio player implementation (macOS)
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
./RunGame
```

### สำหรับ Windows

```bash
cmake -S . -B build
cmake --build build --config Release
cd build/Release
RunGame.exe
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

const unsigned int SCR_WIDTH = 1280;   // ความกว้างหน้าต่าง (HD)
const unsigned int SCR_HEIGHT = 720;   // ความสูงหน้าต่าง (HD)
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
glfwWindowHint(GLFW_SAMPLES, 4);  // Enable 4x MSAA for anti-aliasing
```
- **glfwInit()**: เริ่มต้น GLFW library
- **glfwWindowHint()**: กำหนด OpenGL version เป็น 3.3 และใช้ Core Profile
- **GLFW_SAMPLES**: เปิดใช้งาน 4x MSAA สำหรับ anti-aliasing

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
- สร้างหน้าต่างขนาด 1280x720 (HD)
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
glEnable(GL_MULTISAMPLE);  // Enable MSAA
```
- **GL_DEPTH_TEST**: เปิดใช้งาน depth testing เพื่อให้เรนเดอร์ 3D objects ถูกต้อง (objects ที่อยู่ใกล้จะบัง objects ที่อยู่ไกล)
- **GL_MULTISAMPLE**: เปิดใช้งาน 4x MSAA (Multi-Sample Anti-Aliasing) เพื่อให้ขอบวัตถุเรียบขึ้น

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
    // Green cells เป็น 2x2 clusters:
    // Top-left cluster: (1,1), (1,2), (2,1), (2,2)
    // Bottom-right cluster: (12,12), (12,13), (13,12), (13,13)
    if ((x == 1 || x == 2) && (z == 1 || z == 2))
        return true;
    if ((x == 12 || x == 13) && (z == 12 || z == 13))
        return true;
    return false;
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
3. **Green Cells**: จุด spawn ผู้เล่น - เป็น 2x2 clusters:
   - Top-left cluster: (1,1), (1,2), (2,1), (2,2) - สำหรับ P1
   - Bottom-right cluster: (12,12), (12,13), (13,12), (13,13) - สำหรับ P2
   - ไม่มีบล็อกทำลายได้ในพื้นที่นี้
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
float deltaTime = 0.0f;
float lastFrame = glfwGetTime();

while (!glfwWindowShouldClose(window))
{
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    
    // Cap deltaTime to avoid huge jumps
    if (deltaTime > 0.1f) deltaTime = 0.1f;
    
    // ... rendering code ...
}
```
Loop หลักที่ทำงานจนกว่าผู้ใช้จะปิดหน้าต่าง

**Delta Time Calculation:**
- คำนวณ deltaTime เพื่อให้การเคลื่อนที่และแอนิเมชันสม่ำเสมอไม่ขึ้นกับ frame rate
- Cap deltaTime ที่ 0.1 วินาทีเพื่อป้องกันการกระโดดใหญ่เมื่อเกิด lag

**Input Processing:**
```cpp
processInput(window, breakableBlockPositions, gen, generateBreakableBlocks, leftPose, rightPose);
```
ตรวจสอบ keyboard input (ESC, R, WASD, Arrow keys) และอัปเดตการเคลื่อนที่ของตัวละคร

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

#### ส่วนที่ 10: Character System Setup

**Character Structures:**
```cpp
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
};

struct CharacterTextures
{
    unsigned int diffuse  = 0;
    unsigned int specular = 0;
    unsigned int normal   = 0;
    unsigned int gloss    = 0;
};
```

**คำอธิบาย:**
- **CharacterPose**: เก็บสถานะของตัวละคร (ตำแหน่ง grid, world position, rotation, animation state)
- **CharacterTextures**: เก็บ texture IDs สำหรับตัวละคร (diffuse, specular, normal, gloss)
- **Grid-Based Movement**: ตัวละครเคลื่อนที่แบบ grid-based (1 tile ต่อครั้ง)
- **Smooth Interpolation**: ใช้ `moveProgress` เพื่อ interpolate การเคลื่อนที่ให้ลื่นไหล

**Model และ Animation Loading:**
```cpp
const std::string idleAnimationPath = FileSystem::getPath("assets/Character/Movement/Idle.dae");
const std::string walkAnimationPath = FileSystem::getPath("assets/Character/Movement/walk.dae");

Model characterModelP1(idleAnimationPath);
Model characterModelP2(idleAnimationPath);

Animation idleAnimation(idleAnimationPath, &characterModelP1);
Animation walkAnimation(walkAnimationPath, &characterModelP1);

Animator animatorP1(&idleAnimation);
Animator animatorP2(&idleAnimation);
```

**คำอธิบาย:**
- โหลดโมเดลตัวละครจากไฟล์ `.dae` (Collada format) ด้วย Assimp
- โหลดแอนิเมชัน Idle และ Walk
- สร้าง Animator สำหรับแต่ละตัวละครเพื่อจัดการแอนิเมชัน

**Character Texture Loading:**
```cpp
const CharacterTextures p1Textures = LoadCharacterTextures(p1TextureDir);
const CharacterTextures p2Textures = LoadCharacterTextures(p2TextureDir);
ApplyTexturesToModel(characterModelP1, p1Textures, "P1");
ApplyTexturesToModel(characterModelP2, p2Textures, "P2");
```

**คำอธิบาย:**
- โหลด texture สำหรับแต่ละตัวละคร (P1 และ P2 มี texture set แยกกัน)
- Apply texture ไปยังโมเดลเพื่อให้แต่ละตัวละครมีสี/ลวดลายต่างกัน

**Character Initialization:**
```cpp
// P1 at (1,1) - Top-Left
CharacterPose leftPose;
leftPose.gridX = 1;
leftPose.gridY = 1;
leftPose.position = GridToWorld(leftPose.gridX, leftPose.gridY);
leftPose.position.y = BLOCK_HEIGHT + 0.3f;
leftPose.rotation = 0.0f;

// P2 at (13,13) - Bottom-Right
CharacterPose rightPose;
rightPose.gridX = 13;
rightPose.gridY = 13;
rightPose.position = GridToWorld(rightPose.gridX, rightPose.gridY);
rightPose.position.y = BLOCK_HEIGHT + 0.3f;
rightPose.rotation = glm::radians(180.0f);
```

**คำอธิบาย:**
- P1 เริ่มต้นที่ (1,1) - มุมซ้ายบน, หันหน้าไปทางทิศใต้ (0°)
- P2 เริ่มต้นที่ (13,13) - มุมขวาล่าง, หันหน้าไปทางทิศเหนือ (180°)
- ตัวละครอยู่สูงกว่าพื้น 0.3 unit (BLOCK_HEIGHT + 0.3f)

**Movement System:**
```cpp
bool TryMoveCharacter(CharacterPose& character, int dx, int dy, 
                      const std::vector<std::pair<int, int>>& breakableBlocks, 
                      const CharacterPose& otherChar)
{
    if(character.isMoving)
        return false;  // Already moving, can't start new movement
    
    int newX = character.gridX + dx;
    int newY = character.gridY + dy;
    
    if(IsWalkable(newX, newY, breakableBlocks, otherChar))
    {
        character.targetGridX = newX;
        character.targetGridY = newY;
        character.isMoving = true;
        character.moveProgress = 0.0f;
        
        // Set rotation based on direction
        if(dx > 0) character.rotation = glm::radians(90.0f);   // Right
        else if(dx < 0) character.rotation = glm::radians(-90.0f);  // Left
        else if(dy > 0) character.rotation = 0.0f;  // Down
        else if(dy < 0) character.rotation = glm::radians(180.0f);  // Up
        
        return true;
    }
    return false;
}
```

**คำอธิบาย:**
- **IsWalkable()**: ตรวจสอบว่าตำแหน่งนั้นสามารถเดินได้หรือไม่
  - ตรวจสอบขอบแผนที่
  - ตรวจสอบ Red Blocks
  - ตรวจสอบ Breakable Blocks
  - ตรวจสอบ collision กับตัวละครอื่น
- **Rotation**: หมุนตัวละครตามทิศทางการเคลื่อนที่
- **Grid-Based**: เคลื่อนที่ทีละ 1 tile

**Smooth Movement Update:**
```cpp
void UpdateCharacterMovement(CharacterPose& character, float deltaTime)
{
    if(character.isMoving)
    {
        const float moveSpeed = 3.0f;  // Blocks per second
        character.moveProgress += moveSpeed * deltaTime;
        
        if(character.moveProgress >= 1.0f)
        {
            character.moveProgress = 1.0f;
            character.gridX = character.targetGridX;
            character.gridY = character.targetGridY;
            character.isMoving = false;
        }
        
        // Interpolate position
        glm::vec3 startPos = GridToWorld(character.gridX, character.gridY);
        glm::vec3 endPos = GridToWorld(character.targetGridX, character.targetGridY);
        character.position = glm::mix(startPos, endPos, character.moveProgress);
        character.position.y = BLOCK_HEIGHT + 0.3f;
    }
    else
    {
        character.position = GridToWorld(character.gridX, character.gridY);
        character.position.y = BLOCK_HEIGHT + 0.3f;
    }
}
```

**คำอธิบาย:**
- **moveSpeed**: ความเร็วการเคลื่อนที่ (3 tiles ต่อวินาที)
- **Interpolation**: ใช้ `glm::mix()` เพื่อ interpolate ระหว่างตำแหน่งเริ่มต้นและปลายทาง
- **Delta Time**: ใช้ deltaTime เพื่อให้การเคลื่อนที่สม่ำเสมอไม่ขึ้นกับ frame rate

**Animation State Management:**
```cpp
if(leftPose.isMoving)
    SetAnimation(leftPose, animatorP1, CharacterPose::State::Walk, &idleAnimation, &walkAnimation);
else
    SetAnimation(leftPose, animatorP1, CharacterPose::State::Idle, &idleAnimation, &walkAnimation);

animatorP1.UpdateAnimation(deltaTime);
```

**คำอธิบาย:**
- สลับแอนิเมชันระหว่าง Idle และ Walk ตามสถานะการเคลื่อนที่
- อัปเดตแอนิเมชันทุก frame ด้วย deltaTime

**Character Rendering:**
```cpp
characterShader.use();
characterShader.setMat4("view", view);
characterShader.setMat4("projection", projection);

// Upload bone matrices for skeletal animation
const auto& transforms = animatorP1.GetFinalBoneMatrices();
for(int i = 0; i < transforms.size(); ++i)
{
    characterShader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
}

// Render character
glm::mat4 modelMatrix = glm::mat4(1.0f);
modelMatrix = glm::translate(modelMatrix, leftPose.position);
modelMatrix = glm::rotate(modelMatrix, leftPose.rotation, glm::vec3(0.0f, 1.0f, 0.0f));
modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
characterShader.setMat4("model", modelMatrix);
characterModelP1.Draw(characterShader);
```

**คำอธิบาย:**
- **Bone Matrices**: ส่ง bone transformation matrices ไปยัง shader สำหรับ skeletal animation
- **Model Matrix**: Transform ตัวละครตามตำแหน่ง, rotation, และ scale
- **Scale**: ลดขนาดตัวละครเป็น 50% (0.5f) เพื่อให้พอดีกับแผนที่

#### ส่วนที่ 11: Skybox Setup

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

#### ส่วนที่ 12: Cleanup

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

#### ส่วนที่ 13: Helper Functions

**processInput():**
```cpp
void processInput(GLFWwindow *window, std::vector<std::pair<int, int>>& breakableBlockPositions, 
                 std::mt19937& gen, const std::function<void(...)>& generateBlocks,
                 CharacterPose& leftCharacter, CharacterPose& rightCharacter)
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
    
    // Character Movement Input
    // Left character (P1) uses WASD
    ProcessCharacterInput(window, leftCharacter, 
                          GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D,
                          breakableBlockPositions, rightCharacter);
    
    // Right character (P2) uses Arrow keys
    ProcessCharacterInput(window, rightCharacter,
                          GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_LEFT, GLFW_KEY_RIGHT,
                          breakableBlockPositions, leftCharacter);
}
```

**ProcessCharacterInput():**
```cpp
bool ProcessCharacterInput(GLFWwindow* window, CharacterPose& character, 
                           int forwardKey, int backwardKey, int leftKey, int rightKey,
                           const std::vector<std::pair<int, int>>& breakableBlocks, 
                           const CharacterPose& otherChar)
{
    const bool forwardPressed  = glfwGetKey(window, forwardKey)  == GLFW_PRESS;
    const bool backwardPressed = glfwGetKey(window, backwardKey) == GLFW_PRESS;
    const bool leftPressed     = glfwGetKey(window, leftKey)     == GLFW_PRESS;
    const bool rightPressed    = glfwGetKey(window, rightKey)    == GLFW_PRESS;
    
    if(forwardPressed && !character.isMoving)
    {
        TryMoveCharacter(character, 0, -1, breakableBlocks, otherChar);  // Up
    }
    else if(backwardPressed && !character.isMoving)
    {
        TryMoveCharacter(character, 0, 1, breakableBlocks, otherChar);  // Down
    }
    else if(leftPressed && !character.isMoving)
    {
        TryMoveCharacter(character, -1, 0, breakableBlocks, otherChar);  // Left
    }
    else if(rightPressed && !character.isMoving)
    {
        TryMoveCharacter(character, 1, 0, breakableBlocks, otherChar);  // Right
    }
    
    return character.isMoving;
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

#### anim_model.vs และ anim_model.fs (Character Shaders)

ใช้สำหรับ render ตัวละครพร้อม skeletal animation

#### anim_model.vs (Character Vertex Shader)

```glsl
#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec2 tex;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;
layout(location = 5) in ivec4 boneIds; 
layout(location = 6) in vec4 weights;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

out vec2 TexCoords;

void main()
{
    vec4 totalPosition = vec4(0.0f);
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
    {
        if(boneIds[i] == -1) 
            continue;
        if(boneIds[i] >= MAX_BONES) 
        {
            totalPosition = vec4(pos, 1.0f);
            break;
        }
        vec4 localPosition = finalBonesMatrices[boneIds[i]] * vec4(pos, 1.0f);
        totalPosition += localPosition * weights[i];
    }

    TexCoords = tex;
    gl_Position = projection * view * model * totalPosition;
}
```

**คำอธิบาย:**
- **Skeletal Animation**: ใช้ bone matrices เพื่อ transform vertices ตามโครงกระดูก
- **Bone Weights**: แต่ละ vertex สามารถได้รับผลกระทบจากหลาย bones (สูงสุด 4 bones)
- **Bone IDs และ Weights**: มาจากโมเดลที่โหลดด้วย Assimp
- **finalBonesMatrices**: bone transformation matrices ที่คำนวณจาก Animator

#### anim_model.fs (Character Fragment Shader)

```glsl
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{    
    vec4 texColor = texture(texture_diffuse1, TexCoords);
    if (texColor.r < 0.01 && texColor.g < 0.01 && texColor.b < 0.01)
    {
        FragColor = vec4(0.4, 0.7, 0.9, 1.0);  // Default color for missing textures
    }
    else
    {
        FragColor = texColor;
    }
}
```

**คำอธิบาย:**
- อ่านสีจาก diffuse texture
- ถ้า texture สีดำ (missing texture) จะใช้สี default แทน

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
project(RunGame)
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
add_executable(RunGame ${SRC})

target_include_directories(RunGame PRIVATE
  ${CMAKE_BINARY_DIR}
  ${CMAKE_BINARY_DIR}/learnopengl
)

add_library(glad STATIC ${GLAD_SOURCE_FILE})
target_include_directories(glad PUBLIC ${GLAD_INCLUDE_DIR})

target_link_libraries(RunGame PRIVATE glfw glad assimp glm)
```

**คำอธิบาย:**
- **add_executable()**: สร้าง executable จาก main.cpp
- **target_include_directories()**: เพิ่ม include paths สำหรับ target
- **add_library()**: สร้าง static library จาก GLAD source
- **target_link_libraries()**: Link libraries เข้ากับ executable

**OpenGL Linking:**
```cmake
if (WIN32)
    target_link_libraries(RunGame PRIVATE opengl32)
else()
    find_package(OpenGL REQUIRED)
    target_link_libraries(RunGame PRIVATE OpenGL::GL)
endif()
```

**คำอธิบาย:**
- Windows: Link กับ opengl32
- Linux/macOS: ใช้ find_package เพื่อหา OpenGL และ link

#### ส่วนที่ 7: Copy Resources

```cmake
add_custom_command(TARGET RunGame POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:RunGame>/assets
  COMMENT "Copying assets to the target binary directory"
)

add_custom_command(TARGET RunGame POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:RunGame>/shaders
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
set_target_properties(RunGame PROPERTIES
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
const unsigned int SCR_WIDTH = 1280;   // เปลี่ยนความกว้าง
const unsigned int SCR_HEIGHT = 720;  // เปลี่ยนความสูง
```

### ปรับความเร็วการเคลื่อนที่ของตัวละคร
แก้ไขใน `src/main.cpp`:
```cpp
const float moveSpeed = 3.0f;  // Blocks per second (เปลี่ยนเป็นค่าที่ต้องการ)
```

### ปรับขนาดตัวละคร
แก้ไขใน `src/main.cpp`:
```cpp
modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));  // เปลี่ยน 0.5f เป็นค่าที่ต้องการ
```

### ปรับระบบระเบิด
แก้ไขใน `src/main.cpp`:
```cpp
Bomb(int x, int y, int ownerId) : gridX(x), gridY(y), timer(3.0f), owner(ownerId) {}
// เปลี่ยน 3.0f เป็นเวลาที่ต้องการ (วินาที)

const int EXPLOSION_RANGE = 2;  // เปลี่ยนเป็นระยะการระเบิดที่ต้องการ (ช่อง)
```

### ปรับความเร็วการหมุนของ Skybox
แก้ไขใน `src/main.cpp`:
```cpp
skyboxRotation += glm::radians(3.0f) * deltaTime;  // เปลี่ยน 3.0f เป็นองศาต่อวินาทีที่ต้องการ
```

### ปรับระดับเสียงเพลงพื้นหลัง
แก้ไขใน `src/main.cpp`:
```cpp
backgroundMusic.setVolume(0.5f);  // เปลี่ยน 0.5f เป็นค่าที่ต้องการ (0.0-1.0)
```

## 🎯 ระบบระเบิด (Bomb System)

### การทำงานของระเบิด
- **การวางระเบิด**: ผู้เล่นสามารถวางระเบิดได้ที่ตำแหน่งปัจจุบัน (กด Q สำหรับ P1, M สำหรับ P2)
- **ข้อจำกัด**: แต่ละผู้เล่นมีจำนวนระเบิดสูงสุดที่วางได้พร้อมกัน (เริ่มต้น: 3 ลูก, เพิ่มได้ด้วย Power-Up)
- **Timer**: ระเบิดจะระเบิดหลังจาก 3 วินาที
- **การระเบิด**: ระเบิดจะทำลายบล็อกทำลายได้ในรูปแบบกากบาท (2 ช่องในแต่ละทิศทาง: ขึ้น, ลง, ซ้าย, ขวา)
- **การหยุดการระเบิด**: การระเบิดจะหยุดเมื่อเจอบล็อกทำลายไม่ได้หรือขอบแผนที่
- **Explosion Preview**: แสดง preview บริเวณที่ระเบิดจะระเบิดด้วย pulsing animation (ยิ่งใกล้ระเบิดยิ่งชัดเจน)
- **Collision**: ตัวละครไม่สามารถเดินผ่านระเบิดได้

### โครงสร้างข้อมูลระเบิด
```cpp
struct Bomb
{
    int gridX;        // ตำแหน่ง X ใน grid
    int gridY;        // ตำแหน่ง Y ใน grid
    float timer;      // Timer นับถอยหลัง (วินาที) - เริ่มต้น 3.0 วินาที
    int owner;        // เจ้าของระเบิด (1 = P1, 2 = P2)
    bool exploded;    // สถานะการระเบิด - true เมื่อระเบิดแล้ว
};
```

### การจัดการจำนวนระเบิด
- ผู้เล่นแต่ละคนมี `activeBombCount` เพื่อติดตามจำนวนระเบิดที่วางอยู่
- เมื่อระเบิดระเบิดแล้ว `activeBombCount` จะลดลง
- ผู้เล่นไม่สามารถวางระเบิดได้ถ้า `activeBombCount >= maxBombCount`

## 🎮 ตรรกะเกมและกลไกการเล่น (Game Logic & Mechanics)

### 📋 สรุประบบหลักของเกม

| ระบบ | คำอธิบาย | ค่าเริ่มต้น |
|------|----------|------------|
| **แผนที่** | ขนาด 15×15 tiles พร้อมบล็อกและจุด spawn | - |
| **Health** | ผู้เล่นแต่ละคนมี Health 3 จุด | 3 |
| **Bomb Timer** | เวลานับถอยหลังก่อนระเบิด | 3 วินาที |
| **Explosion Range** | ระยะการระเบิดในแต่ละทิศทาง | 2 ช่อง |
| **Max Bombs** | จำนวนระเบิดสูงสุดที่วางได้พร้อมกัน | 3 ลูก |
| **Movement Speed** | ความเร็วการเคลื่อนที่ | 3 tiles/s |
| **Power-Up Drop** | โอกาสที่บล็อกทำลายได้จะดรอป Power-Up | 30% |
| **Breakable Blocks** | โอกาสในการสร้างบล็อกทำลายได้ | 60% |

### 🗺️ โครงสร้างแผนที่ (Map Structure)

เกมใช้แผนที่ขนาด **15×15 tiles** โดยแบ่งเป็น:

1. **ขอบแผนที่ (Border Blocks)**:
   - วางรอบๆ แผนที่ที่ตำแหน่ง x=0, x=14, z=0, z=14
   - เป็นบล็อกทำลายไม่ได้ สูง 1.0 unit
   - ทำหน้าที่เป็นกำแพงป้องกันไม่ให้ผู้เล่นออกนอกแผนที่

2. **บล็อกทำลายไม่ได้แบบแพทเทิร์น (Red Blocks)**:
   - วางที่ตำแหน่ง x และ z เป็นเลขคู่ (2, 4, 6, 8, 10, 12)
   - สูง 0.75 unit (75% ของขอบ)
   - สร้างเป็นแพทเทิร์นถาวรในแผนที่เพื่อกำหนดเส้นทาง

3. **จุด Spawn ผู้เล่น (Green Cells)**:
   - **P1 (Player 1)**: มุมซ้ายบน ที่ตำแหน่ง (1,1), (1,2), (2,1), (2,2) - 2×2 cluster
   - **P2 (Player 2)**: มุมขวาล่าง ที่ตำแหน่ง (12,12), (12,13), (13,12), (13,13) - 2×2 cluster
   - พื้นที่เหล่านี้ไม่มีบล็อกทำลายได้ เพื่อให้ผู้เล่นเริ่มต้นได้ง่าย

4. **บล็อกทำลายได้ (Breakable Blocks)**:
   - สร้างแบบสุ่มในพื้นที่ "White Cells" (พื้นที่ว่างที่ไม่ใช่ขอบ, Red Blocks, หรือ Green Cells)
   - ความน่าจะเป็นในการสร้าง: **60%** (ค่าเริ่มต้น)
   - สูง 0.75 unit
   - เมื่อถูกระเบิดทำลาย อาจดรอป Power-Up (30% โอกาส)

### 🚶 ระบบการเคลื่อนที่ (Movement System)

#### Grid-Based Movement
- ตัวละครเคลื่อนที่แบบ **grid-based** ทีละ 1 tile
- ไม่สามารถเคลื่อนที่แบบ diagonal ได้
- การเคลื่อนที่ใช้ **smooth interpolation** เพื่อให้การเคลื่อนไหวลื่นไหล

#### การตรวจสอบ Collision
ตัวละครไม่สามารถเคลื่อนที่ไปที่ตำแหน่งที่:
1. **ขอบแผนที่** (Border blocks)
2. **บล็อกทำลายไม่ได้** (Red blocks)
3. **บล็อกทำลายได้** (Breakable blocks)
4. **ระเบิดที่ยังไม่ระเบิด** (Active bombs)
5. **ตำแหน่งที่ตัวละครอื่นอยู่** หรือกำลังเคลื่อนที่ไป (Character collision)

#### ความเร็วการเคลื่อนที่
- **ความเร็วพื้นฐาน**: 3 tiles ต่อวินาที
- **เมื่อใช้ Speed Boost**: ความเร็วเพิ่มเป็น 2 เท่า (6 tiles ต่อวินาที) นาน 10 วินาที

#### การหมุนตัวละคร
ตัวละครจะหมุนตามทิศทางการเคลื่อนที่:
- **ขวา (Right)**: หมุน 90° (หันไปทางทิศตะวันออก)
- **ซ้าย (Left)**: หมุน -90° (หันไปทางทิศตะวันตก)
- **ลง (Down)**: หมุน 0° (หันไปทางทิศใต้)
- **ขึ้น (Up)**: หมุน 180° (หันไปทางทิศเหนือ)

### 💣 ระบบระเบิด (Bomb System)

#### การวางระเบิด
- **P1**: กด **Q** เพื่อวางระเบิด
- **P2**: กด **M** เพื่อวางระเบิด
- **ข้อจำกัด**: 
  - ไม่สามารถวางระเบิดได้ถ้ามีระเบิดอยู่แล้วที่ตำแหน่งนั้น
  - ตัวละครแต่ละตัวมีขีดจำกัดจำนวนระเบิดที่วางได้พร้อมกัน (เริ่มต้น: 3 ลูก, เพิ่มได้ด้วย Power-Up)

#### Timer และการระเบิด
- ระเบิดมี **timer 3 วินาที** นับถอยหลังก่อนระเบิด
- เมื่อ timer หมด ระเบิดจะระเบิดทันที
- ระเบิดจะถูกทำเครื่องหมายว่า `exploded = true` และจำนวน active bombs ของผู้เล่นจะลดลง

#### รูปแบบการระเบิด (Explosion Pattern)
การระเบิดเป็นรูปแบบ **กากบาท (Cross Pattern)**:

```
    ↑
    |
← ← 💣 → →
    |
    ↓
```

- **ระยะการระเบิดพื้นฐาน**: 2 ช่องในแต่ละทิศทาง (ขึ้น, ลง, ซ้าย, ขวา)
- **เมื่อใช้ Range Boost**: ระยะเพิ่มเป็น 3 ช่อง (นาน 10 วินาที)
- **การหยุดการระเบิด**:
  - หยุดทันทีเมื่อเจอ **บล็อกทำลายไม่ได้** (Red blocks หรือ Border)
  - หยุดทันทีเมื่อเจอ **บล็อกทำลายได้** (Breakable blocks) - จะทำลายบล็อกก่อนหยุด

#### ความเสียหายต่อผู้เล่น
- เมื่อผู้เล่นอยู่ในบริเวณที่ระเบิด (รวมถึงตำแหน่งของระเบิดและทุกช่องในรัศมีระเบิด) จะได้รับความเสียหาย:
  - **ลด Health 1 จุด** (เริ่มต้นมี 3 จุด)
  - **Invulnerability Timer**: ผู้เล่นจะปลอดภัยจากการโดนระเบิดซ้ำเป็นเวลา 1 วินาที
  - **Shield Protection**: ถ้ามี Shield active จะไม่ได้รับความเสียหาย

#### การทำลายบล็อกทำลายได้
- เมื่อระเบิดทำลายบล็อกทำลายได้:
  - บล็อกจะถูกลบออกจากแผนที่
  - มีโอกาส **30%** ที่จะดรอป Power-Up ที่ตำแหน่งนั้น
  - ประเภทของ Power-Up ถูกสุ่มแบบเท่ากัน (20% ต่อแต่ละประเภท)

### 💎 ระบบ Power-Up

เมื่อบล็อกทำลายได้ถูกทำลาย จะมีโอกาส 30% ที่จะดรอป Power-Up หนึ่งใน 5 ประเภท:

#### 1. 🔵 Range Boost (Blue Power)
- **ผล**: เพิ่มระยะการระเบิด +1 ช่อง (จาก 2 เป็น 3 ช่อง)
- **ระยะเวลา**: 10 วินาที
- **การใช้งาน**: มีผลกับระเบิดที่วางขณะ Power-Up active

#### 2. 🟠 Bomb Capacity (Orange Bomb)
- **ผล**: เพิ่มจำนวนระเบิดสูงสุดที่วางได้พร้อมกัน +1
- **ระยะเวลา**: **ถาวร** (ไม่หมดเวลา)
- **การใช้งาน**: เริ่มต้นที่ 3 ลูก, เพิ่มได้ไม่จำกัด

#### 3. ⚪ Shield (White Shield)
- **ผล**: ป้องกันความเสียหายจากระเบิด
- **ระยะเวลา**: 5 วินาที
- **การใช้งาน**: ตัวละครจะไม่ได้รับความเสียหายเมื่ออยู่ในบริเวณระเบิด (แต่ยังมี Invulnerability Timer)

#### 4. 🟢 Speed Boost (Green Speed)
- **ผล**: เพิ่มความเร็วการเคลื่อนที่เป็น 2 เท่า
- **ระยะเวลา**: 10 วินาที
- **การใช้งาน**: ความเร็วเพิ่มจาก 3 tiles/s เป็น 6 tiles/s

#### 5. ❤️ Heart (Red Heart)
- **ผล**: ฟื้นฟู Health +1 จุด
- **การใช้งาน**: 
  - เพิ่ม Health ได้ถึงสูงสุด 3 จุด
  - ถ้า Health เต็มแล้วจะไม่ได้รับผล

#### การเก็บ Power-Up
- Power-Up ถูกเก็บอัตโนมัติเมื่อผู้เล่นเดินเข้าไปที่ตำแหน่งเดียวกัน
- Power-Up มีแอนิเมชันหมุนเพื่อให้เห็นชัดเจน
- เมื่อเก็บแล้วจะเล่นเสียงและถูกลบออกจากแผนที่

### ❤️ ระบบ Health และเกมโอเวอร์

#### Health System
- **Health เริ่มต้น**: 3 จุด (แสดงเป็น 3 หัวใจบน UI)
- **การลด Health**: 
  - โดนระเบิด = -1 Health
  - ถ้า Health = 0 ผู้เล่นจะแพ้

#### Invulnerability Frame
- เมื่อผู้เล่นได้รับความเสียหายจะมี **Invulnerability Timer 1 วินาที**
- ในช่วงนี้ผู้เล่นจะไม่ได้รับความเสียหายจากระเบิดครั้งใหม่ (ป้องกันการโดนซ้ำหลายครั้ง)

#### Game Over Condition
เกมจะจบเมื่อผู้เล่นคนใดคนหนึ่ง Health = 0:
- **P1 Health ≤ 0**: **P2 ชนะ** (Game Over - Player 2 Wins!)
- **P2 Health ≤ 0**: **P1 ชนะ** (Game Over - Player 1 Wins!)

#### การเริ่มเกมใหม่
- เมื่อเกมจบ ผู้เล่นสามารถกดปุ่ม **Restart** เพื่อเริ่มเกมใหม่
- เกมจะรีเซ็ต:
  - ตำแหน่งผู้เล่นกลับไปที่จุด spawn
  - Health กลับมาเป็น 3
  - บล็อกทำลายได้ถูกสร้างใหม่แบบสุ่ม
  - ระเบิดและ Power-Up ทั้งหมดถูกลบ
  - Power-Up timers ถูกรีเซ็ต

### 🎯 กลยุทธ์การเล่น

#### กลยุทธ์พื้นฐาน
1. **วางกับดัก**: วางระเบิดเพื่อบังคับให้คู่ต่อสู้เคลื่อนที่ไปในทิศทางที่ต้องการ
2. **บล็อกเส้นทาง**: ใช้ระเบิดเพื่อปิดกั้นเส้นทางหนีของคู่ต่อสู้
3. **ทำลายบล็อก**: ทำลายบล็อกเพื่อเปิดเส้นทางและหาวัตถุ Power-Up
4. **หนีให้ทัน**: ระเบิดมีเวลา 3 วินาที ให้วางระเบิดแล้วรีบหนี

#### กลยุทธ์ระดับสูง
1. **Chain Bombs**: วางระเบิดหลายลูกในตำแหน่งที่เชื่อมกันเพื่อสร้างระเบิดต่อเนื่อง
2. **Power-Up Priority**: 
   - Shield ช่วยป้องกันในสถานการณ์วิกฤต
   - Range Boost ช่วยโจมตีจากระยะไกล
   - Bomb Capacity ช่วยวางกับดักได้มากขึ้น
3. **Positioning**: ควบคุมพื้นที่กลางแผนที่เพื่อครอบครอง Power-Up
4. **Timing**: ใช้ความเร็วในการหนีและวางระเบิดให้เหมาะสม

### 🔄 Game Loop และการอัปเดต

เกมทำงานตาม Game Loop ต่อไปนี้ (ทุก frame):

1. **Input Processing**: 
   - อ่าน keyboard input
   - ตรวจสอบการเคลื่อนที่และวางระเบิด

2. **Game State Updates** (ถ้าเกมยังไม่จบ):
   - **Update Bombs**: ลด timer, ตรวจสอบการระเบิด
   - **Update Power-Ups**: อัปเดตแอนิเมชันหมุน, ตรวจสอบการเก็บ
   - **Update Characters**: อัปเดตการเคลื่อนที่, animations, timers (shield, speed boost, etc.)

3. **Collision Detection**:
   - ตรวจสอบว่าตำแหน่งใหม่สามารถเดินได้หรือไม่
   - ตรวจสอบการชนกันระหว่างผู้เล่น

4. **Game Over Check**:
   - ตรวจสอบว่า Health ของผู้เล่นคนใดคนหนึ่งเป็น 0 หรือไม่

5. **Rendering**:
   - Render แผนที่ (พื้น, ขอบ, บล็อก)
   - Render ระเบิดและ explosion preview
   - Render Power-Ups
   - Render ตัวละคร
   - Render UI (Health, Game Over screen)
   - Render Skybox

6. **Audio**:
   - เล่นเสียงเพลงพื้นหลัง
   - เล่นเสียงระเบิดและ Power-Up เมื่อเกิดเหตุการณ์

### 📊 สรุปค่าพารามิเตอร์หลัก

| พารามิเตอร์ | ค่าเริ่มต้น | คำอธิบาย |
|------------|------------|----------|
| Health | 3 | จำนวน Health ของผู้เล่น |
| Bomb Timer | 3.0 วินาที | เวลานับถอยหลังก่อนระเบิด |
| Base Explosion Range | 2 ช่อง | ระยะการระเบิดพื้นฐาน |
| Boosted Explosion Range | 3 ช่อง | ระยะการระเบิดเมื่อใช้ Range Boost |
| Base Movement Speed | 3 tiles/s | ความเร็วการเคลื่อนที่พื้นฐาน |
| Speed Boost Multiplier | 2x | ตัวคูณความเร็วเมื่อใช้ Speed Boost |
| Max Bomb Count | 3 ลูก | จำนวนระเบิดสูงสุดที่วางได้พร้อมกัน |
| Breakable Block Probability | 60% | โอกาสในการสร้างบล็อกทำลายได้ |
| Power-Up Drop Rate | 30% | โอกาสที่บล็อกทำลายได้จะดรอป Power-Up |
| Range Boost Duration | 10 วินาที | ระยะเวลาของ Range Boost |
| Shield Duration | 5 วินาที | ระยะเวลาของ Shield |
| Speed Boost Duration | 10 วินาที | ระยะเวลาของ Speed Boost |
| Invulnerability Time | 1.0 วินาที | เวลาที่ปลอดภัยหลังโดนระเบิด |

## 🔊 ระบบเสียง (Audio System)

### Background Music
- รองรับการเล่นไฟล์ MP3 บน macOS ผ่าน AVFoundation
- เล่นวนซ้ำอัตโนมัติ
- สามารถปรับระดับเสียงได้ (0.0 - 1.0)
- โหลดไฟล์จาก `assets/sound/background.mp3`

### Platform Support
- **macOS**: รองรับเต็มรูปแบบผ่าน AVFoundation
- **Windows/Linux**: ยังไม่รองรับ (placeholder implementation)

## 📄 License

โปรเจคนี้ใช้ไลบรารีภายนอกหลายตัวที่มี license ต่างกัน:
- GLFW: Zlib/libpng license
- GLM: MIT license
- Assimp: BSD 3-Clause license
- GLAD: Public domain
- STB: Public domain
