set(headers
    ATPSA.hh
    ATPSAProto.hh
    ATPSASimple2.hh
    ATPSASimple.hh
    ATPSATask.hh
    )

set(sources
    ATPSA.cc
    ATPSAProto.cc
    ATPSASimple2.cc
    ATPSASimple.cc
    ATPSATask.cc
    )

add_library(ATPSATask  SHARED ${sources})# G__ATPSATask.cxx)
target_include_directories(ATPSATask PUBLIC ${CMAKE_SOURCE_DIR}/ATDecoder)
target_include_directories(ATPSATask PUBLIC ${CMAKE_SOURCE_DIR}/ATDigiPar)
target_include_directories(ATPSATask PUBLIC ${CMAKE_SOURCE_DIR}/FairRoot)
target_link_libraries(ATPSATask ${ROOT_LIBRARIES})
target_link_libraries(ATPSATask Spectrum)
target_link_libraries(ATPSATask FairRoot)
target_link_libraries(ATPSATask ATDigiPar)
