#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "LexicalAnalyzer::lexical_analyzer" for configuration ""
set_property(TARGET LexicalAnalyzer::lexical_analyzer APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(LexicalAnalyzer::lexical_analyzer PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/bin/lexer.exe"
  )

list(APPEND _cmake_import_check_targets LexicalAnalyzer::lexical_analyzer )
list(APPEND _cmake_import_check_files_for_LexicalAnalyzer::lexical_analyzer "${_IMPORT_PREFIX}/bin/lexer.exe" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
