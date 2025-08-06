cmake_minimum_required(VERSION 3.15)

# Nom du projet
project(ecs LANGUAGES CXX)

# Définit la version de C++ à utiliser
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Ajoute les répertoires include
include_directories(include)

# Crée une bibliothèque statique (changer le nom de "myecs" en "ecs")
add_library(ecs STATIC
    src/entity.cpp
    src/registry.cpp
)

# Ajoute les répertoires include au projet
target_include_directories(ecs
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/template
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Option pour activer les warnings
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(ecs PRIVATE -Wall -Wextra -pedantic)
endif()
