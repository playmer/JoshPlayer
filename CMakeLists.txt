################################################################################
# Set up the required version of cmake and policies related to versions.
################################################################################
CMake_Minimum_Required(VERSION 3.9.1 FATAL_ERROR) 
Set_Property(GLOBAL PROPERTY USE_FOLDERS ON)  

################################################################################
# Declaring our Project and it's used languages.
################################################################################
Project(MediaPlaymer CXX C)

################################################################################
# Set up some variables that we'll be able to access anywhere.
################################################################################
Set(Repo_Root               ${CMAKE_CURRENT_SOURCE_DIR})
Set(CMake_Include          ${Repo_Root}/cmake)
Set(Dependencies_Root      ${Repo_Root}/Dependencies)
Set(MediaPlaymer_Binary_Dir  ${Repo_Root}/Output/Binary)
Set(MediaPlaymer_Library_Dir ${Repo_Root}/Output/Library)

################################################################################
# Path to modules for finding external libraries.
################################################################################
Set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${Source_Root}/cmake/modules/")

################################################################################
## Includes
################################################################################
Include(${CMake_Include}/AddFile.cmake)
Include(${CMake_Include}/MediaPlaymer_Source_Group.cmake) 

################################################################################
## Setting compiler warnings based on platform.
################################################################################
If (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
  Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
  Set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /debug:FASTLINK")
EndIf()

Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DGLM_FORCE_CXX14 ")

################################################################################
## Separate Projects into folders for IDEs.
################################################################################
Set_Property(GLOBAL PROPERTY USE_FOLDERS ON)

################################################################################
## Sets the folder to check for dependencies in, based on architecture.
################################################################################
Set(QtPath $ENV{Qt})
If(NOT QtPath) 
  Message(FATAL_ERROR "Please define the environment variable `Qt` with the path to Qt 5.9.0 - i.e. export Qt = C:/Qt/5.9")
EndIf()

If (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
  Set(CMAKE_PREFIX_PATH ${QtPath}/msvc2017_64/)
EndIf()

################################################################################
## Call the CMakeLists.txt in our engine folder, and game folder.
################################################################################
Add_Subdirectory(Source)