# tests para manejo de comillas
tests = [
    # Pruebas básicas de comillas simples
    "echo 'hello world'",  # Comillas simples, texto sin expansión
    "echo 'It\'s a test'",  # Comillas simples con escape
    "echo '$USER'",  # Comillas simples, no se expande la variable
    "echo 'hello \"world\"'",  # Comillas simples con comillas dobles dentro

    # Pruebas básicas de comillas dobles
    "echo \"hello world\"",  # Comillas dobles, texto simple
    "echo \"It's a test\"",  # Comillas dobles con comilla simple dentro
    "echo \"$USER\"",  # Comillas dobles, se expande la variable
    "echo \"hello 'world'\"",  # Comillas dobles con comillas simples dentro

    # Pruebas combinadas
    "echo '\"$HOME\"'",  # Comillas simples envuelven comillas dobles y una variable, no se expande
    "echo \"'$HOME'\"",  # Comillas dobles envuelven comillas simples y una variable, se expande
    "echo 'hello'\"world\"",  # Concatenación de comillas simples y dobles
    "echo \"hello\"'world'",  # Concatenación de comillas dobles y simples

    # Uso de comillas con variables
    "export VAR='test' && echo \"$VAR\"",  # Expansión de variable en comillas dobles
    "export VAR='test' && echo '$VAR'",  # No expansión de variable en comillas simples
    "echo \"La ruta es: $HOME\"",  # Expansión de variable de entorno en comillas dobles
    "echo 'La ruta es: $HOME'",  # No expansión de variable de entorno en comillas simples

    # Manejo de comillas vacías
    "echo ''",  # Comillas simples vacías
    "echo \"\"",  # Comillas dobles vacías
    "echo ' '",  # Espacio dentro de comillas simples
    "echo \" \"",  # Espacio dentro de comillas dobles

    # Casos extremos
    "echo 'This is a \"complex\" case'",  # Comillas dobles dentro de comillas simples
    "echo \"This is a 'complex' case\"",  # Comillas simples dentro de comillas dobles
    "echo '\"'\"",  # Comillas dobles dentro de comillas simples, luego comillas dobles
    "echo \"'\"'",  # Comillas simples dentro de comillas dobles, luego comillas simples

    # Uso con caracteres especiales
    "echo 'hello | world'",  # Comillas simples con pipe, no se interpreta
    "echo \"hello > world\"",  # Comillas dobles con redirección, no se interpreta
    "echo 'hello && world'",  # Comillas simples con AND lógico, no se interpreta
    "echo \"hello; world\"",  # Comillas dobles con punto y coma, no se interpreta

    # Pruebas de escape dentro de comillas
    "echo \"Escaping \\\"double quotes\\\" inside\"",  # Escapando comillas dobles dentro de comillas dobles
    "echo 'Escaping \\'single quotes\\' inside'",  # Escapando comillas simples dentro de comillas simples (no usual)
    "echo \"Mix \\$ with \$HOME\"",  # Mezcla de escape y expansión de variable
    "echo 'Mix \\$ with \$HOME'",  # Sin expansión, incluso con el escape

    # Combinación de comillas y redirecciones
    "echo 'output' > 'file.txt'",  # Uso de comillas simples para nombres de archivo
    "echo \"output\" > \"file.txt\"",  # Uso de comillas dobles para nombres de archivo
    "cat \"file.txt\"",  # Leer archivo usando comillas dobles
    "cat 'file.txt'",  # Leer archivo usando comillas simples

    # Pruebas de uso con múltiples comandos y comillas
    "echo 'start' && echo \"$USER\" && echo 'end'",  # Secuencia de comandos con comillas y variables
    "echo 'Hello' || echo 'Failed'",  # Uso de OR lógico con comillas
    "echo \"Check: $(echo 'subshell')\"",  # Uso de comillas dentro de subshell
    "echo \"Nested $(echo 'inside $(echo \"deep\")')\"",  # Anidamiento de subshells con comillas
]
