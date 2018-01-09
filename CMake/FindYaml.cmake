# - Find YAML
# Find the native YAML includes and library
#
#  YAML_INCLUDE_DIR  - where to find yaml.h, etc.
#  YAML_LIBRARIES    - List of libraries when using YAML.
#  YAML_FOUND        - True if YAML found.

IF (YAML_INCLUDE_DIR AND YAML_LIBRARIES)
  # Already in cache, be silent
  SET(YAML_FIND_QUIETLY TRUE)
ENDIF (YAML_INCLUDE_DIR AND YAML_LIBRARIES)


IF (WIN32)  
    SET(YAML_NAMES yaml)
    FIND_PATH(YAML_INCLUDE_DIR yaml.h ${PROJECT_SOURCE_DIR}/src/include/external/yaml)
    FIND_LIBRARY(YAML_LIBRARIES  ${YAML_NAMES} ${PROJECT_SOURCE_DIR}/lib)
ELSEIF (APPLE)  
    SET(YAML_NAMES libyaml.dylib)
    FIND_PATH(YAML_INCLUDE_DIR yaml.h ${PROJECT_SOURCE_DIR}/src/include/external/yaml)
    FIND_LIBRARY(YAML_LIBRARIES  ${YAML_NAMES} /usr/local/lib/)
ELSE ()  
    SET(YAML_NAMES libyaml.so) 
    FIND_PATH(YAML_INCLUDE_DIR yaml.h /usr/local/include)
    FIND_LIBRARY(YAML_LIBRARIES  ${YAML_NAMES} /usr/local/lib/)
ENDIF ()


# handle the QUIETLY and REQUIRED arguments and set YAML_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(yaml DEFAULT_MSG YAML_LIBRARIES YAML_INCLUDE_DIR)

MARK_AS_ADVANCED(YAML_LIBRARIES YAML_INCLUDE_DIR)
