set(ENTRY_POINT ${CMAKE_SOURCE_DIR}/src/main.cpp)


file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cpp)
list(REMOVE_ITEM CORE_SOURCES ${ENTRY_POINT})

add_library(LivcCore STATIC ${CORE_SOURCES})
target_compile_features(LivcCore PUBLIC cxx_std_23)
target_include_directories(LivcCore PUBLIC ${CMAKE_SOURCE_DIR}/src)

add_executable(livc ${ENTRY_POINT})
target_link_libraries(livc PRIVATE LivcCore)