#Especificar donde se encuentran los ficheros cpp a compilar
target_sources(facadeSFML PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/ArrayVertices.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Evento.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Fuente.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Reloj.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Sprite.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Texto.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Textura.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Tiempo.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/VentanaRender.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Vista.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Buffer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Sonido.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Musica.cpp"

    )

#Y donde se encuentran los .h
target_include_directories(facadeSFML PUBLIC 
"${CMAKE_CURRENT_SOURCE_DIR}"
)

