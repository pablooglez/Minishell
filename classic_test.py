import subprocess
import time
import os
import readline
import importlib.util
from datetime import datetime
import signal

# Colores ANSI para la terminal
COLOR_RESET = "\033[0m"
COLOR_GREEN = "\033[92m"
COLOR_RED = "\033[91m"
COLOR_BLUE = "\033[94m"
COLOR_YELLOW = "\033[93m"
COLOR_GRAY = "\033[90m"

# Separadores para el formato de salida
SEPARATOR_TOP = f"{COLOR_BLUE}╔{'═' * 100}╗{COLOR_RESET}"
SEPARATOR_MIDDLE = f"{COLOR_BLUE}╟{'─' * 100}╢{COLOR_RESET}"
SEPARATOR_BOTTOM = f"{COLOR_BLUE}╚{'═' * 100}╝{COLOR_RESET}"

# Contadores globales para los tests
test_number = 0
tests_passed = 0
tests_failed = 0
failed_tests = []

# Generar nombre único para el archivo de fallos
log_counter = 1
while os.path.exists(f"fallos_{log_counter}.txt"):
    log_counter += 1
failure_log_filename = f"test_fallidos_{log_counter}.txt"

# Función para ejecutar comandos en Bash o Minishell
def run_command(shell_cmd, shell_path):
    try:
        process = subprocess.Popen(
            shell_path,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            shell=False
        )
        output, error = process.communicate(shell_cmd + "\nexit\n")
        exit_code = process.returncode
        # Filtrar el prompt de Minishell
        filtered_output = "\n".join(line for line in output.splitlines() if "Minishell ➜" not in line)
        return filtered_output.strip(), error.strip(), exit_code
    except Exception as e:
        return "", str(e), 1

def compare_output(cmd):
    global tests_passed, tests_failed, test_number
    test_number += 1

    print(SEPARATOR_TOP)
    print(f"            ✨                    {COLOR_YELLOW}TEST NUMERO {test_number}: {COLOR_RESET}  {cmd}")
    print(SEPARATOR_MIDDLE)

    # Ejecutar el comando en Bash y Minishell
    bash_output, bash_error, bash_exit_code = run_command(cmd, "/bin/bash")
    minishell_output, minishell_error, minishell_exit_code = run_command(cmd, "./minishell")

    # Normalizar las salidas
    bash_output = "\n".join(line.strip() for line in bash_output.splitlines() if line.strip())
    minishell_output = "\n".join(line.strip() for line in minishell_output.splitlines() if line.strip())

    # Mostrar salidas estándar (stdout)
    print(f"{COLOR_YELLOW}Salidas estándar (stdout):{COLOR_RESET}")
    print(f"  Bash Output: {bash_output if bash_output else '<No Output>'}")
    print(f"  Minishell Output: {minishell_output if minishell_output else '<No Output>'}")

    # Mostrar códigos de salida y errores alineados horizontalmente
    print(f"\n{COLOR_YELLOW}Códigos de salida y errores:{COLOR_RESET}")
    print(f"  Bash Exit Code: {bash_exit_code:<3} | Minishell Exit Code: {minishell_exit_code:<3}")
    print(f"  Bash Error: {bash_error if bash_error else '<No Error>':<30} | Minishell Error: {minishell_error if minishell_error else '<No Error>'}")

    # Comparar resultados
    if bash_output == minishell_output and bash_exit_code == minishell_exit_code:
        print(f"{COLOR_GREEN}✅ Test PASADO{COLOR_RESET}")
        tests_passed += 1
    else:
        print(f"{COLOR_RED}❌ Test FALLADO{COLOR_RESET}")
        tests_failed += 1
        failed_tests.append(cmd)

    # Guardar detalles de los fallos
    if bash_output != minishell_output or bash_exit_code != minishell_exit_code:
        with open(failure_log_filename, "a") as f:
            f.write(f"TEST NUMERO {test_number}: {cmd}\n")
            f.write(f"Bash Output: {bash_output}\nMinishell Output: {minishell_output}\n")
            f.write(f"Bash Exit Code: {bash_exit_code}, Minishell Exit Code: {minishell_exit_code}\n")
            f.write(f"Bash Error: {bash_error if bash_error else '<No Error>'}\n")
            f.write(f"Minishell Error: {minishell_error if minishell_error else '<No Error>'}\n")
            f.write("-" * 80 + "\n")

    print(SEPARATOR_BOTTOM)



# Función para ejecutar un archivo de pruebas
def run_test_file(test_file):
    global tests_passed, tests_failed, failed_tests
    tests_passed = 0
    tests_failed = 0
    failed_tests = []

     # Verificar si el archivo está en la carpeta "tests/"
    if not os.path.isfile(test_file):
        test_file = f"./tests/{test_file}"
        if not os.path.isfile(test_file):
            print(f"{COLOR_RED}❌ Archivo de prueba '{test_file}' no encontrado.{COLOR_RESET}")
            return
        
    spec = importlib.util.spec_from_file_location("test_module", test_file)
    test_module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(test_module)

    if hasattr(test_module, "tests"):
        for cmd in test_module.tests:
            compare_output(cmd)

        print(SEPARATOR_TOP)
        print(f"📊 {COLOR_YELLOW}RESUMEN DE LOS TESTS EN {test_file}:{COLOR_RESET}")
        print(f"{COLOR_GREEN}✔️ Tests Pasados: {tests_passed}{COLOR_RESET}")
        print(f"{COLOR_RED}❌ Tests Fallados: {tests_failed}{COLOR_RESET}")

        if failed_tests:
            print(f"{COLOR_RED}Lista de Tests Fallidos guardada en '{failure_log_filename}'{COLOR_RESET}")

# Función para listar archivos de prueba
def list_test_files():
    test_files = [file for file in os.listdir("./tests") if file.startswith("test_") and file.endswith(".py")]
    for index, file in enumerate(test_files, start=1):
        print(f"  {index}. {file}")
    return test_files

# Modo interactivo para probar comandos
def interactive_shell():
    print(f"{COLOR_GREEN}Modo interactivo para Minishell. Escribe 'exit' para salir.{COLOR_RESET}")
    try:
        while True:
            cmd = input("Minishell ➜ ").strip()
            if cmd.lower() == "exit":
                break
            compare_output(cmd)
    except KeyboardInterrupt:
        print(f"{COLOR_RED}\nInterrupción detectada. Saliendo del modo interactivo.{COLOR_RESET}")

# Mostrar el menú principal
def show_menu():
    print(f"{COLOR_BLUE}Menú Principal:{COLOR_RESET}")
    print("1. Ejecutar un comando")
    print("2. Lista de archivos de prueba")
    print("3. Ejecutar archivo de prueba")
    print("4. Modo interactivo")
    print("5. Salir")

# Función principal
if __name__ == "__main__":
    while True:
        show_menu()
        choice = input("Seleccione una opción: ").strip()

        if choice == "1":
            cmd = input("Comando a probar: ").strip()
            compare_output(cmd)
        elif choice == "2":
            list_test_files()
        elif choice == "3":
            test_file = input("Nombre del archivo de prueba: ").strip()
            run_test_file(test_file)
        elif choice == "4":
            interactive_shell()
        elif choice == "5":
            print(f"{COLOR_GREEN}👋 Saliendo del comparador. ¡Adiós!{COLOR_RESET}")
            break
        else:
            print(f"{COLOR_RED}Opción inválida. Intente de nuevo.{COLOR_RESET}")
