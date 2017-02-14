
file(GLOB sources  *.cc)
file(GLOB headers  *.hh)

add_library(ATAnalysisTask  SHARED ${sources})
target_link_libraries(ATAnalysisTask ${ROOT_LIBRARIES})
target_link_libraries(ATAnalysisTask FairRoot)
target_link_libraries(ATAnalysisTask ATDigiPar)
target_link_libraries(ATAnalysisTask ATHoughTask)

target_include_directories(ATAnalysisTask PUBLIC ${CMAKE_SOURCE_DIR}/ATDigiPar)
target_include_directories(ATAnalysisTask PUBLIC ${CMAKE_SOURCE_DIR}/ATDecoder)
target_include_directories(ATAnalysisTask PUBLIC ${CMAKE_SOURCE_DIR}/FairRoot)
target_include_directories(ATAnalysisTask PUBLIC ${CMAKE_SOURCE_DIR}/tbjcClonesArray)
target_include_directories(ATAnalysisTask PUBLIC ${CMAKE_SOURCE_DIR}/ATHoughTask)
