add_library(hs-engine SHARED
        src/World.cc
        src/Engine.cc
        src/EntitySet.cc
        src/ComponentStore.cc include/HypnoSquid/utility.hh)
target_include_directories(hs-engine PUBLIC include)