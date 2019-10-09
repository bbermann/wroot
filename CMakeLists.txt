cmake_minimum_required(VERSION 3.15)

project(wroot CXX)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
set(CMAKE_CXX_STANDARD 20)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${PROJECT_SOURCE_DIR}/src/3rdParty")
include_directories("${PROJECT_SOURCE_DIR}/src/3rdParty/luacpp")
include_directories("${PROJECT_SOURCE_DIR}/src/3rdParty/asio/asio/include")

link_directories(${CMAKE_SOURCE_DIR}/src/3rdParty/lua)

set(SOURCES
        src/main.cpp
        src/include/helper/Base64.cpp
        src/3rdParty/zlib/ZLib.cpp
        src/include/helper/LuaScript.cpp
        src/include/helper/EnvironmentBuilder.cpp
        src/include/core/Core.cpp
        src/include/core/HttpRequest.cpp
        src/include/core/HttpResponse.cpp
        src/include/core/HttpServer.cpp
        src/include/helper/Crypto.cpp
        src/include/helper/FileHelper.cpp
        src/include/helper/FunctionMap.cpp
        src/include/helper/UrlRewriter.cpp
        src/include/library/CustomLibrary.cpp
        src/include/library/FileLibrary.cpp
        src/include/library/RouterLibrary.cpp
        src/include/type/Process.cpp
        src/include/type/String.cpp
        src/include/type/Timer.cpp
        src/sample_app/HttpSample.cpp)

set(HEADERS
        src/include/helper/Base64.hpp
        src/3rdParty/luacpp/lua.hpp
        src/3rdParty/pthreads/pthread.h
        src/3rdParty/pthreads/sched.h
        src/3rdParty/pthreads/semaphore.h
        src/3rdParty/zlib/include/zconf.h
        src/3rdParty/zlib/include/zlib.h
        src/3rdParty/zlib/ZLib.hpp
        src/include/core/Core.hpp
        src/include/core/HttpRequest.hpp
        src/include/core/HttpResponse.hpp
        src/include/core/HttpServer.hpp
        src/include/helper/LuaScript.hpp
        src/include/helper/EnvironmentBuilder.hpp
        src/include/helper/Crypto.hpp
        src/include/helper/ExampleHelper.hpp
        src/include/helper/FileHelper.hpp
        src/include/helper/FunctionMap.hpp
        src/include/helper/TestHelper.hpp
        src/include/helper/ValidationHelper.hpp
        src/include/helper/UrlRewriter.hpp
        src/include/library/CustomLibrary.hpp
        src/include/library/FileLibrary.hpp
        src/include/library/RouterLibrary.hpp
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
        src/include/exceptions/lua/LuaScriptException.hpp)

add_executable(wroot ${SOURCES} ${HEADERS})

## Link libraries
target_link_libraries(wroot stdc++ stdc++fs pthread z lua ${CMAKE_DL_LIBS})