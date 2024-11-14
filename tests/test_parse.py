# test_parse.py
tests = [
    # Pruebas básicas de tokenización
    "echo hello world",  # Comando simple
    "ls -la /tmp",  # Comando con argumentos
    "pwd && echo done",  # Comando encadenado con AND
    "mkdir test_dir || echo 'Failed'",  # Comando encadenado con OR

    # Pruebas con espacios y tabs
    "    echo    spaced    input   ",  # Espacios extras
    "\techo\t'tabbed input'",  # Tabs
    "echo   'multiple   spaces'   here",  # Múltiples espacios dentro de comillas

    # Pruebas con comillas simples
    "echo 'Hello World'",  # Texto entre comillas simples
    "echo 'It''s a test'",  # Comillas simples anidadas escapadas
    "echo 'Unclosed quote",  # Comilla simple sin cerrar (debe manejar error)
    "echo '$USER is a variable'",  # Variable dentro de comillas simples (no se expande)

    # Pruebas con comillas dobles
    "echo \"Hello $USER\"",  # Expansión de variable dentro de comillas dobles
    "echo \"Nested 'quotes' inside\"",  # Comillas simples dentro de comillas dobles
    "echo \"Unclosed quote",  # Comilla doble sin cerrar (debe manejar error)
    "echo \"Special chars: \$, \", and \\\"",  # Caracteres especiales escapados

    # Pruebas con redirecciones
    "echo 'Testing' > output.txt",  # Redirección de salida
    "cat < input.txt",  # Redirección de entrada
    "grep 'pattern' < input.txt > output.txt",  # Redirección combinada
    "echo 'Append' >> append.txt",  # Redirección de append
    "cat <<EOF\nHello World\nEOF",  # Here document

    # Pruebas con pipes
    "echo 'Hello' | grep 'H'",  # Comando simple con pipe
    "ls | grep 'test' | sort",  # Comando encadenado con pipes
    "cat | cat | cat",  # Múltiples pipes sin argumentos
    "echo 'Pipe test' |",  # Pipe al final (debe manejar error)

    # Pruebas con expansión de variables
    "echo $HOME",  # Variable de entorno
    "echo $?",  # Código de salida del último comando
    "echo $$",  # PID del shell
    "echo $UNDEFINED_VAR",  # Variable no definida
    "echo \"Path: $PATH\"",  # Expansión dentro de comillas dobles
    "echo 'Path: $PATH'",  # No debe expandir dentro de comillas simples

    # Pruebas con comandos especiales y caracteres
    "ls; echo 'Done'",  # Punto y coma como separador (debe manejar error)
    "echo 'Hello && Goodbye'",  # AND dentro de comillas (no debe interpretar)
    "echo \"Hello || Goodbye\"",  # OR dentro de comillas (no debe interpretar)
    "echo \\$",  # Carácter escapado
    "echo \"Escaped \\\" quote\"",  # Comillas escapadas

    # Pruebas con comandos vacíos y nulos
    "echo ''",  # Argumento vacío
    "echo \"\"",  # Argumento vacío con comillas dobles
    "echo '' ''",  # Múltiples argumentos vacíos
    "\"\"",  # Input solo con comillas (debe manejar error)
    "''",  # Input solo con comillas simples (debe manejar error)
    " ",  # Input con solo un espacio
    "\t",  # Input con solo un tab

    # Pruebas con caracteres especiales no interpretados
    "echo $@",  # Parámetros de posición (no deben estar definidos)
    "echo $#",  # Número de argumentos (no debe estar definido)
    "echo $*",  # Todos los argumentos (no debe estar definido)
    "echo 'Special chars: *, &, ;, |, >, <'",  # Caracteres especiales literales

    # Pruebas complejas de tokenización
    "echo \"Start\" && echo \"Middle\" || echo \"End\"",  # Combinación de AND y OR
    "echo \"Nested $(echo 'subshell')\"",  # Expansión de subshell (no implementado, debe manejarse)
    "echo \"Escaped \\\" double quotes\"",  # Comillas dobles escapadas
    "echo 'Mixed quotes \"inside\" single quotes'",  # Comillas dobles dentro de comillas simples
    "echo \"Mixed quotes 'inside' double quotes\"",  # Comillas simples dentro de comillas dobles
    "echo $(echo $(echo 'Nested subshell'))",  # Subsidiaria anidada (no implementado, debe manejarse)

    # Casos límite
    ";",  # Solo un punto y coma (debe manejar error)
    "|",  # Solo un pipe (debe manejar error)
    ">",  # Solo redirección de salida (debe manejar error)
    "<",  # Solo redirección de entrada (debe manejar error)
    "echo \"Unbalanced quote",  # Comilla sin cerrar
    "echo 'Unbalanced single quote",  # Comilla simple sin cerrar
    "echo \"Nested 'unbalanced",  # Comillas anidadas y no balanceadas
]
