# tests para el comando echo
tests = [
    # Pruebas básicas de echo
    "echo hello world",  # Mensaje simple
    "echo 'hello world'",  # Mensaje con comillas simples
    "echo \"hello world\"",  # Mensaje con comillas dobles
    "echo",  # Echo vacío, debe imprimir una línea en blanco
    "echo -n 'No newline'",  # Opción -n, no debe agregar nueva línea

    # Pruebas con la opción -n
    "echo -n Hello",  # Mensaje con -n, sin nueva línea
    "echo -n -n Multiple -n Options",  # Varias opciones -n (bash permite esto)
    "echo -n -e 'Hello\nWorld'",  # Combinación de -n y secuencias de escape
    "echo -n",  # Solo -n, sin texto

    # Manejo de variables
    "export USERNAME='testuser' && echo $USERNAME",  # Expansión de variable
    "echo '$USERNAME'",  # No expansión de variable con comillas simples
    "echo \"$USERNAME\"",  # Expansión de variable con comillas dobles
    "echo $HOME",  # Expansión de variable de entorno
    "echo '$HOME'",  # No expansión con comillas simples
    "echo \"$HOME\"",  # Expansión con comillas dobles

    # Secuencias de escape
    "echo -e 'Line1\nLine2'",  # Secuencia de escape \n (nueva línea)
    "echo -e 'Column1\tColumn2'",  # Secuencia de escape \t (tabulador)
    "echo -e '\\\\'",  # Secuencia de escape para barra invertida
    "echo -e 'Quotes: \\\"Double\\\" and \\\'Single\\\''",  # Escapando comillas

    # Manejo de caracteres especiales
    "echo *",  # Expansión de comodín (wildcard)
    "echo ?",  # Caracter de interrogación
    "echo > out.txt",  # Redirección (debe ser interpretada correctamente)
    "echo '>' out.txt",  # Comillas simples, no debe redireccionar
    "echo \"*\"",  # Comillas dobles, no debe expandir

    # Uso con múltiples líneas y concatenación
    "echo 'Line1' 'Line2'",  # Mensaje concatenado con comillas simples
    "echo \"Line1\" \"Line2\"",  # Mensaje concatenado con comillas dobles
    "echo 'Hello'\"World\"",  # Concatenación de comillas simples y dobles
    "echo \"Hello\"'World'",  # Concatenación de comillas dobles y simples

    # Pruebas con espacios y tabulaciones
    "echo '   Leading spaces'",  # Espacios al inicio
    "echo 'Trailing spaces   '",  # Espacios al final
    "echo '   Mixed   spaces   '",  # Espacios mixtos
    "echo 'Tabs\tare\there'",  # Tabulaciones en la cadena

    # Pruebas con secuencias de escape no interpretadas
    "echo 'Hello\\nWorld'",  # Secuencia de escape dentro de comillas simples (sin interpretar)
    "echo \"Hello\\nWorld\"",  # Secuencia de escape dentro de comillas dobles (sin interpretar)

    # Casos extremos y errores
    "echo \"",  # Comillas no cerradas
    "echo 'Unmatched quotes",  # Comillas simples no cerradas
    "echo -x 'Invalid option'",  # Opción inválida
    "echo $UNDEFINED_VAR",  # Variable no definida
    "echo $?",  # Código de salida del último comando
    "echo \"Mix $UNDEFINED_VAR with text\"",  # Variable no definida dentro de texto
    "echo -e -n 'Invalid combination'",  # Combinación de opciones inválida

    # Uso de echo con redirección y tuberías
    "echo 'This is a test' > test_output.txt",  # Redirección de salida
    "echo 'Append this line' >> test_output.txt",  # Append a archivo
    "echo 'Pipe this' | cat",  # Uso de echo con tubería
    "echo -n 'No newline' | cat",  # Uso de -n con tubería
    "echo 'Test with grep' | grep 'grep'",  # Uso con grep
]
