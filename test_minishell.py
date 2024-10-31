import subprocess
import os

# Ruta al ejecutable de tu minishell
MINISHELL_PATH = "./minishell"  # Cambia esta ruta si el ejecutable está en otra ubicación

# Obtener el valor de $USER para comparación
USER = os.getenv("USER")

# Lista de comandos de prueba con salida esperada o comparación con bash
TEST_COMMANDS = [
    {
        "command": "echo Hello, World!",
        "expected_output": "Comando: echo\n  Argumento 0: Hello,\n  Argumento 1: World!\n"
    },
    {
        "command": "echo $USER",
        "expected_output": f"Comando: echo\n  Argumento 0: {USER}\n" if USER else None,
    },
    {
        "command": "invalidcommand",
        "expected_output": "command not found",
    },
    {
        "command": "echo 'single quote test'",
        "expected_output": "Comando: echo\n  Argumento 0: single quote test\n",
    },
    {
        "command": "echo \"double quote test\"",
        "expected_output": "Comando: echo\n  Argumento 0: double quote test\n",
    },
    {
        "command": "echo test > output.txt",
        "expected_output": "Comando: echo\n  Argumento 0: test\n  Redirección: salida a archivo: output.txt\n",
    },
    {
        "command": "echo test >",
        "expected_output": "Error: falta archivo o delimitador para la redirección.\n"
    },
    {
        "command": "ls | grep minishell",
        "expected_output": None,  # Compara con bash
    },
    {
        "command": "echo \"unmatched quote",
        "expected_output": "Error: comillas desbalanceadas\n",
    },
]

def run_command_in_shell(command, shell_path):
    """Ejecuta un comando en el shell especificado y captura la salida y los errores."""
    try:
        result = subprocess.run(
            [shell_path, "-c", command],
            text=True,
            capture_output=True,
            check=True,
            timeout=5  # Límite de 5 segundos para evitar bloqueos
        )
        return result.stdout, None
    except subprocess.CalledProcessError as e:
        return None, e.stderr  # Retorna solo los errores
    except subprocess.TimeoutExpired:
        return None, "Error: Timeout"

def compare_with_bash(command):
    """Ejecuta el comando en bash para comparar la salida con minishell."""
    bash_output, bash_error = run_command_in_shell(command, "/bin/bash")
    minishell_output, minishell_error = run_command_in_shell(command, MINISHELL_PATH)

    # Compara las salidas estándar
    if bash_output != minishell_output:
        print(f"Fallo en el comando: {command}")
        print(f"Esperado (bash): {bash_output}")
        print(f"Obtenido (minishell): {minishell_output}")
        return False

    # Compara los errores
    if bash_error != minishell_error:
        print(f"Fallo en el comando: {command}")
        print(f"Error esperado (bash): {bash_error}")
        print(f"Error obtenido (minishell): {minishell_error}")
        return False

    return True

def run_tests():
    """Ejecuta todas las pruebas en la lista de comandos de prueba."""
    all_passed = True
    for test in TEST_COMMANDS:
        command = test["command"]
        expected_output = test["expected_output"]

        print(f"\nEjecutando prueba para comando: {command}")
        
        # Ejecuta y verifica la salida
        minishell_output, minishell_error = run_command_in_shell(command, MINISHELL_PATH)
        
        if expected_output:
            # Comparación directa con la salida esperada
            if expected_output not in (minishell_output or minishell_error):
                print(f"Error: para el comando '{command}'")
                print(f"Se esperaba: '{expected_output}'")
                print(f"Obtenido: '{minishell_output or minishell_error}'")
                all_passed = False
        else:
            # Compara con bash para comandos sin salida esperada definida
            if not compare_with_bash(command):
                all_passed = False

    if all_passed:
        print("\nTodas las pruebas pasaron correctamente.")
    else:
        print("\nAlgunas pruebas fallaron.")

if __name__ == "__main__":
    run_tests()
