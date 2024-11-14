# tests para el comando pwd
tests = [
    # Pruebas básicas
    "pwd",  # Comando básico para obtener el directorio actual

    # Cambiar de directorio y verificar
    "cd / && pwd",  # Cambiar al directorio raíz y mostrar
    "cd .. && pwd",  # Subir un nivel y mostrar
    "cd . && pwd",  # Quedarse en el mismo directorio y mostrar
    "cd ~ && pwd",  # Ir al directorio HOME y mostrar

    # Pruebas con directorios relativos y absolutos
    "cd /usr && pwd",  # Cambiar al directorio /usr
    "cd /tmp && pwd",  # Cambiar al directorio /tmp
    "cd ../../ && pwd",  # Subir dos niveles
    "cd /usr/local/bin && pwd",  # Cambiar al directorio específico

    # Uso con redirección
    "pwd > output.txt && cat output.txt",  # Guardar la salida de pwd en un archivo y mostrarlo
    "pwd >> output.txt && cat output.txt",  # Añadir la salida de pwd al archivo existente

    # Uso con tuberías
    "pwd | grep '/'",  # Filtrar la salida para mostrar solo si contiene '/'
    "pwd | wc -l",  # Contar líneas de la salida (debe ser 1)

    # Pruebas con variables de entorno
    "export PWD_OVERRIDE='/custom/path' && echo $PWD_OVERRIDE && pwd",  # Comprobar si pwd utiliza la variable PWD
    "cd /var && export NEW_DIR=$PWD && echo $NEW_DIR && pwd",  # Usar PWD para asignar a una nueva variable

    # Pruebas con enlaces simbólicos
    "cd /tmp && ln -s /usr tmp_link && cd tmp_link && pwd",  # Crear enlace simbólico y cambiar de directorio
    "cd /tmp && mkdir test_dir && cd test_dir && pwd",  # Crear un directorio y cambiar
    "cd /tmp && rm -rf test_dir && pwd",  # Eliminar el directorio creado y mostrar PWD

    # Pruebas extremas
    "cd ../../../../../../../../ && pwd",  # Ir lo más arriba posible en el sistema de archivos
    "cd /does_not_exist && pwd",  # Intentar cambiar a un directorio que no existe
    "cd / && pwd && cd /root && pwd",  # Probar acceso a un directorio restringido

    # Uso con comillas y espacios
    "cd '/usr/local/bin' && pwd",  # Directorio con comillas simples
    'cd "/usr/local/bin" && pwd',  # Directorio con comillas dobles
    "cd '/tmp/test dir' && pwd",  # Directorio con espacio en el nombre

    # Casos de error y manejo de errores
    "pwd invalid_argument",  # Comando pwd con argumento inválido
    "cd && pwd",  # Comportamiento de pwd al cambiar al HOME
    "cd . && pwd && cd .. && pwd",  # Secuencia de cambios de directorio
    "cd /usr && pwd && cd /non_existent_dir && pwd",  # Cambio a un directorio inexistente
    "cd /tmp && unset PWD && pwd",  # Probar comportamiento al eliminar PWD de las variables de entorno

    # Pruebas adicionales con export y unset
    "export CURRENT_DIR=$(pwd) && echo $CURRENT_DIR",  # Usar pwd en combinación con export
    "unset PWD && pwd",  # Eliminar PWD y ejecutar pwd
    "cd / && export NEW_PWD=$(pwd) && echo $NEW_PWD",  # Usar pwd con export para asignar un nuevo valor

    # Pruebas de comportamiento en shell subshell
    "(cd /usr && pwd)",  # Uso de pwd en subshell
    "(cd /tmp && pwd && cd /usr && pwd)",  # Secuencia de pwd en subshell
]
