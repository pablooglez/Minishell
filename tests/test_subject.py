# tests para verificar el cumplimiento del enunciado
tests = [
    # 1. Mostrar entrada mientras espera un comando nuevo
    "echo Hola Mundo",  # Verificar si se muestra la entrada y se ejecuta el comando

    # 2. Historial funcional
    "echo prueba_historial && history | tail -n 1",  # Comprobar si el comando se guarda en el historial

    # 3. Ejecución de ejecutables usando PATH
    "ls",  # Ejecutable usando PATH
    "/bin/ls",  # Ejecutable usando ruta absoluta
    "./minishell",  # Ejecutable usando ruta relativa (si se encuentra en el directorio actual)

    # 4. Señales (Ctrl-C, Ctrl-D, Ctrl-\)
    # Estas pruebas requieren interacción manual:
    # "Ctrl-C debería interrumpir la ejecución y mostrar una nueva línea de entrada",
    # "Ctrl-D debería terminar el shell",
    # "Ctrl-\ no debería hacer nada",

    # 5. Manejo de comillas
    "echo 'texto entre comillas simples'",  # No expandir metacaracteres
    'echo "texto entre comillas dobles con $HOME"',  # Expandir $HOME dentro de comillas dobles

    # 6. Redirecciones
    "echo 'hola' > test_output.txt && cat test_output.txt",  # Redirección de output
    "cat < test_output.txt",  # Redirección de input
    "echo 'apendiendo texto' >> test_output.txt && cat test_output.txt",  # Redirección con append
    "cat << EOF\nhola\nEOF",  # Here document

    # 7. Pipes
    "echo 'pipe test' | grep 'pipe'",  # Uso de pipes
    "ls | wc -l",  # Conectar output de ls al input de wc

    # 8. Expansión de variables
    "export VAR1='expansión' && echo $VAR1",  # Expansión de variables
    "echo $?",  # Expansión del último código de salida

    # 9. Built-ins
    "echo 'hola mundo'",  # Built-in echo
    "echo -n 'sin salto de línea'",  # Opción -n en echo
    "pwd",  # Built-in pwd
    "export TEST_VAR=123 && env | grep TEST_VAR",  # Built-in export
    "unset TEST_VAR && env | grep TEST_VAR",  # Built-in unset
    "env",  # Built-in env
    "exit 42",  # Built-in exit con código de salida

    # 10. Manejo de errores y comportamientos no solicitados
    "echo 'comillas sin cerrar",  # Error de comillas sin cerrar
    "ls ; echo 'comando no válido'",  # Punto y coma no soportado
    "echo \\ ",  # Backslash no soportado
    "export VAR@=123 && echo $VAR@",  # Nombre de variable no válido
    "cd /invalido && echo $?",  # Cambio de directorio inválido
    "export PATH='/invalido' && ls && unset PATH",  # PATH inválido
]

# Instrucciones:
# 1. Guarda este archivo como `test_enunciado.py` en tu carpeta `./tests/`.
# 2. Ejecuta las pruebas usando tu comparador:
#    - Para ejecutar solo estas pruebas: `run test_enunciado.py`.
#    - Para ejecutar todas las pruebas: `run all`.
