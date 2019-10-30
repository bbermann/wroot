cmake_minimum_required(VERSION 3.15)

project(wroot CXX)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
set(CMAKE_CXX_STANDARD 20)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${PROJECT_SOURCE_DIR}/src/3rdParty")
include_directories("${PROJECT_SOURCE_DIR}/src/3rdParty/LuaJIT/src")
include_directories("${PROJECT_SOURCE_DIR}/src/3rdParty/nlohmann/json/include") # <nlohmann/json/...>

link_directories("${CMAKE_SOURCE_DIR}/src/3rdParty/LuaJIT/src")

set(SOURCES
        src/main.cpp
        src/include/helper/LuaScript.cpp
        src/include/helper/EnvironmentBuilder.cpp
        src/include/core/Core.cpp
        src/include/network/http/Response.cpp
        src/include/core/HttpServer.cpp
        src/include/helper/FileHelper.cpp
        src/include/helper/UrlRewriter.cpp
        src/include/library/CustomLibrary.cpp
        src/include/library/FileLibrary.cpp
        src/sample_app/HttpSample.cpp
        src/include/type/Process.cpp
        src/include/type/String.cpp
        src/include/type/Timer.cpp
        src/include/library/EvoLibrary/RandomNum.cpp)

set(HEADERS
        src/include/core/Core.hpp
        src/include/network/http/Request.hpp
        src/include/network/http/Response.hpp
        src/include/core/HttpServer.hpp
        src/include/helper/LuaScript.hpp
        src/include/helper/EnvironmentBuilder.hpp
        src/include/helper/FileHelper.hpp
        src/include/helper/TestHelper.hpp
        src/include/helper/ValidationHelper.hpp
        src/include/helper/UrlRewriter.hpp
        src/include/library/CustomLibrary.hpp
        src/include/library/FileLibrary.hpp
        src/include/type/Process.hpp
        src/include/type/String.hpp
        src/include/type/Timer.hpp
        src/sample_app/HttpSample.hpp
        src/include/exceptions/http/response/NotFound.hpp
        src/include/exceptions/http/response/Forbidden.hpp
        src/include/exceptions/http/response/InternalServerError.hpp
        src/include/exceptions/http/response/Unauthorized.hpp
        src/include/exceptions/http/request/KeyNotFound.hpp
        src/include/exceptions/http/request/FailedReceivingData.hpp
        src/include/exceptions/lua/LuaScriptException.hpp
        src/include/network/Connection.cpp
        src/include/network/Connection.hpp
        src/include/network/ConnectionManager.cpp
        src/include/network/ConnectionManager.hpp
        src/include/network/http/RequestHandler.cpp
        src/include/network/http/RequestHandler.hpp
        src/include/network/http/KeyValuePair.hpp
        src/include/network/http/MimeTypes.cpp
        src/include/network/http/MimeTypes.hpp
        src/include/network/http/RequestParser.cpp
        src/include/network/http/RequestParser.hpp
        src/include/helper/Chronometer.hpp src/include/library/EvoLibrary.cpp src/include/library/EvoLibrary.hpp src/include/library/EvoLibrary/Individual.cpp src/include/library/EvoLibrary/Individual.hpp src/include/library/EvoLibrary/RandomNum.hpp)

add_executable(wroot ${SOURCES} ${HEADERS})

## Link libraries
target_link_libraries(wroot stdc++ stdc++fs pthread z luajit ${CMAKE_DL_LIBS})