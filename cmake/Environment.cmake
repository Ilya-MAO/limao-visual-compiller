if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

if (MSVC)
  add_compile_options($<$<CONFIG:Release>:/O2 /GL>)
  add_link_options($<$<CONFIG:Release>:/LTCG>)

  add_custom_command(TARGET LimaoVisualCompiller POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_RUNTIME_DLLS:livc>
    $<TARGET_FILE_DIR:livc>
    COMMAND_EXPAND_LISTS
  )
else()
  add_compile_options($<$<CONFIG:Release>:-O3>)
  add_link_options($<$<CONFIG:Release>:-s>)
endif()