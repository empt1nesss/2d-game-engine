cmake_minimum_required(VERSION 3.22)
project(2d-engine)

file(GLOB_RECURSE SOURCE_FILES
  source/*.cpp
  source/*.c
)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(REQUIRED_SFML_LIBS
  sfml-system
  sfml-window
  sfml-graphics
  sfml-audio
  sfml-network
)
set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

include_directories(source)


option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
add_subdirectory(lib/sfml)
foreach(SFML_LIB ${REQUIRED_SFML_LIBS})
  target_compile_options(${SFML_LIB} PRIVATE "-w")
endforeach()

target_link_libraries(${PROJECT_NAME} PRIVATE
  ${REQUIRED_SFML_LIBS}
)

if(WIN32)
  list(FIND REQUIRED_SFML_LIBS sfml-audio SFML_AUDIO_INDEX)
  if(SFML_AUDIO_INDEX GREATER -1)
    add_custom_command(
      TARGET ${PROJECT_NAME}
      COMMENT "Copy OpenAL DLL"
      PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy
        ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
      VERBATIM
    )
  endif()
endif()

file(REMOVE_RECURSE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
if(EXISTS ${CMAKE_SOURCE_DIR}/assets)
  file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()
