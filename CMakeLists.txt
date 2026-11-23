cmake_minimum_required(VERSION 3.20)

project(MyTracker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
cpr
GIT_REPOSITORY https://github.com/libcpr/cpr.git
GIT_TAG 3139b2682f0b1d6af8c7ae48cf400f9d9988deec
)
set(CPR_CURL_USE_LIBPSL OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(
json
GIT_REPOSITORY https://github.com/nlohmann/json.git
 GIT_TAG v3.12.0
)
FetchContent_MakeAvailable(json)

add_executable(MyTracker WIN32 main.cpp sqlite3.c)
target_link_libraries(MyTracker PRIVATE cpr::cpr nlohmann_json::nlohmann_json)
target_link_libraries(MyTracker PRIVATE cpr::cpr)
