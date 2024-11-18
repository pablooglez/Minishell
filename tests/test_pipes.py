# test_pipes.py

tests = [
    # Casos básicos de pipes
    "echo 'Hello World' | cat",  # Pipe simple, debería funcionar
    "echo 'Hello' | grep 'H'",  # Filtrado usando pipe
    "echo 'Hello' | grep 'h'",  # Filtrado fallido (grep es case-sensitive)
    "ls | sort",  # Listar archivos y ordenarlos
    "ls -la | grep 'test'",  # Filtrar archivos con 'test'

    # Casos de error con pipes
    "| echo 'Hello'",  # Pipe al inicio, debería dar error
    "echo 'Test' || echo 'This should not run'",  # Doble pipe (no válido en obligatorio)
    "echo 'Test' | | echo 'Hello'",  # Dos pipes consecutivos, debería dar error
    "echo 'Test' |",  # Pipe al final, debería dar error
    "echo | echo",  # Pipe vacío, debería dar error

    # Pipes anidados
    "echo 'Line 1\nLine 2\nLine 3' | grep 'Line' | wc -l",  # Contar líneas con grep y wc
    "echo 'Hello' | tr '[:lower:]' '[:upper:]' | grep 'HELLO'",  # Convertir a mayúsculas y filtrar
    "cat /etc/passwd | grep 'root' | cut -d: -f1",  # Buscar 'root' y cortar el primer campo

    # Pipes con comandos inexistentes
    "invalid_command | echo 'Hello'",  # Comando inexistente antes del pipe
    "echo 'Test' | invalid_command",  # Comando inexistente después del pipe

    # Pipes con redirecciones
    "echo 'Hello World' | tee output.txt | cat",  # Redireccionar a archivo y mostrar en pantalla
    "ls | grep 'test' > output.txt",  # Redirigir salida del pipe a archivo
    "cat < output.txt | grep 'test'",  # Usar redirección con pipe

    # Here documents con pipes
    "cat <<EOF | grep 'Hello'\nHello World\nEOF",  # Here document con pipe
    "grep 'Hello' <<EOF | wc -l\nHello\nWorld\nEOF",  # Filtrar con here document y contar líneas

    # Combinaciones complejas
    "echo 'test1\ntest2\ntest3' | grep 'test' | sort | uniq",  # Ordenar y eliminar duplicados
    "echo 'A\nB\nC' | tr '[:upper:]' '[:lower:]' | grep 'a'",  # Convertir a minúsculas y buscar
    "seq 1 5 | xargs -I {} echo 'Number: {}'",  # Usar xargs con pipe
    #"yes | head -n 10 | grep 'y'",  # Generar líneas y filtrar

    # Casos límite
    "echo 'Test' |",  # Pipe al final, debería dar error
    "| echo 'Test'",  # Pipe al inicio, debería dar error
    "echo 'Test' | | echo 'Double Pipe'",  # Doble pipe, debería dar error
    "echo 'Test' | invalid_command | echo 'Should not run'",  # Comando inexistente en el medio

    # Pruebas de ejecución en segundo plano (background)
    "sleep 1 | echo 'Should wait'",  # Comando con sleep en pipe
    "echo 'Test' | sleep 1",  # Sleep al final del pipe
    "sleep 1 & echo 'Running in background' | cat",  # Background con pipe

    # Pruebas de uso de pipes con comandos built-in
    "export VAR='Hello' | echo $VAR",  # Uso de pipe con export
    "cd / | pwd | grep '/'",  # Uso de pipe con cd y pwd
    "exit 0 | echo 'Should not print'",  # Uso de pipe con exit
]

