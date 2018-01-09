# - Find CURL
# Find the native CURL includes and library
#
#  CURL_INCLUDE_DIR  - where to find curl/curl.h, etc.
#  CURL_LIBRARIES    - List of libraries when using CURL.
#  CURL_FOUND        - True if CURL found.

IF (CURL_INCLUDE_DIR AND CURL_LIBRARIES)
  # Already in cache, be silent
  SET(CURL_FIND_QUIETLY TRUE)
ENDIF (CURL_INCLUDE_DIR AND CURL_LIBRARIES)

IF (WIN32)  
    SET(CURL_NAMES libcurl)
    FIND_PATH(CURL_INCLUDE_DIR curl/curl.h ${PROJECT_SOURCE_DIR}/src/include/external)   
    FIND_LIBRARY(CURL_LIBRARIES  ${CURL_NAMES} ${PROJECT_SOURCE_DIR}/lib)
ELSEIF (APPLE)
    SET(CURL_NAMES libcurl.dylib)   
    FIND_PATH(CURL_INCLUDE_DIR curl/curl.h  /usr/local/curl/lib) 
    FIND_LIBRARY(CURL_LIBRARIES  ${CURL_NAMES} /usr/local/lib)
ELSE ()
    SET(CURL_NAMES libcurl.so) 
    FIND_PATH(CURL_INCLUDE_DIR curl/curl.h  /usr/local/curl/lib) 
    FIND_LIBRARY(CURL_LIBRARIES  ${CURL_NAMES} /usr/local/lib)
ENDIF ()  

# handle the QUIETLY and REQUIRED arguments and set CURL_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(curl DEFAULT_MSG CURL_LIBRARIES CURL_INCLUDE_DIR)

MARK_AS_ADVANCED(CURL_LIBRARIES CURL_INCLUDE_DIR)
