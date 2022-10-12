cmake_minimum_required(VERSION 3.23)
project(CoolProj)

set(CMAKE_CXX_STANDARD 20)

add_library(hs-engine SHARED 
    HypnoSquid.Engine/src/World.cc
    HypnoSquid.Engine/src/Engine.cc 
    HypnoSquid.Engine/src/EntitySet.cc
    HypnoSquid.Engine/src/ComponentStore.cc)
target_include_directories(hs-engine PUBLIC HypnoSquid.Engine/include)

add_library(hs-core SHARED
    HypnoSquid.Core/src/SystemMainLoop.cc)
target_include_directories(hs-core PUBLIC HypnoSquid.Core/include)
target_link_libraries(hs-core PUBLIC hs-engine)

add_executable(hs-game HypnoSquid.Game/src/main.cc)

target_link_libraries(hs-game PRIVATE hs-core hs-engine)