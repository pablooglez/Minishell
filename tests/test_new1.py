tests = [
    # Prueba 1: Línea vacía
    "   ",
    "\t\t\t",
    "\n",

    # Prueba 2: Entrada con solo espacios
    "     ",
    "   \t   ",
    "      \t      ",

    # Prueba 3: EOF (Ctrl+D)
    "\x04",  # Simula Ctrl+D para enviar EOF

    # Prueba 4: Manejo de Ctrl+C
    "\x03",  # Simula Ctrl+C para interrumpir la línea actual

    # Prueba 5: Comando desconocido
    "comando_no_existente",

    # Prueba 6: Comando válido
    "echo Hola Mundo",

    # Prueba 7: Comando con múltiples espacios
    "ls    -la   ",
    "echo     'Test    con   espacios'   ",

    # Prueba 8: Comando con tabulaciones
    "echo\tHola\tMundo",
    "ls\t-la\t/usr\t",

    # Prueba 9: Manejo de salida con Ctrl+D después de un comando
    "echo Adiós\x04",

    # Prueba 10: Manejo de múltiples Ctrl+C
    "\x03\x03\x03",
    "echo Hola\x03",

    # Prueba 11: Normalización de entrada
    "echo\t\t\tHola",
    "echo    \t    Test",
    "ls\t -la   \t/usr",

    # Prueba 12: Entrada que mezcla espacios y tabulaciones
    "    \t\t   ",
    "\t    \t   \t   ",

    # Prueba 13: Comando con salida estándar redirigida
    "echo 'Probando redirección' > output.txt",

    # Prueba 14: Comando con redirección inválida
    "echo 'Esto fallará' > ",

    # Prueba 15: Manejo de comillas no balanceadas
    "echo 'Esta frase no cierra",
    "echo \"Otra frase sin cierre",

    # Prueba 16: Normalización de entrada con caracteres especiales
    "echo $USER",
    "echo $HOME",
    "echo '$USER no debe expandirse'",

    # Prueba 17: Verificación de comandos internos
    "pwd",
    "cd .. && pwd",
    "export TEST_VAR='Hola' && echo $TEST_VAR && unset TEST_VAR && echo $TEST_VAR",
    "env",

    # Prueba 18: Manejo de pipes simples
    "echo 'Prueba de pipe' | grep 'Prueba'",
    "ls | sort",

    # Prueba 19: Secuencia de salida normal con EOF
    "exit 0",
    "exit 42",
    "exit",

    # Prueba 20: Manejo de SIGQUIT (Ctrl+\)
    "\x1c",  # Simula Ctrl+\ para probar que se ignora
]