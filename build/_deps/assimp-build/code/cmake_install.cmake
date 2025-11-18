# Install script for directory: /Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "libassimp5.2.4-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-build/lib/libassimp.a")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimp.a" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimp.a")
    execute_process(COMMAND "/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimp.a")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "assimp-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/anim.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/aabb.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/ai_assert.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/camera.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/color4.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/color4.inl"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-build/code/../include/assimp/config.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/ColladaMetaData.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/commonMetaData.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/defs.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/cfileio.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/light.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/material.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/material.inl"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/matrix3x3.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/matrix3x3.inl"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/matrix4x4.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/matrix4x4.inl"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/mesh.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/ObjMaterial.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/pbrmaterial.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/GltfMaterial.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/postprocess.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/quaternion.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/quaternion.inl"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/scene.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/metadata.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/texture.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/types.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/vector2.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/vector2.inl"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/vector3.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/vector3.inl"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/version.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/cimport.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/importerdesc.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/Importer.hpp"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/DefaultLogger.hpp"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/ProgressHandler.hpp"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/IOStream.hpp"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/IOSystem.hpp"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/Logger.hpp"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/LogStream.hpp"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/NullLogger.hpp"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/cexport.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/Exporter.hpp"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/DefaultIOStream.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/DefaultIOSystem.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/ZipArchiveIOSystem.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/SceneCombiner.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/fast_atof.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/qnan.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/BaseImporter.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/Hash.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/MemoryIOWrapper.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/ParsingUtils.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/StreamReader.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/StreamWriter.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/StringComparison.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/StringUtils.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/SGSpatialSort.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/GenericProperty.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/SpatialSort.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/SkeletonMeshBuilder.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/SmallVector.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/SmoothingGroups.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/SmoothingGroups.inl"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/StandardShapes.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/RemoveComments.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/Subdivision.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/Vertex.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/LineSplitter.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/TinyFormatter.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/Profiler.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/LogAux.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/Bitmap.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/XMLTools.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/IOStreamBuffer.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/CreateAnimMesh.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/XmlParser.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/BlobIOSystem.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/MathFunctions.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/Exceptional.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/ByteSwapper.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/Base64.hpp"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "assimp-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/Compiler/pushpack1.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/Compiler/poppack1.h"
    "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-src/code/../include/assimp/Compiler/pstdint.h"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/Users/sarootareerattanawetch/OpenGL/GameEngProject-Palbom/build/_deps/assimp-build/code/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
