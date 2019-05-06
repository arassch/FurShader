cmake_minimum_required(VERSION 3.1.0)

project(shader)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# set(CMAKE_VERBOSE_MAKEFILE ON)


# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)


add_definitions(-DGL_SILENCE_DEPRECATION)

set(Qt5Core_DIR "/usr/local/Cellar/qt/5.10.1/lib/cmake/Qt5Core")
set(Qt5Gui_DIR "/usr/local/Cellar/qt/5.10.1/lib/cmake/Qt5Gui")
set(Qt5Widgets_DIR "/usr/local/Cellar/qt/5.10.1/lib/cmake/Qt5Widgets")

# Find the QtWidgets library
find_package(Qt5Core CONFIG REQUIRED )
find_package(Qt5Gui CONFIG REQUIRED )
find_package(Qt5Widgets CONFIG REQUIRED )
find_package(Qt5OpenGL CONFIG REQUIRED )
find_package(Qt5Xml CONFIG REQUIRED )


find_package(OPENGL REQUIRED)
#find_package(GLUT REQUIRED HINT /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/System/Library/Frameworks)

# message(${GLUT_INCLUDE_DIRS})

# Populate a CMake variable with the sources
set(shader_SRCS
    src/main.cpp 
    src/simpleViewer.cpp
    src/textfile.cpp
    src/viewerInterface.cpp
    src/viewerInterface.ui
)

# Tell CMake to create the helloworld executable
add_executable(shader ${shader_SRCS})

target_include_directories(shader PUBLIC
    /Users/sarac.schvartzman/Dropbox/Code/ElasticFEM/LinearAlgebra/inc
    /Users/sarac.schvartzman/Dropbox/Code/ElasticFEM/TriMesh/inc
    /Users/sarac.schvartzman/Dropbox/Code/ElasticFEM/CDQueries/inc
    /Users/sarac.schvartzman/Dropbox/Code/libQGLViewer-2.7.0
    ${OPENGL_INCLUDE_DIRS}
    /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/System/Library/Frameworks/GLUT.framework/Headers
)

if (CMAKE_BUILD_TYPE STREQUAL "DEBUG")
    message("DEBUG compile")
    find_library(LinearAlgebra
        NAMES LinearAlgebra
        HINTS "/Users/sarac.schvartzman/Dropbox/Code/ElasticFEM/build-LinearAlgebra-Clang_omp-Debug"
    )

    find_library(TriMesh
        NAMES TriMesh
        HINTS "/Users/sarac.schvartzman/Dropbox/Code/ElasticFEM/build-TriMesh-Clang_omp-Debug"
    )

    find_library(CDQueries
        NAMES CDQueries
        HINTS "/Users/sarac.schvartzman/Dropbox/Code/ElasticFEM/build-CDQueries-Clang_omp-Debug"
    )
else()
    message("RELEASE compile")
    find_library(LinearAlgebra
        NAMES LinearAlgebra
        HINTS "/Users/sarac.schvartzman/Dropbox/Code/ElasticFEM/build-LinearAlgebra-Clang_omp-Release"
    )

    find_library(TriMesh
        NAMES TriMesh
        HINTS "/Users/sarac.schvartzman/Dropbox/Code/ElasticFEM/build-TriMesh-Clang_omp-Release"
    )

    find_library(CDQueries
        NAMES CDQueries
        HINTS "/Users/sarac.schvartzman/Dropbox/Code/ElasticFEM/build-CDQueries-Clang_omp-Release"
    )
endif()

find_library(QGLViewer
    NAMES QGLViewer
    HINTS "/Users/sarac.schvartzman/Dropbox/Code/libQGLViewer-2.7.0/QGLViewer"
)

# find_library(GLEW
#     NAMES GLEW
#     HINTS "/usr/local/Cellar/glew/2.1.0/lib"
# )



target_link_libraries(shader 
    Qt5::Gui 
    Qt5::OpenGL 
    Qt5::Widgets
    Qt5::Xml
    ${OPENGL_LIBRARIES} 
    /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/System/Library/Frameworks/GLUT.framework
    ${GLEW_LIBRARY}
    ${QGLViewer}
)

target_link_libraries(shader 
    ${LinearAlgebra}
    ${TriMesh}
    ${CDQueries}
)