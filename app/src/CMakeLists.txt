cmake_minimum_required(VERSION 3.13)

set(NAME app)
set(TAGLIB_PATH "../3dparty/taglib/taglib")

find_library(TAGLIB libtag)
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
file(GLOB SOURCE *.cpp)

add_library(${NAME} STATIC ${SOURCE})
target_link_libraries(${NAME} Qt5::Core Qt5::Widgets tag)
