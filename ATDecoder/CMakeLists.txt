include_directories(AtMap)
include_directories(GETDecoder2)

add_subdirectory(AtMap)
add_subdirectory(GETDecoder2)

file(GLOB sources *.cxx *.cc)
file(GLOB headers *.h *.hh)

ROOT_GENERATE_DICTIONARY(G__ATDecoder ${headers} )
add_library(ATDecoder  SHARED ${sources} G__ATDecoder.cxx)
target_include_directories(ATDecoder PUBLIC GETDecoder2)
target_include_directories(ATDecoder PUBLIC AtMap)

target_link_libraries(ATDecoder ${ROOT_LIBRARIES})
target_link_libraries(ATDecoder  GETDecoder2)
target_link_libraries(ATDecoder  AtMap)
