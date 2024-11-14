# hoja_correccion.py

# Lista de pruebas para evaluación
tests = [
    # Prueba 1: Compilación y Makefile
    "make -n",  # Verificar compilación con flags -Wall -Wextra -Werror
    "make",  # Compilar minishell
    "make re",  # Verificar que no haga relink

    # Prueba 2: Ejecución simple
    "/bin/ls",  # Comando absoluto
    "/bin/echo 'Hello World'",  # Comando con argumentos
    "   ",  # Espacios vacíos
    "\t",  # Tabulaciones

    # Prueba 3: Manejo de señales
    "echo 'Escribe algo y presiona Ctrl-C'",  # Prueba Ctrl-C
    "echo 'Escribe algo y presiona Ctrl-D'",  # Prueba Ctrl-D
    "echo 'Escribe algo y presiona Ctrl-\\'",  # Prueba Ctrl-\

    # Prueba 4: Argumentos
    "/bin/ls -l /usr/bin",  # Comando con argumentos
    "/bin/echo Hola Mundo",  # Comando con múltiples argumentos

    # Prueba 5: Built-in echo
    "echo 'Probando echo'",  # Comando echo
    "echo -n 'Sin salto de línea'",  # Echo con -n

    # Prueba 6: Built-in exit
    "exit",  # Comando exit
    "exit 42",  # Exit con código de salida
    "echo $?",  # Verificar el código de salida

    # Prueba 7: Expansión de variables
    "export TEST_VAR=123 && echo $TEST_VAR",  # Expansión de variable
    "echo $?",  # Expansión de código de salida
    "expr $? + $?",  # Operación con código de salida

    # Prueba 8: Manejo de comillas
    "echo 'texto con comillas simples'",  # Comillas simples
    'echo "texto con comillas dobles y $USER"',  # Comillas dobles con variable

    # Prueba 9: Built-in env
    "env",  # Mostrar variables de entorno

    # Prueba 10: Built-in export y unset
    "export VAR1=Hola && env | grep VAR1",  # Exportar variable
    "unset VAR1 && env | grep VAR1",  # Eliminar variable

    # Prueba 11: Built-in cd y pwd
    "cd / && pwd",  # Cambio de directorio
    "cd .. && pwd",  # Cambio a directorio padre
    "cd . && pwd",  # Cambio al mismo directorio
    "cd invalid_dir && echo $?",  # Cambio a un directorio inválido

    # Prueba 12: Manejo de PATH
    "unset PATH && ls",  # PATH inválido
    "export PATH=/usr/bin && ls",  # PATH válido

    # Prueba 13: Pipes
    "echo 'Hola' | grep 'H'",  # Uso de pipes
    "ls | wc -l",  # Conectar comandos con pipes
    "ls | grep 'file' | wc -l",  # Pipeline con múltiples comandos

    # Prueba 14: Redirecciones
    "echo 'redireccionando' > test_output.txt && cat test_output.txt",  # Redirección de output
    "cat < test_output.txt",  # Redirección de input
    "echo 'append' >> test_output.txt && cat test_output.txt",  # Redirección con append
    "cat << EOF\nhola\nEOF",  # Here document

    # Prueba 15: Manejo de errores
    "echo 'comando inválido;'",  # Punto y coma no soportado
    "echo \\ ",  # Backslash no soportado
    "export VAR@=123 && echo $VAR@",  # Nombre de variable no válido

    # Prueba 16: History
    "echo 'Probando historial' && history",  # Verificar historial

    # Prueba 17: Manejo de señales en comandos en ejecución
    "cat",  # Ejecutar cat y probar Ctrl-C y Ctrl-D manualmente

    # Prueba 18: Comportamiento interactivo
    "echo 'Presiona Ctrl-C después de este mensaje'",  # Probar Ctrl-C
    "echo 'Presiona Ctrl-D después de este mensaje'",  # Probar Ctrl-D
    "echo 'Presiona Ctrl-\\ después de este mensaje'",  # Probar Ctrl-\

    # Prueba 19: Comillas y variables
    'echo "$USER"',  # Comillas dobles con expansión de variable
    "echo '$USER'",  # Comillas simples sin expansión de variable

    # Prueba 20: Built-in exit y limpieza
    "exit 0",  # Salir limpiamente
]

# Instrucciones:
# 1. Guarda este archivo como `hoja_correccion.py` en tu carpeta `./tests/`.
# 2. Ejecuta las pruebas usando tu comparador:
#    - Para ejecutar solo estas pruebas: `run hoja_correccion.py`.
#    - Para ejecutar todas las pruebas: `run all`.
