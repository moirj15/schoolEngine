#set(CMAKE_C_COMPILER_WORKS 1)
#set(CMAKE_CXX_COMPILER_WORKS 1)
#set(CMAKE_C_COMPILER "C:/Program Files/LLVM/bin/clang.exe")
#set(CMAKE_CXX_COMPILER "C:/Program Files/LLVM/bin/clang.exe")
cmake_minimum_required(VERSION 3.10)
project(yeo)

file(GLOB IMGUI_FILES "imgui/*")
file(GLOB SRC_FILES "src/*")
file(GLOB_RECURSE GUI_FILES "src/gui/*")
file(GLOB STB_FILES "stb/*")

#source_group("imgui" FILES ${IMGUI_FILES})
#source_group("stb" FILES ${STB_FILES})
#source_group("src" FILES ${SRC_FILES})
#source_group("src/gui" FILES ${GUI_FILES})

#add_library(yeo ${IMGUI_FILES} ${STB_FILES} ${SRC_FILES})

set(SOURCE_LIST 
	${SRC_FILES}
	${STB_FILES}
	${IMGUI_FILES}
	${GUI_FILES}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_LIST})

add_executable(
    yoe
	${SOURCE_LIST}
#	${SRC_FILES}
#	${STB_FILES}
#	${IMGUI_FILES}
#	${GUI_FILES}
)
#λ cmake .. -G"NMake Makefiles JOM" -DCMAKE_C_COMPILER="C:/Program Files/LLVM/bin/clang.exe" -DCMAKE_CXX_COMPILER="C:/Program Files/LLVM/bin/clang.exe" -DCMAKE_LINKER="C:/Program Files/LLVM/bin/lld-link.exe" -DCMAKE_C_COMPILER_ID="Clang" -DCMAKE_CXX_COMPILER_ID="CLANG" -DCMAKE_SYSTEM_NAME="Generic"
find_package(OpenGL REQUIRED)
#set(CMAKE_CXX_FLAGS "-std=c++17 -g -Wall -Wextra -pedantic -D_DEBUG")#-MT -D_DLL")
#set(CMAKE_EXE_LINKER_FLAGS "-lGL -lGLEW -lglfw")

if (WIN32)
	message("WINDOWS")
    include_directories(
        D:/libraries/glm;
        D:/libraries/glfw-3.2.1.bin.WIN64/include;
        D:/libraries/glew-2.1.0/include
    )
	set(CMAKE_CXX_FLAGS "/std:c++17 /MP /GS /TP /W3 -D_CRT_SECURE_NO_WARNINGS")
	#set(CMAKE_CXX_FLAGS "-std=c++17 -g -Wall -Wextra -pedantic -D_DEBUG")
#    #add_definitions(-DGLEW_STATIC=1)
    target_link_libraries(
        yoe
        ${OPENGL_LIBRARY}#OpenGL
        #D:/libraries/glew-2.1.0/lib/Release/x64/libglew32.a
        #D:/libraries/glew-2.1.0/lib/Release/x64/libglew32.dll.a
        #D:/libraries/glew-2.1.0/lib/Release/x64/glew32s.lib
        D:/libraries/glew-2.1.0/lib/Release/x64/glew32.lib
        #D:/libraries/glew-2.1.0/lib/Release/x64/glew32.dll
        D:/libraries/glfw-3.2.1.bin.WIN64/lib-vc2015/glfw3.lib
        #D:/libraries/glfw-3.2.1.bin.WIN64/lib-mingw-w64/libglfw3.a
    )
else()
message("not windows hit me")
	set(CMAKE_CXX_FLAGS "-std=c++17 -g -Wall -Wextra -pedantic -D_DEBUG")
    target_link_libraries(
        yoe
        opengl32
        glew32
        glfw3
	)
#set(CMAKE_CXX_FLAGS "-std=c++17 -g -Wall -Wextra -pedantic -D_DEBUG")
#    include_directories(
#        D:/libraries/glm;
#        D:/libraries/glfw-3.2.1.bin.WIN64/include;
#        D:/libraries/glew-2.1.0/include
#    )
#    find_package(GLEW REQUIRED)
#    find_package(GLFW3 REQUIRED)
#    target_link_libraries(
#        yoe
#        opengl32
#        glew32
#        glfw3
#    )
endif ()







