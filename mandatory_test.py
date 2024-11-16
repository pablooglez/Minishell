import subprocess
import time
import os
import readline
import importlib.util
from datetime import datetime

# Colores ANSI para la terminal
COLOR_RESET = "\033[0m"
COLOR_GREEN = "\033[92m"
COLOR_RED = "\033[91m"
COLOR_BLUE = "\033[94m"
COLOR_YELLOW = "\033[93m"
COLOR_GRAY = "\033[90m"

# Historial de comandos
command_history = []
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

def run_command(shell_cmd, shell_path):
    try:
        start_time = time.time()
        process = subprocess.run(
            shell_path,
            input=shell_cmd + "\nexit\n",
            capture_output=True,
            text=True,
            shell=False
        )
        end_time = time.time()

        output = process.stdout.strip()
        error = process.stderr.strip()
        exit_code = process.returncode
        elapsed_time = end_time - start_time

        filtered_output = "\n".join(
            line for line in output.splitlines() if "Minishell ➜" not in line
        )
        return filtered_output, error, exit_code, elapsed_time
    except Exception as e:
        return "", str(e), 1, 0

import difflib

def compare_output(cmd):
    global tests_passed, tests_failed, test_number
    test_number += 1

    print(SEPARATOR_TOP)
    print(f"            ✨                    {COLOR_YELLOW}TEST NUMERO {test_number}: {COLOR_RESET}  {cmd}")
    print(SEPARATOR_MIDDLE)

    # Ejecutar en Bash y Minishell
    bash_output, bash_error, bash_exit_code, bash_time = run_command(cmd, "/bin/bash")
    minishell_output, minishell_error, minishell_exit_code, minishell_time = run_command(cmd, "./minishell")

    # Normalizar outputs
    bash_output_normalized = "\n".join(line.strip() for line in bash_output.splitlines() if line.strip())
    minishell_output_normalized = "\n".join(line.strip() for line in minishell_output.splitlines() if line.strip())

    # Comparación horizontal
    bash_lines = bash_output_normalized.splitlines()
    minishell_lines = minishell_output_normalized.splitlines()
    max_lines = max(len(bash_lines), len(minishell_lines))

    print(f"{COLOR_GRAY} ╔{'─' * 48}╗{COLOR_RESET}{COLOR_GRAY}╔{'─' * 48}╗{COLOR_RESET}")
    print(f"                {'BASH OUTPUT':<28}                           {'MINISHELL OUTPUT':<28}")
    print(f"{COLOR_GRAY} ╟{'─' * 48}||{'─' * 48}╢{COLOR_RESET}")

    for i in range(max_lines):
        bash_line = bash_lines[i] if i < len(bash_lines) else "<No Output>"
        minishell_line = minishell_lines[i] if i < len(minishell_lines) else "<No Output>"
        print(f"  {bash_line:<28}                      {minishell_line:<28}")

    print(f"{COLOR_GRAY} ╚{'─' * 48}╝{COLOR_RESET}{COLOR_GRAY}╚{'─' * 48}╝{COLOR_RESET}")

    # Comparar resultados
    if bash_output_normalized == minishell_output_normalized and bash_exit_code == minishell_exit_code:
        result = "      ✅✅✅✅✅✅✅✅✅✅✅✅      PERFECTO CABESSA !!     ✅✅✅✅✅✅✅✅✅✅✅✅"
        color = COLOR_GREEN
        tests_passed += 1
    else:
        result = "   ❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌   ERROR ZUPRI !!!   ❌❌❌❌❌❌❌❌❌❌❌❌❌❌❌"
        color = COLOR_RED
        tests_failed += 1
        failed_tests.append(cmd)

        print(SEPARATOR_MIDDLE)
    print(f"{COLOR_GRAY} ╔{'─' * 20}╤{'─' * 20}╤{'─' * 56}╗{COLOR_RESET}")

    # Códigos de salida
    print(f"  {'🚨 CÓDIGO DE SALIDA':<20}{'⏱️  TIEMPO EJECUCIÓN ':<21}   ⚠️  {'ERRORES (stderr)⚠️':<35}")
    print(f"  {'BASH:':<10} {bash_exit_code:<9}  {'BASH:':<3}      {bash_time:.4f}s{' ' * 4}{bash_error if bash_error else '<No Error>':<35} ")
    print(f"  {'MINISHELL:':<10} {minishell_exit_code:<9}  {'Minishell:':<1} {minishell_time:.4f}s{' ' * 4}{minishell_error if minishell_error else '<No Error>':<35}")
    print(f"{COLOR_GRAY} ╚{'─' * 20}╧{'─' * 20}╧{'─' * 56}╝{COLOR_RESET}")


    print(SEPARATOR_MIDDLE)
    print(f"    {color}{result}{COLOR_RESET}")
    print(SEPARATOR_BOTTOM)
    print("·······································##############···············································")
    print("########################################  NEXT TEST ################################################")
    print("·······································##############···············································")

    # Guardar el fallo solo si hay diferencias
    if bash_output_normalized != minishell_output_normalized or bash_exit_code != minishell_exit_code:
        with open(failure_log_filename, "a") as f:
            f.write(f"TEST NUMERO {test_number}: {cmd}\n")
            f.write(f"  Bash Output: {bash_output_normalized}\n")
            f.write(f"  Minishell Output: {minishell_output_normalized}\n")
            f.write(f"  Bash Exit Code: {bash_exit_code}, Minishell Exit Code: {minishell_exit_code}\n")
            f.write(f"  Bash Error: {bash_error if bash_error else '<No Error>'}\n")
            f.write(f"  Minishell Error: {minishell_error if minishell_error else '<No Error>'}\n")
            f.write("-" * 80 + "\n")

