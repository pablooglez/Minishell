import subprocess
from colorama import init, Fore, Style

# Inicializar colorama para soportar colores en la terminal
init()

def run_minishell_test(input_cmd, expected_parsing):
    """
    Ejecuta el minishell con un comando de entrada y compara el parsing real con el esperado.
    """
    try:
        # Usar subprocess para ejecutar el minishell
        process = subprocess.Popen(
            ["./minishell"],  # Suponiendo que el binario de tu minishell se llame "minishell"
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )

        # Enviar el comando al minishell
        stdout, stderr = process.communicate(input=input_cmd)

        # Verificar si el parsing es como se esperaba
        passed = expected_parsing in stdout
        return {
            "input": input_cmd.strip(),
            "expected": expected_parsing,
            "actual": stdout.strip(),
            "error": stderr.strip() if stderr else "",
            "passed": passed
        }

    except Exception as e:
        return {
            "input": input_cmd.strip(),
            "expected": expected_parsing,
            "actual": "",
            "error": str(e),
            "passed": False
        }

# Lista de pruebas para Parsing
test_cases = [
    # 1. Interpretación de Comillas
    ("echo 'Hello $USER'", "Argument: 'Hello $USER'"),
    ("echo \"Hello $USER\"", "Argument: Hello /home/israel"),  # Ajusta $USER al entorno
    ("echo \"Unclosed quote", "Error: comilla no cerrada"),

    # 2. Manejo de Caracteres Escapados
    ("echo hello\\ world", "Argument: hello world"),  # Ajusta para soportar escape si necesario

    # 3. Redirecciones
    ("cat < input.txt", "Redirection: < input.txt"),
    ("echo 'Hello' > output.txt", "Redirection: > output.txt"),
    ("echo 'Hello' >> output.txt", "Redirection: >> output.txt"),
    ("cat << END\nHello\nEND", "Redirection: << END"),

    # 4. Pipes
    ("echo 'hello' | grep hello", "Comando: echo Argument: 'hello' | Comando: grep Argument: hello"),
    ("ls -l | grep minishell | wc -l", "Comando: ls Argument: -l | Comando: grep Argument: minishell | Comando: wc Argument: -l"),

    # 5. Expansión de Variables de Entorno
    ("echo $HOME", "Argument: /home/israel"),  # Ajustar según tu entorno real
    ("export TESTVAR='my_test'\necho $TESTVAR", "Argument: my_test"),
    ("echo $?", "Argument: 0"),  # Basado en el último comando ejecutado

    # 6. Detección y Manejo de Errores
    ("echo 'Hello World", "Error: comilla no cerrada"),
    ("echo |", "Error: falta comando después del pipe"),
    ("echo > ", "Error: falta nombre de archivo después de >"),
    ("ls || echo 'error'", "Error: uso de operador || no permitido en parsing básico"),
]

# Ejecutar todas las pruebas
results = []
for cmd, expected in test_cases:
    result = run_minishell_test(cmd, expected)
    results.append(result)
    if result["passed"]:
        print(Fore.GREEN + f"✔ Test passed for input: {cmd.strip()}" + Style.RESET_ALL)
    else:
        print(Fore.RED + f"✘ Test failed for input: {cmd.strip()}" + Style.RESET_ALL)

# Resumen de resultados
failed_tests = [r for r in results if not r["passed"]]

print("\n" + "=" * 40)
print("Resumen de Pruebas")
print("=" * 40)

if not failed_tests:
    print(Fore.GREEN + "✔ Todos los tests han pasado." + Style.RESET_ALL)
else:
    print(Fore.RED + f"✘ {len(failed_tests)} tests fallaron:" + Style.RESET_ALL)
    for failed in failed_tests:
        print(Fore.RED + f"\nInput: {failed['input']}" + Style.RESET_ALL)
        print(Fore.RED + f"Expected Parsing: {failed['expected']}" + Style.RESET_ALL)
        print(Fore.RED + f"Actual Parsing: {failed['actual']}" + Style.RESET_ALL)
        if failed["error"]:
            print(Fore.RED + f"Error: {failed['error']}" + Style.RESET_ALL)
