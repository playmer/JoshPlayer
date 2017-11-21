Macro(AddFile aSource aPreviousSource aFileName)
  Get_Filename_Component(DirectoryName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
  
  Set(${aPreviousSource} ${${aPreviousSource}} ${DirectoryName}/${aFileName})
EndMacro()
