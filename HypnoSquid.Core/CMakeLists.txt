add_library(hs-core SHARED
        src/SystemMainLoop.cc)
target_include_directories(hs-core PUBLIC include)
target_link_libraries(hs-core PUBLIC hs-engine)