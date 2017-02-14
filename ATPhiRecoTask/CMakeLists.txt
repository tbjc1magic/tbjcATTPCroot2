
set(sources
    ATPhiReco.cc
    ATPhiRecoSimple.cc
    ATPhiRecoTask.cc
    ATPhiRecoTriple.cc
    )
set(headers
    ATPhiReco.hh
    ATPhiRecoSimple.hh
    ATPhiRecoTask.hh
    ATPhiRecoTriple.hh
    )
add_library(ATPhiRecoTask  SHARED ${sources})

target_include_directories(ATPhiRecoTask PUBLIC ${CMAKE_SOURCE_DIR}/ATDigiPar)
target_include_directories(ATPhiRecoTask PUBLIC ${CMAKE_SOURCE_DIR}/ATDecoder)
target_include_directories(ATPhiRecoTask PUBLIC ${CMAKE_SOURCE_DIR}/FairRoot)
target_include_directories(ATPhiRecoTask PUBLIC ${CMAKE_SOURCE_DIR}/tbjcClonesArray)
target_link_libraries(ATPhiRecoTask ${ROOT_LIBRARIES})
target_link_libraries(ATPhiRecoTask FairRoot)
target_link_libraries(ATPhiRecoTask ATDigiPar)
target_link_libraries(ATPhiRecoTask ATDecoder)
