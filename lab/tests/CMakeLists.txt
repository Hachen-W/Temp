cmake_minimum_required(VERSION 3.10)
find_package(Catch2 2 REQUIRED)
# создание исполняемого файла для программы тестирования

add_compile_options(--coverage)
add_link_options(--coverage)

add_library(aura ../src/Aura.h ../src/Aura.cpp)
add_library(characteristics ../src/Characteristics.h ../src/Characteristics.cpp)
add_library(construction ../src/Construction.h ../src/Construction.cpp)
add_library(construction_enemy ../src/Construction_Enemy.h ../src/Construction_Enemy.cpp)
add_library(enemy ../src/Enemy.h ../src/Enemy.cpp)
add_library(entity ../src/Entity.h ../src/Entity.cpp)
add_library(landscape ../src/Landscape.h ../src/Landscape.cpp)

add_executable(func_testing testing.cpp)

target_link_libraries(func_testing aura characteristics construction construction_enemy enemy entity landscape Catch2::Catch2)
