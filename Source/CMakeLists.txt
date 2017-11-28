Set(Source_Root ${CMAKE_CURRENT_SOURCE_DIR})

Include_Directories(${Source_Root})
Include_Directories(${ParentDirectory}
                    ${Dependencies_Root}/vlc-2.2.6/include)

Set(MediaPlaymerSource main.cpp
                   WinMain.cpp
                   FourCC.cpp
                   FourCC.hpp
                   iTunesImport.hpp
                   Library.cpp
                   Library.hpp
                   MusicPlayer.cpp
                   MusicPlayer.hpp
                   MediaPlaymer.rc)

Add_Executable(MediaPlaymer ${MediaPlaymerSource})

Set_Target_Properties(MediaPlaymer
                      PROPERTIES
                      ARCHIVE_OUTPUT_DIRECTORY ${MediaPlaymer_Library_Dir}
                      LIBRARY_OUTPUT_DIRECTORY ${MediaPlaymer_Library_Dir}
                      RUNTIME_OUTPUT_DIRECTORY ${MediaPlaymer_Binary_Dir})
     
MediaPlaymer_Source_Group(Source_Root MediaPlaymerSource)
                          
# Find the QtWidgets library
Find_Package(Qt5Widgets)
Find_Package(Qt5Sql)
Find_Package(Qt5Xml)


#Target_Link_Libraries(MediaPlaymer Imagehlp.lib Qt5::Widgets)
Target_Link_Libraries(MediaPlaymer ${Dependencies_Root}/vlc-2.2.6/libvlc.lib Qt5::Widgets Qt5::Sql Qt5::Xml)

Target_Compile_Options(MediaPlaymer PRIVATE -permissive- -std:c++17 -WX- -W4)

Set_Property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT MediaPlaymer) 
Set_Target_Properties(MediaPlaymer PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${Repo_Root}/Assets)