
file(GLOB sources *.cxx *.cc)
file(GLOB headers *.h *.hh)

#ROOT_GENERATE_DICTIONARY(G__ATDigiPar ${headers} )
add_library(ATDigiPar  SHARED ${sources})# G__ATDigiPar.cxx)

target_link_libraries(ATDigiPar ${ROOT_LIBRARIES})
