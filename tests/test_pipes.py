# test_pipes.py
tests = [
    # Casos simples
    "echo 'Hola Mundo' | cat",  # Pipe básico
    "echo 'Hello' | wc -c",  # Contar caracteres
    "ls | grep 'test'",  # Filtrar salida de ls
    "echo 'Testing' | tr '[:lower:]' '[:upper:]'",  # Convertir a mayúsculas

    # Casos encadenados
    "echo '1 2 3 4 5' | tr ' ' '\n' | sort | uniq",  # Procesamiento encadenado
    "ls | grep 'py' | wc -l",  # Contar archivos .py
    "cat /etc/passwd | grep 'root' | cut -d: -f1",  # Extraer usuario root
    "echo 'Pipe test' | cat | cat | cat",  # Múltiples pipes

    # Comandos inexistentes
    "ls | nonexistent_command",  # Comando no existente después de pipe
    "nonexistent_command | grep 'test'",  # Comando no existente antes de pipe

    # Redirecciones combinadas con pipes
    "echo 'Output redirection' > test_output.txt | cat test_output.txt",  # Redirección antes de pipe
    "echo 'Appending' >> test_output.txt | cat test_output.txt",  # Append antes de pipe
    "cat < test_output.txt | grep 'Output'",  # Redirección de input con pipe
    "rm -f test_output.txt",  # Limpiar archivo de prueba

    # Here documents combinados con pipes
    "cat << EOF | grep 'test'\nHello test\nEOF",  # Here document con pipe
    "cat << EOF | wc -l\nLine 1\nLine 2\nEOF",  # Contar líneas de Here document

    # Pipes con built-ins
    "echo 'Hello World' | echo",  # Pipe con echo
    "pwd | cat",  # Pipe con pwd
    "export VAR='Testing' | echo $VAR",  # Pipe con export (no debe afectar a export)
    "unset VAR | echo $VAR",  # Pipe con unset (no debe afectar a unset)
    "cd /tmp | pwd",  # Pipe con cd (no debe cambiar directorio)

    # Expansión de variables con pipes
    "echo $USER | grep $(whoami)",  # Expansión de variable y uso en pipe
    "echo $SHELL | tr '[:lower:]' '[:upper:]'",  # Convertir el valor de $SHELL a mayúsculas

    # Casos con pipes múltiples y complejos
    "echo 'Minishell Test' | tr ' ' '\n' | sort | uniq | wc -l",  # Pipeline largo
    "cat /dev/null | echo 'Empty Input'",  # Pipe con input vacío
    "cat /etc/hosts | head -n 5 | tail -n 1",  # Extracción de una línea específica

    # Pipes anidados y manejo de errores
    "echo 'Nested Pipe Test' | (grep 'Pipe' | tr '[:lower:]' '[:upper:]')",  # Pipes anidados
    "echo 'Error Test' | grep 'Test' | nonexistent_command",  # Comando inválido en pipeline
    "ls /invalid_directory | grep 'something'",  # Error en comando inicial de pipeline

    # Uso de stderr con pipes
    "ls invalid_file 2>&1 | grep 'No such file'",  # Redirigir stderr a stdout y usar pipe
    "grep 'root' /etc/passwd 2>&1 | wc -l",  # Redirigir stderr y contar líneas
    "echo 'Test stderr' | nonexistent_command 2>&1 | grep 'command not found'",  # Comando inválido y manejo de stderr

    # Casos con señales y pipes
    "sleep 2 | echo 'Signal Test'",  # Comando que espera, interrumpir con Ctrl-C
    "cat | grep 'Signal' & sleep 2",  # Comando en segundo plano, interrumpir con Ctrl-C

    # Combinación de pipes y redirecciones
    "echo 'Redirect and Pipe' > pipe_test.txt | cat pipe_test.txt",  # Redirección y pipe
    "cat < pipe_test.txt | tr '[:lower:]' '[:upper:]' > result.txt && cat result.txt",  # Redirección y escritura en archivo
    "rm -f pipe_test.txt result.txt",  # Limpiar archivos de prueba
]
