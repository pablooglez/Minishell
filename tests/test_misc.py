# test_misc.py
tests = [
    # Comandos vacíos y espacios
    "",  # Línea vacía
    "    ",  # Espacios en blanco
    "\t\t",  # Tabs
    "   \t   ",  # Combinación de espacios y tabs

    # Comandos inexistentes
    "comando_inexistente",  # Comando que no existe
    "invalid_command | echo 'Hello'",  # Comando inexistente con pipe
    "echo 'Test' | invalid_command",  # Comando inexistente después del pipe
    "/bin/false",  # Comando que falla explícitamente
    "/bin/true",  # Comando que siempre tiene éxito

    # Variables de entorno inesperadas
    "echo $",  # Variable vacía
    "echo $$",  # PID de shell
    "echo $USER $HOME",  # Variables estándar
    "echo $0 $1 $2",  # Argumentos de posición (no deberían estar definidos)
    "unset PATH && ls",  # PATH vacío, debería fallar

    # Expansión de variables con strings complejos
    "echo '$USER'",  # No debe expandir
    "echo \"$USER\"",  # Debe expandir
    "echo \"\$USER\"",  # Escape del signo de dólar
    "echo 'Hola $USER'",  # Literal, no expande
    "echo \"Hola $USER\"",  # Expande

    # Redirecciones combinadas
    "echo 'Testing' > test_file.txt && cat < test_file.txt",  # Redirección input/output
    "echo 'Overwrite' > test_file.txt && cat test_file.txt",  # Sobrescribir archivo
    "echo 'Append' >> test_file.txt && cat test_file.txt",  # Append al archivo
    "rm -f test_file.txt",  # Limpiar archivo de prueba

    # Here documents (heredocs)
    "cat <<EOF\nHello World\nEOF",  # Here document simple
    "cat <<EOF\nLine 1\nLine 2\nEOF",  # Here document con múltiples líneas
    "grep 'Hello' <<EOF\nHello\nWorld\nEOF",  # Here document con filtro

    # Comandos encadenados (combinaciones de pipes y redirecciones)
    "echo 'Hello' | tr '[:lower:]' '[:upper:]' | grep 'HELLO'",  # Pipeline encadenado
    "ls -la | grep 'test' > output.txt && cat output.txt",  # Redirigir salida de pipeline a archivo
    "rm -f output.txt",  # Limpiar archivo de prueba

    # Ejecución de comandos en segundo plano
    "sleep 1 &",  # Comando en segundo plano
    "sleep 1 && echo 'Finished'",  # Comando con AND
    "sleep 1 || echo 'Should not print this'",  # Comando con OR

    # Manejo de señales
    "cat & sleep 2",  # Ejecutar cat en segundo plano, interrumpir con Ctrl-C
    "sleep 5",  # Ejecutar y cancelar con Ctrl-C
    "echo 'Interrupt Test' && sleep 5",  # Interrumpir comando largo

    # Comportamiento al ejecutar archivos binarios
    "./test_files/test_binary",  # Ejecutar archivo binario (debe existir)
    "./test_files/invalid_binary",  # Ejecutar archivo binario inválido
    "chmod +x ./test_files/test_script.sh && ./test_files/test_script.sh",  # Ejecutar script de shell

    # Casos límite con argumentos y límites del sistema
    "echo 'A' * 10000",  # Argumento extremadamente largo
    "echo $((2**31))",  # Cálculo de límite entero
    "echo $(ls /)",  # Expansión de comando en subshell
    "echo $(seq 1 10000)",  # Expansión masiva de subshell

    # Pruebas de built-ins encadenados
    "cd / && pwd && cd -",  # Volver al directorio anterior
    "export TEST_VAR='Hello' && echo $TEST_VAR && unset TEST_VAR && echo $TEST_VAR",  # Crear, usar y eliminar variable
    "exit 42",  # Comando exit con código específico

    # Casos edge
    "echo 'Multi-line\nTest'",  # String con salto de línea
    "ls -la /dev/null",  # Acceso a /dev/null
    "ls -la /dev/doesnotexist",  # Acceso a dispositivo inexistente
    "kill -9 $$",  # Matar el proceso de minishell (debe terminar)
    "ulimit -c 0 && /bin/ls",  # Limitar core dump y ejecutar comando
]
