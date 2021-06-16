add_library(saker saker.hpp saker/zone.hpp saker/prompt.hpp saker/content.hpp saker/icon.hpp)

add_library(rang rang.hpp)

add_library(environment environment.hpp saker/transforming.hpp saker/vert.hpp)

target_link_libraries(saker rang environment)

set_target_properties(saker PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(rang PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(environment PROPERTIES LINKER_LANGUAGE CXX)
