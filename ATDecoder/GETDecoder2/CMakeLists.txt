

file(GLOB sources
    GETBasicFrame.cc
    GETBasicFrameHeader.cc
    GETCoboFrame.cc
    GETDecoder2.cc
    GETFileChecker.cc
    GETFrameInfo.cc
    GETHeaderBase.cc
    GETLayeredFrame.cc
    GETLayerHeader.cc
    GETMath2.cc
    GETTopologyFrame.cc
    )
file(GLOB headers
    GETBasicFrameHeader.hh
    GETBasicFrame.hh
    GETCoboFrame.hh
    GETDecoder2.hh
    GETFileChecker.hh
    GETFrameInfo.hh
    GETHeaderBase.hh
    GETLayeredFrame.hh
    GETLayerHeader.hh
    GETMath2.hh
    GETTopologyFrame.hh
    )

add_library(GETDecoder2  SHARED ${sources})# G__GETDecoder.cxx)
target_link_libraries(GETDecoder2 ${ROOT_LIBRARIES})
target_include_directories(GETDecoder2 PUBLIC ${PROJECT_SOURCE_DIR}/tbjcClonesArray)
