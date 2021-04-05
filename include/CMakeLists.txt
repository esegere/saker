add_library(saker saker.hpp)

add_library(rang rang.hpp)

target_link_libraries(saker rang)

set_target_properties(rang PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(saker PROPERTIES LINKER_LANGUAGE CXX)