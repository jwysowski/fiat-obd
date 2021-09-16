set(LIB_SOURCES
  libs/iaw16f/iaw16f.c
  libs/lang/pl.c
)

set(MAIN_SOURCES
  main.c
)

set(HEADERS 
  ${CONFIG_HEADER}
  libs/iaw16f/iaw16f.h
  libs/lang/pl.h
)

add_library(iaw16f STATIC ${LIB_SOURCES} ${HEADERS})

add_executable(main ${HEADERS} ${MAIN_SOURCES})
target_link_libraries(main iaw16f)

install(TARGETS iaw16f
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/libs/iaw16f/iaw16f.h DESTINATION include)
