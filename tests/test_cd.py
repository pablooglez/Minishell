# tests para el comando `cd`
tests = [
    # Básicas
    "cd ~ && pwd",  # Cambio al directorio home
    "cd / && pwd",  # Cambio al directorio raíz
    "cd .. && pwd",  # Subir un nivel en el directorio
    "cd . && pwd",  # Permanecer en el mismo directorio

    # Directorios relativos
    "cd ./tests && pwd",  # Cambio a un directorio relativo
    "cd ../ && pwd",  # Subir un nivel y mostrar el directorio actual
    "cd .././../ && pwd",  # Uso combinado de rutas relativas

    # Directorios inválidos
    "cd invalid_dir && echo $?",  # Intentar cambiar a un directorio inexistente
    "cd /invalid_path && echo $?",  # Ruta inválida absoluta
    "cd /root && echo $?",  # Directorio al que posiblemente no se tenga acceso
    "cd tests/nonexistent_dir && echo $?",  # Directorio inexistente en una ruta relativa

    # Uso de `-` para volver al directorio anterior
    "cd /usr && cd - && pwd",  # Cambio a /usr y volver al directorio anterior
    "cd / && cd /usr && cd - && pwd",  # Cambio entre directorios y uso de `cd -`

    # Pruebas de variables de entorno
    "cd $HOME && pwd",  # Cambio al directorio definido por la variable $HOME
    "export TEST_DIR=/tmp && cd $TEST_DIR && pwd",  # Uso de variable de entorno personalizada
    "unset HOME && cd ~ && echo $?",  # Intentar usar `~` sin la variable HOME
    "cd $PWD && pwd",  # Uso de la variable $PWD

    # Pruebas con comillas
    "cd \"$HOME\" && pwd",  # Cambio al directorio home usando comillas dobles
    "cd '$HOME' && echo $?",  # Uso de comillas simples, no debería expandir la variable
    "cd \"\" && pwd",  # `cd` con argumento vacío
    "cd '' && pwd",  # `cd` con argumento vacío usando comillas simples

    # Pruebas con espacios y caracteres especiales
    "mkdir -p './test dir' && cd './test dir' && pwd",  # Directorio con espacios
    "cd './test dir' && cd .. && pwd",  # Salir de un directorio con espacios
    "mkdir -p 'dir-with-hyphen' && cd dir-with-hyphen && pwd",  # Directorio con guiones
    "mkdir -p 'dir_with_underscore' && cd dir_with_underscore && pwd",  # Directorio con guiones bajos

    # Casos extremos
    "cd ////// && pwd",  # Varios slashes seguidos, debe ser tratado como /
    "cd ~root && echo $?",  # Cambio al home de root, puede fallar si no hay permisos
    "cd /var/log/ && cd ../../ && pwd",  # Navegar hacia atrás desde un directorio profundo
    "cd /dev/null && echo $?",  # Intentar cambiar a un archivo en lugar de un directorio

    # Combinaciones de `cd` y otros comandos
    "cd && echo $PWD",  # Cambio al home y mostrar el directorio actual
    "cd /tmp && touch prueba.txt && cd - && ls /tmp/prueba.txt",  # Cambio temporal de directorio y volver
    "cd /usr/bin && cd /bin && cd - && cd -",  # Alternar entre dos directorios
    "cd / && (cd /usr && pwd) && pwd",  # Cambio temporal de directorio usando subshell
]
