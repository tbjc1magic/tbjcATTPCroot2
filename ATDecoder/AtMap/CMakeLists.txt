

set(sources
    AtTpcMap.cxx
    AtTpcProtoMap.cxx
    )
set(headers
    AtTpcMap.h
    AtTpcProtoMap.h
    )

#ROOT_GENERATE_DICTIONARY(G__AtMap ${headers} LinkDef.h)
add_library(AtMap  SHARED ${sources})# G__AtMap.cxx)
target_link_libraries(AtMap ${ROOT_LIBRARIES} XMLParser)
