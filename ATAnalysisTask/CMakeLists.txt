
file(GLOB sources  *.cc)
file(GLOB headers  *.hh)

ROOT_GENERATE_DICTIONARY(G__ATAnalysisTask ${headers})
add_library(ATAnalysisTask  SHARED ${sources} G__ATAnalysisTask.cxx)

target_link_libraries(ATAnalysisTask ${ROOT_LIBRARIES})
target_link_libraries(ATAnalysisTask FairRoot)
target_link_libraries(ATAnalysisTask ATDigiPar)