def run_test_file(test_file):
    global tests_passed, tests_failed, failed_tests
    tests_passed = 0
    tests_failed = 0
    failed_tests = []

    spec = importlib.util.spec_from_file_location("test_module", test_file)
    test_module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(test_module)

    if hasattr(test_module, "tests"):
        for cmd in test_module.tests:
            compare_output(cmd)

        print(SEPARATOR_TOP)
        print(f"    📊 {COLOR_YELLOW}RESUMEN DE LOS TESTS EN {test_file}:{COLOR_RESET}")
        print(SEPARATOR_MIDDLE)
        print(f"    {COLOR_GREEN}✔️ Tests Pasados: {tests_passed}{COLOR_RESET}")
        print(f"    {COLOR_RED}❌ Tests Fallados: {tests_failed}{COLOR_RESET}")

        if failed_tests:
            print(f"\n{COLOR_RED}    Lista de Tests Fallidos guardada en '{failure_log_filename}'{COLOR_RESET}")
        else:
            print(f"{COLOR_GREEN}    ¡Todos los tests han pasado!{COLOR_RESET}")
        print(SEPARATOR_BOTTOM)

def list_test_files():
    print(f"{COLOR_BLUE}📂 TESTS DISPONIBLES:{COLOR_RESET}")
    test_files = [file for file in os.listdir("./tests") if file.startswith("test_") and file.endswith(".py")]
    
    for index, file in enumerate(test_files, start=1):
        print(f"  {index}. {file}")
    
    return test_files



def execute_all_tests():
    for file in os.listdir("./tests"):
        if file.startswith("test_") and file.endswith(".py"):
            print(f"{COLOR_YELLOW}\n🚀 Ejecutando archivo de prueba: {file}{COLOR_RESET}")
            run_test_file(f"./tests/{file}")

def execute_command_loop():
    while True:
        cmd = input(f"\n{COLOR_BLUE}  🖋️    ----- COMANDO A TESTEAR :{COLOR_RESET} ").strip()
        if cmd.lower() == "back":
            break
        compare_output(cmd)

def execute_test_loop():
    test_files = list_test_files()
    
    while True:
        choice = input(f"{COLOR_BLUE}  🔍 Ingresa el número del archivo de prueba o 'back' para regresar: {COLOR_RESET}").strip()
        
        if choice.lower() == "back":
            break
        
        if choice.isdigit():
            index = int(choice)
            if 1 <= index <= len(test_files):
                selected_file = test_files[index - 1]
                print(f"{COLOR_YELLOW}\n🚀 Ejecutando archivo de prueba: {selected_file}{COLOR_RESET}")
                run_test_file(f"./tests/{selected_file}")
            else:
                print(f"{COLOR_RED}❌ Número inválido. Por favor, selecciona un número válido.{COLOR_RESET}")
        else:
            print(f"{COLOR_RED}❌ Entrada inválida. Por favor, selecciona un número o escribe 'back'.{COLOR_RESET}")


def show_menu():
    print(f"\n{COLOR_BLUE}╔════════════════════════════════════════════════════╗{COLOR_RESET}")
    print(f"  🏠  {COLOR_YELLOW}       #M E N Ú   P R I N C I P A L     {COLOR_RESET}")
    print(f"{COLOR_BLUE}╟────────────────────────────────────────────────────╢{COLOR_RESET}")
    print(f"  1️⃣   ✨  {COLOR_GREEN}Ejecutar UN COMANDO a probar Cabessa{COLOR_RESET}")
    print(f"  2️⃣   📂  {COLOR_YELLOW}Lista de Archivos de Prueba{COLOR_RESET}")
    print(f"  3️⃣   🚀  {COLOR_YELLOW}Ejecutar UN TEST especifico{COLOR_RESET}")
    print(f"  4️⃣   🔍  {COLOR_YELLOW}Ejecutar TODOS los Tests a la vez{COLOR_RESET}")
    print(f"  5️⃣   ❌  {COLOR_RED}Salir{COLOR_RESET}")
    print(f"{COLOR_BLUE}╟────────────────────────────────────────────────────╢{COLOR_RESET}")
    print(f"        📝  Escribe 'back' para regresar.{COLOR_RESET}")
    print(f"{COLOR_BLUE}╚════════════════════════════════════════════════════╝{COLOR_RESET}")

if __name__ == "__main__":
    print("\n")
    print(f"{COLOR_GREEN}               👋 WELCOME TO 42 COMPARADOR CASERO DE MINISHELL 🐚     {COLOR_RESET}")
    print(f"{COLOR_YELLOW}                          📝 ALBELOPE AND PABLOGON.{COLOR_RESET}")

    while True:
        show_menu()
        choice = input(f"{COLOR_YELLOW}     🔽 Seleccione una opción -> {COLOR_RESET}").strip()

        if choice == "1":
            execute_command_loop()
        elif choice == "2":
            list_test_files()
        elif choice == "3":
            execute_test_loop()
        elif choice == "4":
            execute_all_tests()
        elif choice == "5":
            print(f"{COLOR_GREEN}👋 Saliendo del comparador. ¡Adiós!{COLOR_RESET}")
            break
        else:
            print(f"{COLOR_RED}❌ Opción inválida. Por favor, elige una opción del menú.{COLOR_RESET}")
