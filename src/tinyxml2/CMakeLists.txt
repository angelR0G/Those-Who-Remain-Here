#Especificar donde se encuentran los ficheros cpp a compilar
target_sources(tinyXML PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/LectorXML.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/tinyxml2.cpp"
)

#Y donde se encuentran los .h
target_include_directories(tinyXML PUBLIC 
"${CMAKE_CURRENT_SOURCE_DIR}"
)

