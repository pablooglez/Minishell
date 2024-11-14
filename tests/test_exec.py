# test_exec.py

tests = [
    # Pruebas para el built-in `echo`
    "echo Hello World",  # Comando básico
    "echo -n No newline here",  # Sin salto de línea
    "echo 'This is a test'",  # Con comillas simples
    "echo \"Double quoted text\"",  # Con comillas dobles
    "echo -n 'Nested \"quotes\" inside'",  # Anidación de comillas

    # Pruebas para el built-in `cd`
    "cd / && pwd",  # Cambio a raíz
    "cd .. && pwd",  # Cambio al directorio superior
    "cd /tmp && pwd",  # Cambio a un directorio específico
    "cd invalid_directory",  # Directorio no existente (debe manejar error)
    "cd && pwd",  # Cambio al directorio home
    "cd -",  # Cambio al directorio anterior
    "cd . && pwd",  # Cambio al directorio actual
    "cd / && cd -",  # Cambio entre directorios

    # Pruebas para el built-in `pwd`
    "pwd",  # Mostrar el directorio actual
    "cd /tmp && pwd",  # Cambio de directorio y mostrar pwd
    "cd / && pwd",  # Cambio a raíz y mostrar pwd
    "cd . && pwd",  # Directorio actual

    # Pruebas para el built-in `export`
    "export MY_VAR=123 && echo $MY_VAR",  # Asignar y mostrar variable
    "export USER_NAME='test_user' && echo $USER_NAME",  # Asignar variable con comillas
    "export PATH='/usr/bin:/bin' && echo $PATH",  # Asignar a PATH
    "export TEST_VAR && echo $TEST_VAR",  # Exportar sin asignar valor (debe estar vacío)
    "export INVALID=123 INVALID2=456 && echo $INVALID $INVALID2",  # Múltiples variables

    # Pruebas para el built-in `unset`
    "export TO_REMOVE=delete_me && unset TO_REMOVE && echo $TO_REMOVE",  # Eliminar variable
    "unset NON_EXISTENT_VAR && echo 'No error for non-existent var'",  # Eliminar variable inexistente
    "export TEMP_VAR=123 && unset TEMP_VAR && export TEMP_VAR && echo $TEMP_VAR",  # Reasignar tras eliminar

    # Pruebas para el built-in `env`
    "env | grep USER",  # Mostrar variables de entorno y buscar USER
    "export NEW_VAR=test_value && env | grep NEW_VAR",  # Añadir variable y verificar en env
    "unset PATH && env | grep PATH",  # Eliminar PATH y verificar env (no debe mostrar PATH)

    # Pruebas para el built-in `exit`
    "exit",  # Salir del shell
    "exit 0",  # Salir con código 0
    "exit 1",  # Salir con código 1
    "exit 255",  # Salir con código 255
    "exit 999",  # Código de salida fuera de rango (debe manejar error)
    "exit invalid_arg",  # Argumento no numérico (debe manejar error)

    # Pruebas de ejecución de comandos externos
    "/bin/ls",  # Comando absoluto
    "ls -la",  # Comando con argumentos
    "grep 'root' /etc/passwd",  # Comando con redirección
    "cat /etc/passwd | grep 'bash'",  # Uso de pipe
    "echo 'Test file' > test_exec.txt && cat test_exec.txt",  # Redirección de salida a archivo
    "rm -f test_exec.txt",  # Eliminar archivo creado

    # Pruebas combinadas de ejecución
    "echo 'Hello' && echo 'World'",  # Comandos encadenados con AND
    "echo 'Failing' || echo 'Recovered'",  # Comandos encadenados con OR
    "ls invalid_dir || echo 'Directory not found'",  # Manejo de error con OR
    "mkdir test_dir && cd test_dir && touch file.txt && ls && cd .. && rm -r test_dir",  # Crear y eliminar directorio
    "export VAR1=Hello && echo $VAR1 && unset VAR1 && echo $VAR1",  # Asignar, mostrar y eliminar variable

    # Pruebas con comandos no válidos
    "invalid_command",  # Comando no existente
    "/bin/invalid_command",  # Ruta absoluta a comando no existente
    "echo $? && invalid_command && echo $?",  # Verificar código de salida tras fallo
    "ls | invalid_command | grep 'test'",  # Comando no existente en pipe
    "export && exit",  # Export seguido de exit

    # Casos límite y edge cases
    "echo ''",  # Argumento vacío
    "echo \"$HOME\"",  # Expansión de variable en comillas dobles
    "echo '$HOME'",  # No expansión de variable en comillas simples
    "export VAR_WITH_SPACE='Value with space' && echo $VAR_WITH_SPACE",  # Variable con espacios
    "export EMPTY_VAR='' && echo $EMPTY_VAR",  # Variable vacía
    "unset && export UNSET_TEST=123 && echo $UNSET_TEST",  # unset sin argumento
]
