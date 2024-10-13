set(HEADERS
  ${CMAKE_CURRENT_LIST_DIR}/solitare.hpp
  ${CMAKE_CURRENT_LIST_DIR}/solver.hpp
)

set(SOURCES
  ${CMAKE_CURRENT_LIST_DIR}/solitare.cpp
  ${CMAKE_CURRENT_LIST_DIR}/solver.cpp
)


add_sources("${HEADERS}" "${SOURCES}")
