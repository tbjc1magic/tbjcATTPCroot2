
file(GLOB sources *.cxx *.cc)
file(GLOB headers *.h *.hh)

#ROOT_GENERATE_DICTIONARY(G__FairRoot ${headers} )
add_library(FairRoot  SHARED ${sources})# G__FairRoot.cxx)

target_link_libraries(FairRoot ${ROOT_LIBRARIES})
target_include_directories(FairRoot PUBLIC ${PROJECT_SOURCE_DIR}/tbjcClonesArray)
