add_library(hs-engine SHARED
        src/World.cc
        src/Engine.cc
        src/ComponentStore.cc include/HypnoSquid/utility.hh
        src/EntitySet.cc
        include/HypnoSquid/IEntityCollection.hh)
target_include_directories(hs-engine PUBLIC include)