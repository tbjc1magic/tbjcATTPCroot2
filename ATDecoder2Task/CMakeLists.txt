
file(GLOB sources  *.cc)
file(GLOB headers  *.hh)

#ROOT_GENERATE_DICTIONARY(G__ATDecoder2Task ${headers} )
add_library(ATDecoder2Task  SHARED ${sources})# G__ATDecoder2Task.cxx)

target_include_directories(GETDecoder2 PUBLIC ${PROJECT_SOURCE_DIR}/tbjcClonesArray)
target_include_directories(GETDecoder2 PUBLIC ${PROJECT_SOURCE_DIR}/FairRoot)
target_include_directories(GETDecoder2 PUBLIC ${PROJECT_SOURCE_DIR}/ATDecoder)
target_include_directories(GETDecoder2 PUBLIC ${PROJECT_SOURCE_DIR}/ATDigiPar)

target_link_libraries(ATDecoder2Task ${ROOT_LIBRARIES})
target_link_libraries(ATDecoder2Task FairRoot)
target_link_libraries(ATDecoder2Task ATDecoder)
target_link_libraries(ATDecoder2Task ATDigiPar)
