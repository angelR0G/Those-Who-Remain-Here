cmake_minimum_required(VERSION 3.1)
project(MiJuego)

#Añadir tantos subdirectorios como se necesiten
#En cada subdirectorio debe haber un fichero CMakeLists.txt
#Habitualmente cada subdirectorio determinará una libreria 
add_library(facadeSFML STATIC)
add_subdirectory(fachada)
add_library(tinyXML STATIC)
add_subdirectory(tinyxml2)
add_library(archivos STATIC)
add_subdirectory(archivos)

#Opciones a pasar al compilador de C++
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall -Wextra")
#Se desea generar un ejecutable, se especifica donde está el main
#y ficheros que aporten código que no sean librerias o subcarpetas 
add_executable(MiJuego Juego.cpp)

#Añadir todos los objetos (ficheros o) que requiera el ejecutable. 
#Habitualmente todas las librerias definidas mediante subcarpetas
#aportaran código
target_link_libraries(MiJuego facadeSFML)
target_link_libraries(MiJuego tinyXML)
target_link_libraries(MiJuego archivos)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
#Para usar SFML
find_package(SFML REQUIRED system window graphics network audio)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(MiJuego ${SFML_LIBRARIES})
endif()

