set(sources
	ATHoughSpace.cc
	ATHoughSpaceCircle.cc
	ATHoughSpaceLine.cc
	ATHoughTask.cc
	ATMCMinimization.cc
	ATMinimization.cc)

set(headers
	ATHoughSpaceCircle.hh
	ATHoughSpace.hh
	ATHoughSpaceLine.hh
	ATHoughTask.hh
	ATMCMinimization.hh
	ATMinimization.hh)

add_library(ATHoughTask SHARED  ${sources})# G__ATHoughTask.cxx)

target_include_directories(ATHoughTask PUBLIC ${CMAKE_SOURCE_DIR}/ATDigiPar)
target_include_directories(ATHoughTask PUBLIC ${CMAKE_SOURCE_DIR}/ATDecoder)
target_include_directories(ATHoughTask PUBLIC ${CMAKE_SOURCE_DIR}/FairRoot)
target_include_directories(ATHoughTask PUBLIC ${CMAKE_SOURCE_DIR}/tbjcClonesArray)
target_link_libraries(ATHoughTask ${ROOT_LIBRARIES})
target_link_libraries(ATHoughTask FairRoot)
target_link_libraries(ATHoughTask ATDigiPar)
target_link_libraries(ATHoughTask ATDecoder)
