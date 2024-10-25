import pexpect

def run_minishell_interactive(command):
    try:
        # Iniciar el minishell
        child = pexpect.spawn('./minishell')
        # Esperar que aparezca el prompt (ajusta si tu prompt es diferente)
        child.expect('Minishell>')
        # Enviar el comando
        child.sendline(command)
        # Esperar el resultado del comando
        child.expect('Minishell>')
        # Obtener la salida antes de que aparezca el próximo prompt
        output = child.before.decode().strip()
        # Cerrar el proceso
        child.sendline('exit')
        # Filtrar la salida para ignorar líneas adicionales de depuración
        lines = output.split("\n")
        filtered_output = []
        for line in lines:
            # Ignorar cualquier línea que comience con estos mensajes de depuración
            if not line.startswith("Iniciando") and not line.startswith("Procesando") and not line.startswith("Tokens") and not line.startswith("Lista de tokens") and not line.startswith("CommanDC") and not line.startswith("ArgumentDC") and not line.startswith("---") and not line.startswith("Inicializado"):
                filtered_output.append(line.strip())
        # Unir las líneas relevantes que no se filtraron
        return "\n".join(filtered_output)
    except Exception as e:
        print(f"Error al ejecutar el comando interactivo: {e}")
        return ""

def test_variable_expansion():
    # Test 1: Expansión básica de una variable de entorno
    output = run_minishell_interactive("echo $USER")
    expected = pexpect.run("echo $USER", shell=True).decode().strip()
    assert output == expected, f"Expected: '{expected}', but got: '{output}'"

    # Test 2: Variable de entorno dentro de comillas simples (no debería expandirse)
    output = run_minishell_interactive("echo '$USER'")
    expected = "$USER"
    assert output == expected, f"Expected: '{expected}', but got: '{output}'"

    # Test 3: Variable de entorno dentro de comillas dobles (debería expandirse)
    output = run_minishell_interactive('echo "$USER"')
    expected = pexpect.run('echo "$USER"', shell=True).decode().strip()
    assert output == expected, f"Expected: '{expected}', but got: '{output}'"

    # Test 4: Uso del código de salida con "$?"
    run_minishell_interactive("exit 42")  # Ejecutar minishell con código de salida 42
    output = run_minishell_interactive("echo $?")
    expected = "42"
    assert output == expected, f"Expected: '{expected}', but got: '{output}'"

    # Test 5: Variables múltiples en una sola línea
    output = run_minishell_interactive("echo $USER-$HOME")
    expected = pexpect.run("echo $USER-$HOME", shell=True).decode().strip()
    assert output == expected, f"Expected: '{expected}', but got: '{output}'"

    print("Todos los tests pasaron exitosamente.")

if __name__ == "__main__":
    test_variable_expansion()
