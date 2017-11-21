################################################################################
# Set up the required version of cmake and policies related to versions.
################################################################################
CMake_Minimum_Required(VERSION 3.9.1 FATAL_ERROR) 
Set_Property(GLOBAL PROPERTY USE_FOLDERS ON)  

################################################################################
# Declaring our Project and it's used languages.
################################################################################
Project(JoshPlayer CXX C)

################################################################################
# Set up some variables that we'll be able to access anywhere.
################################################################################
Set(CMake_Include          ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
Set(Dependencies_Root      ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies)
Set(JoshPlayer_Binary_Dir  ${CMAKE_CURRENT_SOURCE_DIR}/Output/Binary)
Set(JoshPlayer_Library_Dir ${CMAKE_CURRENT_SOURCE_DIR}/Output/Library)

################################################################################
# Path to modules for finding external libraries.
################################################################################
Set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${Source_Root}/cmake/modules/")

################################################################################
## Includes
################################################################################
Include(${CMake_Include}/AddFile.cmake)
Include(${CMake_Include}/JoshPlayer_Source_Group.cmake) 

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