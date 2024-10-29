import subprocess
from colorama import Fore, Style, init
import os

# Inicializa colorama para el color en la terminal
init(autoreset=True)

# Define la ruta a tu minishell
MINISHELL_PATH = "./minishell"

# Lista de 100 pruebas con comandos y el resultado de parseo esperado
tests = [ 
    
    {"id": 50, "description": "Expansión de $? en pipeline", "command": "echo $? | cat -e", 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: 0\nCommanDC: cat\nArgumentDC 0: cat\nArgumentDC 1: -e"},
    
    # Secuencias complejas con redirección y múltiples variables
    {"id": 51, "description": "Redirección de salida con múltiples argumentos", "command": "echo Esto es una prueba > prueba.txt", 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: Esto\nArgumentDC 2: es\nArgumentDC 3: una\nArgumentDC 4: prueba\nRedirection: >\nFile: prueba.txt"},
    
    {"id": 52, "description": "Secuencia de redirección y concatenación", "command": "echo Hola | cat > test_output.txt", 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: Hola\nCommanDC: cat\nRedirection: >\nFile: test_output.txt"},
    {"id": 53, "description": "Concatenación de dos variables $USER$HOME", 
     "command": "echo $USER$HOME", 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: <nombre_usuario><home_dir>"},

    {"id": 54, "description": "Concatenación con string intermedio $USER-test-$SHELL", 
     "command": "echo $USER-test-$SHELL", 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: <nombre_usuario>-test-<shell>"},

    # Pruebas con heredoc y delimitadores en varias combinaciones
    {"id": 55, "description": "Heredoc con delimitador simple", 
     "command": "cat << END", 
     "expected_output": "CommanDC: cat\nArgumentDC 0: cat\nRedirection: <<\nDelimiter: END"},

    {"id": 56, "description": "Heredoc con comillas y variable en delimitador", 
     "command": "cat << \"$USER\"", 
     "expected_output": "CommanDC: cat\nArgumentDC 0: cat\nRedirection: <<\nDelimiter: <nombre_usuario>"},

    # Redirección y salida con pipes y variables en redirección
    {"id": 57, "description": "Pipeline y redirección de salida en variable", 
     "command": "echo Hola | cat > $HOME/test_output.txt", 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: Hola\nCommanDC: cat\nRedirection: >\nFile: <home_dir>/test_output.txt"},

    # Errores de redirección sin archivo especificado
    {"id": 58, "description": "Error de redirección de salida sin archivo", 
     "command": "echo hola >", 
     "expected_output": "Error: falta archivo de redirección."},

    # Pipes y redirección en varias combinaciones
    {"id": 59, "description": "Redirección de salida y entrada con pipe intermedio", 
     "command": "cat < input.txt | grep test > output.txt", 
     "expected_output": "CommanDC: cat\nArgumentDC 0: cat\nRedirection: <\nFile: input.txt\nCommanDC: grep\nArgumentDC 0: grep\nArgumentDC 1: test\nRedirection: >\nFile: output.txt"},

    {"id": 60, "description": "Pipeline y redirección compleja con varias variables", 
     "command": "echo $USER | grep $HOME > $PWD/result.txt", 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: <nombre_usuario>\nCommanDC: grep\nArgumentDC 0: grep\nArgumentDC 1: <home_dir>\nRedirection: >\nFile: <directorio_actual>/result.txt"},

    # Escapes y comillas complejas en strings
    {"id": 61, "description": "Comando con comillas dobles y expansión dentro", 
     "command": 'echo "El usuario es $USER"', 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: El usuario es <nombre_usuario>"},

    {"id": 62, "description": "Comillas simples alrededor de expansión", 
     "command": "echo 'Usuario $USER'", 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: Usuario $USER"},

    {"id": 63, "description": "Escape de comillas simples dentro de dobles", 
     "command": 'echo "Es \'importante\' entender"', 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: Es 'importante' entender"},

    # Expansión en argumentos de redirección
    {"id": 64, "description": "Redirección con variable de entorno en nombre de archivo", 
     "command": "echo Prueba > $USER_file.txt", 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: Prueba\nRedirection: >\nFile: <nombre_usuario>_file.txt"},

    {"id": 65, "description": "Redirección de salida con concatenación de múltiples variables", 
     "command": "echo Test > $USER$HOME/test_output.txt", 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: Test\nRedirection: >\nFile: <nombre_usuario><home_dir>/test_output.txt"},

    # Errores y casos límite con pipes y redirección sin comandos
    {"id": 66, "description": "Error de redirección sin comando previo", 
     "command": "> test_output.txt", 
     "expected_output": "Error: redirección sin comando previo."},

    {"id": 67, "description": "Error: Pipe sin comando anterior", 
     "command": "| echo Hola", 
     "expected_output": "Error: pipe sin comando anterior."},

    {"id": 68, "description": "Error: Pipe sin comando después", 
     "command": "echo Hola |", 
     "expected_output": "Error: falta un comando después del pipe."},

    # Redirecciones dobles y dobles pipes en combinación
    {"id": 69, "description": "Comando con dos pipes y dos redirecciones", 
     "command": "echo start | grep s | cat < input.txt > output.txt", 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: start\nCommanDC: grep\nArgumentDC 0: grep\nArgumentDC 1: s\nCommanDC: cat\nRedirection: <\nFile: input.txt\nRedirection: >\nFile: output.txt"},

    {"id": 70, "description": "Redirección de entrada y heredoc juntos", 
     "command": "cat < input.txt << EOF", 
     "expected_output": "CommanDC: cat\nArgumentDC 0: cat\nRedirection: <\nFile: input.txt\nRedirection: <<\nDelimiter: EOF"},

    {"id": 71, "description": "Herencia de heredoc con expansión", 
     "command": "cat << $USER << $PWD", 
     "expected_output": "CommanDC: cat\nArgumentDC 0: cat\nRedirection: <<\nDelimiter: <nombre_usuario>\nRedirection: <<\nDelimiter: <directorio_actual>"},

    # Expansión de variables dentro de redirecciones
    {"id": 72, "description": "Expansión de variables en archivo de redirección", 
     "command": "echo test > $USER_output_$SHELL", 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: test\nRedirection: >\nFile: <nombre_usuario>_output_<shell>"},

    # Expansión y error con variables no existentes
    {"id": 73, "description": "Expansión de variable no existente con texto", 
     "command": "echo $NOT_DEFINED_text", 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: _text"},

    {"id": 74, "description": "Variable no existente entre comillas", 
     "command": 'echo "$NOT_DEFINED dentro"', 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1:  dentro"},

    {"id": 75, "description": "Variable sin expansión en comillas simples", 
     "command": "echo '$USER y $HOME'", 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: $USER y $HOME"},

    # Variaciones en manejo de comillas y escapes
    {"id": 76, "description": "Escapes dentro de comillas dobles y simples", 
     "command": 'echo "Escaped \"characters\" and \'single\' quotes"', 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: Escaped \"characters\" and 'single' quotes"},

    {"id": 77, "description": "Escape de caracter con \\ dentro de comillas dobles", 
     "command": "echo \"test\\nline\"", 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: test\\nline"},

    {"id": 78, "description": "Comillas y escapes combinados", 
     "command": "echo \"test \\\"complex\\\" sequence\"", 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: test \"complex\" sequence"},

    # Varias redirecciones y pipes en una sola línea
    {"id": 79, "description": "Triple redirección con pipes", 
     "command": "echo hi > file1 | grep hi >> file2 | wc -l < file1", 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: hi\nRedirection: >\nFile: file1\nCommanDC: grep\nArgumentDC 0: grep\nArgumentDC 1: hi\nRedirection: >>\nFile: file2\nCommanDC: wc\nArgumentDC 0: wc\nArgumentDC 1: -l\nRedirection: <\nFile: file1"},

    {"id": 80, "description": "Concatenación de redirección y pipe combinados", 
     "command": "grep test < input.txt | sort > output.txt", 
     "expected_output": "CommanDC: grep\nArgumentDC 0: grep\nArgumentDC 1: test\nRedirection: <\nFile: input.txt\nCommanDC: sort\nRedirection: >\nFile: output.txt"},
{"id": 81, "description": "Pipe con múltiples redirecciones de salida", 
     "command": "echo hello | tee output1.txt | tee output2.txt", 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: hello\nCommanDC: tee\nArgumentDC 0: tee\nArgumentDC 1: output1.txt\nCommanDC: tee\nArgumentDC 0: tee\nArgumentDC 1: output2.txt"},

    {"id": 82, "description": "Redirección de entrada y múltiples pipes", 
     "command": "cat < input.txt | grep test | wc -l", 
     "expected_output": "CommanDC: cat\nArgumentDC 0: cat\nRedirection: <\nFile: input.txt\nCommanDC: grep\nArgumentDC 0: grep\nArgumentDC 1: test\nCommanDC: wc\nArgumentDC 0: wc\nArgumentDC 1: -l"},

    # Casos límite de redirección
    {"id": 83, "description": "Redirección de entrada sin archivo especificado", 
     "command": "cat <", 
     "expected_output": "Error: falta archivo para redirección de entrada."},

    {"id": 84, "description": "Redirección de salida sin comando previo", 
     "command": "> output.txt", 
     "expected_output": "Error: redirección sin comando previo."},

    # Combinaciones avanzadas con variables y heredoc
    {"id": 85, "description": "Heredoc con variable en delimitador", 
     "command": "cat << $USER", 
     "expected_output": "CommanDC: cat\nArgumentDC 0: cat\nRedirection: <<\nDelimiter: <nombre_usuario>"},

    {"id": 86, "description": "Heredoc con texto simple", 
     "command": "cat << END", 
     "expected_output": "CommanDC: cat\nArgumentDC 0: cat\nRedirection: <<\nDelimiter: END"},

    {"id": 87, "description": "Heredoc con comillas dobles y variable", 
     "command": "cat << \"$USER\"", 
     "expected_output": "CommanDC: cat\nArgumentDC 0: cat\nRedirection: <<\nDelimiter: <nombre_usuario>"},

    # Comillas y variables en combinaciones complejas
    {"id": 88, "description": "Expansión de varias variables con texto entre ellas", 
     "command": 'echo "User: $USER, Shell: $SHELL"', 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: User: <nombre_usuario>, Shell: <shell>"},

    {"id": 89, "description": "Comillas simples dentro de comillas dobles con expansión", 
     "command": 'echo "Esto es \'$USER\' en comillas"', 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: Esto es '$USER' en comillas"},

    # Secuencias avanzadas de pipes y redirecciones con expansión
    {"id": 90, "description": "Pipe con redirección de salida y expansión en archivo", 
     "command": "echo Prueba | grep Prueba > $USER_output.txt", 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: Prueba\nCommanDC: grep\nArgumentDC 0: grep\nArgumentDC 1: Prueba\nRedirection: >\nFile: <nombre_usuario>_output.txt"},

    {"id": 91, "description": "Combinación de redirección de entrada, pipes y expansión", 
     "command": "cat < $HOME/file | grep test | wc -l", 
     "expected_output": "CommanDC: cat\nArgumentDC 0: cat\nRedirection: <\nFile: <home_dir>/file\nCommanDC: grep\nArgumentDC 0: grep\nArgumentDC 1: test\nCommanDC: wc\nArgumentDC 0: wc\nArgumentDC 1: -l"},

    # Variables no definidas y expansión en argumentos
    {"id": 92, "description": "Variable indefinida en comillas dobles", 
     "command": 'echo "$UNDEF_VAR es indefinido"', 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1:  es indefinido"},

    {"id": 93, "description": "Variable en comillas dobles sin expansión", 
     "command": "echo '$USER $HOME'", 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: $USER $HOME"},

    # Uso avanzado de pipes y redirección con múltiples comandos
    {"id": 94, "description": "Redirección y pipes en secuencia compleja", 
     "command": "ls | grep py | sort > result.txt", 
     "expected_output": "CommanDC: ls\nArgumentDC 0: ls\nCommanDC: grep\nArgumentDC 0: grep\nArgumentDC 1: py\nCommanDC: sort\nRedirection: >\nFile: result.txt"},

    {"id": 95, "description": "Error de redirección y pipe sin comandos", 
     "command": "| > test_output.txt", 
     "expected_output": "Error: redirección y pipe sin comando anterior."},

    # Escapes complejos
    {"id": 96, "description": "Escape de $ dentro de comillas dobles", 
     "command": 'echo "Esto cuesta \$5"', 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: Esto cuesta $5"},

    {"id": 97, "description": "Escape de comillas dentro de comillas dobles", 
     "command": 'echo "Dijo: \\"Hola\\""', 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: Dijo: \"Hola\""},

    # Redirección de salida en concatenación compleja de variables
    {"id": 98, "description": "Expansión de variables concatenadas en redirección", 
     "command": "echo Output > $USER_$SHELL_output.txt", 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: Output\nRedirection: >\nFile: <nombre_usuario>_<shell>_output.txt"},

    {"id": 99, "description": "Variable indefinida y definida en concatenación", 
     "command": "echo $UNDEFINED_VAR$HOME", 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: <home_dir>"},

    {"id": 100, "description": "Secuencia final de redirección y expansión compleja", 
     "command": "echo \"Path: $PATH\" > final_output.txt", 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: Path: <path>\nRedirection: >\nFile: final_output.txt"},
]

# Añade aquí 75 pruebas adicionales que incluyen combinaciones de comandos, redirecciones, pipes, variables de entorno, comillas, etc.

def run_minishell(command):
    """Ejecuta el parser de minishell con un comando y captura su salida"""
    env = os.environ.copy()
    env["TEST_MODE"] = "1"  # Activa el modo de prueba en minishell
    process = subprocess.run(MINISHELL_PATH, input=command, text=True, capture_output=True, shell=True, env=env)
    return process.stdout.strip(), process.returncode

def clean_output(output):
    """Limpia la salida eliminando cualquier línea que contenga 'Minishell>' en cualquier lugar de la salida."""
    lines = output.splitlines()
    filtered_lines = [line for line in lines if "Minishell>" not in line]
    return "\n".join(filtered_lines).strip()

def run_test(test):
    """Ejecuta una prueba de parseo en minishell y compara la salida con el resultado esperado"""
    print(f"\nPrueba {test['id']}: {test['description']}")
    print(Fore.CYAN + f"Comando ejecutado: {test['command']}" + Style.RESET_ALL)
    
    minishell_output, return_code = run_minishell(test["command"])
    minishell_output = clean_output(minishell_output)  # Limpia la salida del prompt y comando inicial
    
    # Adaptamos valores específicos antes de la comparación
    expected_output = test["expected_output"].replace("<nombre_usuario>", os.getenv("USER", ""))
    expected_output = expected_output.replace("<último_estado_de_salida>", "0")
    expected_output = expected_output.replace("<directorio_actual>", os.getenv("PWD", ""))
    expected_output = expected_output.replace("<home_dir>", os.getenv("HOME", ""))
    
    # Verificación de salida del parser
    if minishell_output == expected_output:
        print(Fore.GREEN + f"[OK] Prueba {test['id']}: {test['description']}")
        print(Fore.YELLOW + "Esperado:" + Style.RESET_ALL)
        print(expected_output)
        print(Fore.YELLOW + "Obtenido:" + Style.RESET_ALL)
        print(minishell_output)
        return True
    else:
        print(Fore.RED + f"[FAIL] Prueba {test['id']}: {test['description']}")
        print(Fore.YELLOW + "Esperado:" + Style.RESET_ALL)
        print(expected_output)
        print(Fore.YELLOW + "Obtenido:" + Style.RESET_ALL)
        print(minishell_output)
        return False

def main():
    passed_tests = 0
    failed_tests = 0
    total_tests = len(tests)
    
    for test in tests:
        if run_test(test):
            passed_tests += 1
        else:
            failed_tests += 1

    # Resumen
    print("\n" + "="*30)
    print("Resumen de pruebas de parseo:")
    print(f"Total de pruebas: {total_tests}")
    print(Fore.GREEN + f"Pruebas exitosas: {passed_tests}" + Style.RESET_ALL)
    print(Fore.RED + f"Pruebas fallidas: {failed_tests}" + Style.RESET_ALL)
    print("="*30)

if __name__ == "__main__":
    main()
