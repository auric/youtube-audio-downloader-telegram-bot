cmake_minimum_required(VERSION 3.14)

project(audio-yt-bot LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost 1.66 REQUIRED)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
else()
  message(FATAL "${CMAKE_SOURCE_DIR}/../include")
endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS
    Core
    Network
)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
    Core
    Network
)

add_subdirectory(3rdparty/libtelegram)

add_executable(audio-yt-bot
  src/main.cpp
  src/item.h
  src/resolver.h
  src/youtubefinder.h
  
  src/resolver.cpp
  src/youtubefinder.cpp
  src/downloader.h src/downloader.cpp
  src/uploader.h src/uploader.cpp
)
target_link_libraries(audio-yt-bot PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Network
    Threads::Threads
    OpenSSL::Crypto
    OpenSSL::SSL
)
target_include_directories(audio-yt-bot PRIVATE
    "3rdparty/libtelegram/include"
    ${OPENSSL_INCLUDE_DIR}
)
if(WIN32)
    target_compile_definitions(audio-yt-bot PRIVATE
        TELEGRAM_NO_SIGNAL_HANDLER)
endif()

include(GNUInstallDirs)
install(TARGETS audio-yt-bot
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
