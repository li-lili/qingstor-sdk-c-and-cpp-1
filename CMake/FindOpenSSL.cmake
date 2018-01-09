# - Find CURL
# Find the native CURL includes and library
#
#  CURL_INCLUDE_DIR  - where to find curl/curl.h, etc.
#  CURL_LIBRARIES    - List of libraries when using CURL.
#  CURL_FOUND        - True if CURL found.

IF (OPENSSL_INCLUDE_DIR AND OPENSSL_LIBRARIES)
  # Already in cache, be silent
  SET(OPENSSL_FIND_QUIETLY TRUE)
ENDIF (OPENSSL_INCLUDE_DIR AND OPENSSL_LIBRARIES)

IF (WIN32)  
    FIND_PATH(OPENSSL_INCLUDE_DIR ssl.h ${PROJECT_SOURCE_DIR}/src/include/external/openssl)   
    SET(CURL_NAMES libcurl)
    FIND_LIBRARY(OPENSSL_CRYPTO_LIBRARIES  "libeay32.lib" ${PROJECT_SOURCE_DIR}/lib)
    FIND_LIBRARY(OPENSSL_SSL_LIBRARIES  "ssleay32.lib" ${PROJECT_SOURCE_DIR}/lib)
ELSEIF (APPLE)  
	FIND_PATH(OPENSSL_INCLUDE_DIR ssl.h  /usr/local/include/openssl)  
    FIND_LIBRARY(OPENSSL_CRYPTO_LIBRARIES  "libcrypto.dylib" /usr/local/lib)
    FIND_LIBRARY(OPENSSL_SSL_LIBRARIES  "libssl.dylib" /usr/local/lib)
ELSE ()
	FIND_PATH(OPENSSL_INCLUDE_DIR ssl.h  /usr/local/include/openssl /usr/include/openssl)  
    FIND_LIBRARY(OPENSSL_CRYPTO_LIBRARIES  "libcrypto.so" /usr/local/lib)
    FIND_LIBRARY(OPENSSL_SSL_LIBRARIES  "libssl.so" /usr/local/lib)
ENDIF ()  


# handle the QUIETLY and REQUIRED arguments and set CURL_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(openssl DEFAULT_MSG OPENSSL_CRYPTO_LIBRARIES OPENSSL_INCLUDE_DIR)

MARK_AS_ADVANCED(OPENSSL_CRYPTO_LIBRARIES CURL_INCLUDE_DIR)
