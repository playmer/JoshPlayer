Set(Source_Root ${CMAKE_CURRENT_SOURCE_DIR})

Include_Directories(${Source_Root})
Include_Directories(${ParentDirectory}
                    ${Dependencies_Root}/vlc-2.2.6/include)

Set(JoshPlayerSource main.cpp
                   WinMain.cpp
                   FourCC.cpp
                   FourCC.hpp
                   iTunesImport.hpp
                   MusicPlayer.cpp
                   MusicPlayer.hpp
                   JoshPlayer.rc)

Add_Executable(JoshPlayer ${JoshPlayerSource})

Set_Target_Properties(JoshPlayer
                      PROPERTIES
                      ARCHIVE_OUTPUT_DIRECTORY ${JoshPlayer_Library_Dir}
                      LIBRARY_OUTPUT_DIRECTORY ${JoshPlayer_Library_Dir}
                      RUNTIME_OUTPUT_DIRECTORY ${JoshPlayer_Binary_Dir})
     
JoshPlayer_Source_Group(Source_Root JoshPlayerSource)
                          
# Find the QtWidgets library
Find_Package(Qt5Widgets)
Find_Package(Qt5Sql)
Find_Package(Qt5Xml)


#Target_Link_Libraries(JoshPlayer Imagehlp.lib Qt5::Widgets)
Target_Link_Libraries(JoshPlayer ${Dependencies_Root}/vlc-2.2.6/libvlc.lib Qt5::Widgets Qt5::Sql Qt5::Xml)

Target_Compile_Options(JoshPlayer PRIVATE -permissive- -std:c++17 -WX- -W4)

Set_Property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT JoshPlayer) 
Set_Target_Properties(JoshPlayer PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${Repo_Root}/Assets)