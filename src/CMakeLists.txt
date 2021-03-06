CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(libqingstor_VERSION_MAJOR 0)
SET(libqingstor_VERSION_MINOR 0)
SET(libqingstor_VERSION_PATCH 1)
SET(libqingstor_VERSION_STRING "${libqingstor_VERSION_MAJOR}.${libqingstor_VERSION_MINOR}.${libqingstor_VERSION_PATCH}")
SET(libqingstor_VERSION_API 1)

GET_FILENAME_COMPONENT(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)

SET(libqingstor_ROOT_SOURCES_DIR ${PARENT_DIR}/src)

SET(libqingstor_INCLUDE_DIR ${PARENT_DIR}/include ${libqingstor_ROOT_SOURCES_DIR}/include)

IF(MSCV)
    SET(WS2_LIBRARIES Ws2_32.lib)
    SET(WINLDAP_LIBRARIES Wldap32.lib)
    # force to compile with W0
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W0")
ELSE() 
    SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wno-deprecated-declarations -g2 -ggdb")    
    SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wno-deprecated-declarations")  
ENDIF()

IF(ENABLE_DEBUG)
    SET(libqingstor_VERSION_STRING "${libqingstor_VERSION_STRING}d")
ENDIF(ENABLE_DEBUG)


CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/QsSdkOption.h.in" "${PARENT_DIR}/include/QsSdkOption.h")

AUTO_SOURCES(files "external/yaml/*.c" "RECURSE" "${CMAKE_CURRENT_SOURCE_DIR}")
LIST(APPEND libqingstor_SOURCES ${files})

AUTO_SOURCES(files "external/lib_json/*.cpp" "RECURSE" "${CMAKE_CURRENT_SOURCE_DIR}")
LIST(APPEND libqingstor_SOURCES ${files})

AUTO_SOURCES(files "config/*.cpp" "RECURSE" "${CMAKE_CURRENT_SOURCE_DIR}")
LIST(APPEND libqingstor_SOURCES ${files})

AUTO_SOURCES(files "http/*.cpp" "RECURSE" "${CMAKE_CURRENT_SOURCE_DIR}")
LIST(APPEND libqingstor_SOURCES ${files})

AUTO_SOURCES(files "utils/*.cpp" "RECURSE" "${CMAKE_CURRENT_SOURCE_DIR}")
LIST(APPEND libqingstor_SOURCES ${files})

AUTO_SOURCES(files "request/*.cpp" "RECURSE" "${CMAKE_CURRENT_SOURCE_DIR}")
LIST(APPEND libqingstor_SOURCES ${files})

AUTO_SOURCES(files "service/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}")
LIST(APPEND libqingstor_SOURCES ${files})

# if BUILD_C_STYLE_INTERFACE == ON, install header files with C style.
IF(BUILD_C_STYLE_INTERFACE)
AUTO_SOURCES(files "service/service_with_c_style/*.cpp" "RECURSE" "${CMAKE_CURRENT_SOURCE_DIR}")
LIST(APPEND libqingstor_SOURCES ${files})
ENDIF()

AUTO_SOURCES(files "signer/*.cpp" "RECURSE" "${CMAKE_CURRENT_SOURCE_DIR}")
LIST(APPEND libqingstor_SOURCES ${files})

AUTO_SOURCES(files "*.h" "${libqingstor_INCLUDE_DIR}")
SET(HEADER ${files})

AUTO_SOURCES(files "types/*.h" "${libqingstor_INCLUDE_DIR}")
SET(HEADER_TYPES ${files})

# if BUILD_C_STYLE_INTERFACE == ON, install header files with C style.
IF(BUILD_C_STYLE_INTERFACE)
AUTO_SOURCES(files "service_with_c_style/*.h" "${libqingstor_INCLUDE_DIR}")
SET(HEADER_CStyle ${files})

AUTO_SOURCES(files "service_with_c_style/types/*.h" "${libqingstor_INCLUDE_DIR}")
SET(HEADER_TYPES_CStyle ${files})
ENDIF()

ADD_LIBRARY(libqingstor-static STATIC ${libqingstor_SOURCES})
ADD_LIBRARY(libqingstor-shared SHARED ${libqingstor_SOURCES})

ADD_CUSTOM_COMMAND(
    TARGET libqingstor-shared libqingstor-static
    PRE_BUILD
    COMMAND echo ${libqingstor_VERSION_STRING} > version
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

INCLUDE_DIRECTORIES(${libqingstor_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${libqingstor_INCLUDE_DIR}/external)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${CURL_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${OPENSSL_INCLUDE_DIR})

TARGET_LINK_LIBRARIES(libqingstor-static ${CURL_LIBRARIES})
TARGET_LINK_LIBRARIES(libqingstor-shared ${CURL_LIBRARIES})

TARGET_LINK_LIBRARIES(libqingstor-static ${OPENSSL_SSL_LIBRARIES})
TARGET_LINK_LIBRARIES(libqingstor-shared ${OPENSSL_SSL_LIBRARIES})
TARGET_LINK_LIBRARIES(libqingstor-static ${OPENSSL_CRYPTO_LIBRARIES})
TARGET_LINK_LIBRARIES(libqingstor-shared ${OPENSSL_CRYPTO_LIBRARIES})

# in windows, curl depend on libs:Ws2_32.lib and Wldap32.lib
IF (WIN32)  
    TARGET_LINK_LIBRARIES(libqingstor-static ${WS2_LIBRARIES})
    TARGET_LINK_LIBRARIES(libqingstor-static ${WINLDAP_LIBRARIES})
    TARGET_LINK_LIBRARIES(libqingstor-shared ${WS2_LIBRARIES})
    TARGET_LINK_LIBRARIES(libqingstor-shared ${WINLDAP_LIBRARIES})
ENDIF()

SET_TARGET_PROPERTIES(libqingstor-static PROPERTIES OUTPUT_NAME "qingstor")
SET_TARGET_PROPERTIES(libqingstor-shared PROPERTIES OUTPUT_NAME "qingstor")

SET_TARGET_PROPERTIES(libqingstor-shared PROPERTIES 
    VERSION ${libqingstor_VERSION_MAJOR}.${libqingstor_VERSION_MINOR}.${libqingstor_VERSION_PATCH} 
    SOVERSION ${libqingstor_VERSION_API})

# install lib files.
INSTALL(TARGETS libqingstor-static libqingstor-shared
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

# install header files.
INSTALL(FILES ${HEADER} DESTINATION include/qingstor)
INSTALL(FILES ${HEADER_TYPES} DESTINATION include/qingstor/types)

# if BUILD_C_STYLE_INTERFACE == ON, install header files with C style.
IF(BUILD_C_STYLE_INTERFACE)
INSTALL(FILES ${HEADER_CStyle} DESTINATION include/qingstor/service_with_c_style)
INSTALL(FILES ${HEADER_TYPES_CStyle} DESTINATION include/qingstor/service_with_c_style/types)
ENDIF()
        
SET(libqingstor_SOURCES ${libqingstor_SOURCES} PARENT_SCOPE)
SET(libqingstor_PLATFORM_HEADER_DIR ${CMAKE_CURRENT_BINARY_DIR} PARENT_SCOPE)
SET(libqingstor_ROOT_SOURCES_DIR ${libqingstor_ROOT_SOURCES_DIR} PARENT_SCOPE)
SET(libqingstor_VERSION_STRING ${libqingstor_VERSION_STRING} PARENT_SCOPE)
