import subprocess
from colorama import Fore, Style, init
import os

# Inicializa colorama para el color en la terminal
init(autoreset=True)

# Define la ruta a tu minishell
MINISHELL_PATH = "./minishell"

# Lista de 100 pruebas con comandos y el resultado de parseo esperado
tests = [
    # Variables de entorno y expansión básica
    {"id": 1, "description": "Expansión de variable de entorno $USER", "command": "echo $USER", "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: <nombre_usuario>"},
    {"id": 2, "description": "Expansión de variable no definida", "command": "echo $UNDEFINED_VAR", "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1:"},
    {"id": 3, "description": "Texto literal con comillas simples", "command": "echo '$USER'", "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: $USER"},
    {"id": 4, "description": "Comillas dobles para expandir $USER", "command": 'echo "$USER"', "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: <nombre_usuario>"},
    {"id": 5, "description": "Expansión de $? para estado de salida", "command": "echo $?", "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: <último_estado_de_salida>"},
    # Combinación y separación con espacios
    {"id": 6, "description": "Espacios múltiples entre palabras", "command": "echo    hola      mundo", "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: hola\nArgumentDC 2: mundo"},
    {"id": 7, "description": "Comillas dobles con espacio dentro", "command": 'echo "con espacios  dentro"', "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: con espacios  dentro"},
    {"id": 8, "description": "Comillas simples y dobles mezcladas", "command": 'echo "hola \'mundo\'"', "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: hola 'mundo'"},
    # Pipes y redirecciones
    {"id": 9, "description": "Pipeline simple", "command": "echo Hello | grep H", "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: Hello\nCommanDC: grep\nArgumentDC 0: grep\nArgumentDC 1: H"},
    {"id": 10, "description": "Pipeline con múltiples pipes", "command": "echo Hello | grep H | wc -l", "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: Hello\nCommanDC: grep\nArgumentDC 0: grep\nArgumentDC 1: H\nCommanDC: wc\nArgumentDC 0: wc\nArgumentDC 1: -l"},
    {"id": 11, "description": "Redirección de salida", "command": "echo Hello > output.txt", "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: Hello\nRedirection: >\nFile: output.txt"},
    {"id": 12, "description": "Redirección de entrada", "command": "cat < input.txt", "expected_output": "CommanDC: cat\nArgumentDC 0: cat\nRedirection: <\nFile: input.txt"},
    {"id": 13, "description": "Redirección de entrada y salida", "command": "cat < input.txt > output.txt", "expected_output": "CommanDC: cat\nArgumentDC 0: cat\nRedirection: <\nFile: input.txt\nRedirection: >\nFile: output.txt"},
    # Escape de caracteres especiales y combinaciones
    {"id": 14, "description": "Escape de comillas dobles en comillas simples", "command": 'echo \'"Escapado" dentro\'', "expected_output": 'CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: "Escapado" dentro'},
    {"id": 15, "description": "Escapar \$ en expansión", "command": "echo \$USER", "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: $USER"},
    {"id": 16, "description": "Expansión de variable compuesta $USER$SHELL", "command": "echo $USER$SHELL", "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: <nombre_usuario><shell>"},
    # Redirección en modo append y heredoc
    {"id": 17, "description": "Redirección de salida doble (append)", "command": "echo salida >> output.txt", "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: salida\nRedirection: >>\nFile: output.txt"},
    {"id": 18, "description": "Heredoc simple", "command": "cat << STOP", "expected_output": "CommanDC: cat\nArgumentDC 0: cat\nRedirection: <<\nDelimiter: STOP"},
    # Condiciones de error y mal uso
    {"id": 19, "description": "Error: Pipe sin comando después", "command": "echo hola |", "expected_output": "Errorsito: falta un comando después del pipe."},
    {"id": 20, "description": "Error: Redirección sin archivo", "command": "echo hola >", "expected_output": "Erroraso ZUPri!: falta archivo de redirección.\nCommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: hola"},
    # Test de variables especiales y secuencia de comandos
    {"id": 21, "description": "Expansión de PWD", "command": "echo $PWD", "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: <directorio_actual>"},
    {"id": 22, "description": "Comando con múltiples argumentos y redirección", "command": "ls -la / > listado.txt", "expected_output": "CommanDC: ls\nArgumentDC 0: ls\nArgumentDC 1: -la\nArgumentDC 2: /\nRedirection: >\nFile: listado.txt"},
    # Pruebas con comillas y concatenación de variables
    {"id": 23, "description": "Variable indefinida dentro de comillas", "command": 'echo "$UNDEFINED_VAR no existe"', "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1:  no existe"},
    {"id": 24, "description": "Expansión de múltiples variables en comillas dobles", "command": 'echo "Usuario $USER en $HOME"', "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: Usuario <nombre_usuario> en <home_dir>"},
    # Secuencia compleja con pipes, redirecciones y expansiones
    {"id": 25, "description": "Combinación compleja de pipes y redirecciones", "command": "echo start | grep s > resultado.txt | wc -l", "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: start\nCommanDC: grep\nArgumentDC 0: grep\nArgumentDC 1: s\nRedirection: >\nFile: resultado.txt\nCommanDC: wc\nArgumentDC 0: wc\nArgumentDC 1: -l"},
    {"id": 26, "description": "Pipeline con comandos consecutivos", "command": "echo test | grep t | wc -c", 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: test\nCommanDC: grep\nArgumentDC 0: grep\nArgumentDC 1: t\nCommanDC: wc\nArgumentDC 0: wc\nArgumentDC 1: -c"},

    {"id": 27, "description": "Expansión de dos variables en un argumento", "command": "echo $USER$HOME", 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: <nombre_usuario><home_dir>"},

    {"id": 28, "description": "Echo con redirección de salida y pipe", "command": "echo test > output.txt | wc -l", 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: test\nRedirection: >\nFile: output.txt\nCommanDC: wc\nArgumentDC 0: wc\nArgumentDC 1: -l"},

    {"id": 29, "description": "Error: Redirección sin comando", "command": "> output.txt", 
     "expected_output": "Error: redirección sin comando previo."},

    # Heredoc y delimitadores
    {"id": 30, "description": "Heredoc con delimitador en comillas simples", "command": "cat << 'END'", 
     "expected_output": "CommanDC: cat\nArgumentDC 0: cat\nRedirection: <<\nDelimiter: END"},

    {"id": 31, "description": "Heredoc con variable de entorno en delimitador", "command": "cat << $USER", 
     "expected_output": "CommanDC: cat\nArgumentDC 0: cat\nRedirection: <<\nDelimiter: <nombre_usuario>"},

    # Pruebas con redirección y pipes
    {"id": 32, "description": "Redirección de salida y pipe múltiple", "command": "echo hello > hello.txt | cat hello.txt | wc -c", 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: hello\nRedirection: >\nFile: hello.txt\nCommanDC: cat\nArgumentDC 0: cat\nArgumentDC 1: hello.txt\nCommanDC: wc\nArgumentDC 0: wc\nArgumentDC 1: -c"},

    {"id": 33, "description": "Expansión de variable dentro de heredoc", "command": "cat << $PWD", 
     "expected_output": "CommanDC: cat\nArgumentDC 0: cat\nRedirection: <<\nDelimiter: <directorio_actual>"},

    # Redirección con múltiples variables y escapes
    {"id": 34, "description": "Redirección con expansión y espacio escapado", "command": "echo \"$USER > archivo\" > user.txt", 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: <nombre_usuario> > archivo\nRedirection: >\nFile: user.txt"},

    # Caracteres especiales, manejo de comillas y escapes
    {"id": 35, "description": "Escape de comillas simples dentro de dobles", "command": "echo \"A 'simple' test\"", 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: A 'simple' test"},

    {"id": 36, "description": "Escape de comillas dobles dentro de simples", "command": "echo 'This \"works\" fine'", 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: This \"works\" fine"},

    {"id": 37, "description": "Redirección y variable con escape en heredoc", "command": "echo $USER >> user_data.txt", 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: <nombre_usuario>\nRedirection: >>\nFile: user_data.txt"},

    {"id": 38, "description": "Herencia de múltiples heredocs consecutivos", "command": "echo << STOP << ANOTHER",
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nRedirection: <<\nDelimiter: STOP\nRedirection: <<\nDelimiter: ANOTHER"},

    # Pruebas adicionales de variables de entorno en diferentes contextos
    {"id": 39, "description": "Expansión de HOME dentro de comillas dobles", "command": "echo \"$HOME directory\"", 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: <home_dir> directory"},

    {"id": 40, "description": "Variable en una línea de comando concatenada", "command": "echo $USER | wc -c", 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: <nombre_usuario>\nCommanDC: wc\nArgumentDC 0: wc\nArgumentDC 1: -c"},

    # A continuación, agregar casos variados que incluyan secuencias complejas y combinaciones:
    {"id": 41, "description": "Expansión de variable no existente en comando echo", "command": "echo $NOT_DEFINED", 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1:"},
    
    {"id": 42, "description": "Comillas dobles dentro de simples", "command": "echo '\"Dentro de comillas\"'", 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: \"Dentro de comillas\""},
    
    {"id": 43, "description": "Expansión de variables múltiples en un solo argumento", "command": "echo $USER-$PWD", 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: <nombre_usuario>-<directorio_actual>"},
    
    {"id": 44, "description": "Pipeline con grep y wc", "command": "echo 'Minishell test' | grep test | wc -w", 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: Minishell test\nCommanDC: grep\nArgumentDC 0: grep\nArgumentDC 1: test\nCommanDC: wc\nArgumentDC 0: wc\nArgumentDC 1: -w"},
    
    {"id": 45, "description": "Uso de caracteres especiales no permitidos", "command": "echo hola ; echo adiós", 
     "expected_output": "Error: Uso de caracteres especiales no permitidos (;)"},
    
    {"id": 46, "description": "Comando con redirección y variable no expandida", "command": "echo Hello > $FILE_PATH", 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: Hello\nRedirection: >\nFile: "},

    # Más ejemplos de redirección, herencia y salida
    {"id": 47, "description": "Redirección combinada y expansión variable", "command": "ls -la > $PWD/listado.txt", 
     "expected_output": "CommanDC: ls\nArgumentDC 0: ls\nArgumentDC 1: -la\nRedirection: >\nFile: <directorio_actual>/listado.txt"},
    
    {"id": 48, "description": "Error en redirección y falta de archivo", "command": "echo hola > ", 
     "expected_output": "Error: falta archivo de redirección."},

    {"id": 49, "description": "Redirección en comando con variable en argumento", "command": "echo Hola > $USER.txt", 
     "expected_output": "CommanDC: echo\nArgumentDC 0: echo\nArgumentDC 1: Hola\nRedirection: >\nFile: <nombre_usuario>.txt"},
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
