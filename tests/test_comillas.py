# tests para manejo de comillas
tests = [
    # Pruebas básicas de comillas simples
    "echo 'hello world'",               # Texto simple entre comillas simples
    "echo 'It is a test'",              # Comillas simples con texto común
    "echo '$USER'",                     # Comillas simples, no se expande la variable
    "echo 'hello \"world\"'",           # Comillas dobles dentro de comillas simples
    "echo ''",                          # Comillas simples vacías
    "echo ' '",                         # Espacio dentro de comillas simples
    "echo 'hello world $USER'",         # Comillas simples, no se expande ninguna variable
    "echo 'This is a test'",            # Frase dentro de comillas simples
    "echo '\"$HOME\" is home'",         # Comillas dobles y texto dentro de comillas simples

    # Pruebas básicas de comillas dobles
    "echo \"hello world\"",             # Texto simple entre comillas dobles
    "echo \"It's a test\"",             # Comilla simple dentro de comillas dobles
    "echo \"$USER\"",                   # Comillas dobles, se expande la variable
    "echo \"hello 'world'\"",           # Comillas simples dentro de comillas dobles
    "echo \"\"",                        # Comillas dobles vacías
    "echo \" \"",                       # Espacio dentro de comillas dobles
    "echo \"hello $USER world\"",       # Comillas dobles, texto con variable expandida
    "echo \"'$USER' is a user\"",       # Comillas simples y texto dentro de dobles

    # Combinaciones de comillas
    "echo 'hello' \"world\"",           # Concatenación de comillas simples y dobles
    "echo \"hello\" 'world'",           # Concatenación de comillas dobles y simples
    "echo \"'$USER'\"",                # Comillas dobles envuelven comillas simples con variable
    "echo '\"$USER\"'",                # Comillas simples envuelven comillas dobles con variable
    "echo '\"'",                        # Comillas dobles dentro de comillas simples
    "echo \"'\"",                       # Comillas simples dentro de comillas dobles
    "echo \"hello 'world'\" 'again'",   # Mezcla compleja con concatenación

    # Comillas con variables
    "echo \"$HOME\"",                   # Variable expandida en comillas dobles
    "echo 'Path: $HOME'",               # Variable no expandida en comillas simples
    "echo \"Path is $HOME\"",           # Comillas dobles con variable expandida
    "echo 'Path is \"$HOME\"'",         # Comillas simples contienen comillas dobles y variable no expandida
    "echo \"$USER's directory is $HOME\"",  # Texto y variables en comillas dobles

    # Casos extremos de anidación
    "echo '\"$HOME\"$USER'",            # Comillas simples con variable dentro y fuera
    "echo \"hello 'world' $USER\"",     # Comillas simples y variables dentro de dobles
    "echo 'hello \"world\" $USER'",     # Comillas dobles dentro de simples con variable no expandida
    "echo '\"'",                        # Alternancia mínima: comillas dobles dentro de simples
    "echo \"'\"",                       # Alternancia mínima: comillas simples dentro de dobles
    "echo '\"hello\"' \"world\"",       # Alternancia compleja

    # Casos de errores esperados
    "echo 'hello",                      # Comillas simples no cerradas
    "echo \"hello",                     # Comillas dobles no cerradas
    "echo '\"hello'",                   # Comillas dobles dentro de simples no cerradas
    "echo \"'hello\"",                  # Comillas simples dentro de dobles no cerradas

    # Casos con espacios y texto adicional
    "echo 'multiple words'",            # Texto con espacios dentro de comillas simples
    "echo \"multiple words\"",          # Texto con espacios dentro de comillas dobles
    "echo 'word1' 'word2'",             # Comillas simples separadas
    "echo \"word1\" \"word2\"",         # Comillas dobles separadas
    "echo 'word1'\"word2\"",            # Comillas simples y dobles concatenadas
    "echo \"word1\"'word2'",            # Comillas dobles y simples concatenadas
    "echo 'word1 \"word2\" word3'",     # Mezcla con comillas dobles dentro de simples

    # Casos de múltiples combinaciones de comillas
    "echo \"hello 'world'\" \"again\"", # Mezcla con dobles y simples
    "echo '\"hello\"' 'world'",         # Dobles dentro de simples, separados por espacio
    "echo \"'hello'\" \"$USER\"",       # Simples dentro de dobles con variable expandida
    "echo \"hello\"' '\"world\"",       # Secuencia de comillas con espacio entre palabras
]
