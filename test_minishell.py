import subprocess

# Definir las pruebas como un diccionario de comandos y resultados esperados
pruebas = {
    # 1-10: Comandos básicos
    "echo Hello": ["Command: echo", "Argument 0: echo", "Argument 1: Hello"],
    "ls -la": ["Command: ls", "Argument 0: ls", "Argument 1: -la"],
    "pwd": ["Command: pwd", "Argument 0: pwd"],
    "whoami": ["Command: whoami", "Argument 0: whoami"],
    "date": ["Command: date", "Argument 0: date"],
    "uptime": ["Command: uptime", "Argument 0: uptime"],
    "hostname": ["Command: hostname", "Argument 0: hostname"],
    "uname -a": ["Command: uname", "Argument 0: uname", "Argument 1: -a"],
    "cal": ["Command: cal", "Argument 0: cal"],
    "echo This is a test": ["Command: echo", "Argument 0: echo", "Argument 1: This is a test"],
    
    # 11-20: Expansión de Variables de Entorno
    "echo $HOME": ["Command: echo", "Argument 0: echo", "Argument 1: /home/israel"],
    "echo $USER": ["Command: echo", "Argument 0: echo", "Argument 1: israel"],
    "echo $SHELL": ["Command: echo", "Argument 0: echo", "Argument 1: /bin/bash"],
    "echo $?": ["Command: echo", "Argument 0: echo", "Argument 1: 0"],
    "echo $PATH": ["Command: echo", "Argument 0: echo"],
    'echo "User: $USER"': ["Command: echo", "Argument 0: echo", "Argument 1: User: israel"],
    'echo "Home directory: $HOME"': ["Command: echo", "Argument 0: echo", "Argument 1: Home directory: /home/israel"],
    "echo '$USER'": ["Command: echo", "Argument 0: echo", "Argument 1: $USER"],
    "echo '$HOME'": ["Command: echo", "Argument 0: echo", "Argument 1: $HOME"],
    "echo '$?'": ["Command: echo", "Argument 0: echo", "Argument 1: $?"],
    
    # 21-30: Redirecciones
    "echo Hello > file.txt": ["Command: echo", "Argument 0: echo", "Argument 1: Hello", "Redirection: >", "File: file.txt"],
    "cat < input.txt": ["Command: cat", "Argument 0: cat", "Redirection: <", "File: input.txt"],
    "echo Append >> file.txt": ["Command: echo", "Argument 0: echo", "Argument 1: Append", "Redirection: >>", "File: file.txt"],
    "cat << EOF": ["Command: cat", "Argument 0: cat", "Redirection: <<", "File: EOF"],
    "ls > list.txt": ["Command: ls", "Argument 0: ls", "Redirection: >", "File: list.txt"],
    "grep error < log.txt": ["Command: grep", "Argument 0: grep", "Argument 1: error", "Redirection: <", "File: log.txt"],
    "echo 'output text' > out.txt": ["Command: echo", "Argument 0: echo", "Argument 1: output text", "Redirection: >", "File: out.txt"],
    "sort < data.txt": ["Command: sort", "Argument 0: sort", "Redirection: <", "File: data.txt"],
    "echo Hello > first.txt >> second.txt": [
        "Command: echo", "Argument 0: echo", "Argument 1: Hello",
        "Redirection: >", "File: first.txt", "Redirection: >>", "File: second.txt"
    ],
    "cat < input.txt > output.txt": [
        "Command: cat", "Argument 0: cat",
        "Redirection: <", "File: input.txt",
        "Redirection: >", "File: output.txt"
    ],

    # 31-40: Pipes
    "echo Hello | grep H": [
        "Command: echo", "Argument 0: echo", "Argument 1: Hello",
        "Command: grep", "Argument 0: grep", "Argument 1: H"
    ],
    "ls -l | grep .py": [
        "Command: ls", "Argument 0: ls", "Argument 1: -l",
        "Command: grep", "Argument 0: grep", "Argument 1: .py"
    ],
    "echo 'literal text' | cat": [
        "Command: echo", "Argument 0: echo", "Argument 1: literal text",
        "Command: cat", "Argument 0: cat"
    ],
    "ps aux | grep python | wc -l": [
        "Command: ps", "Argument 0: ps", "Argument 1: aux",
        "Command: grep", "Argument 0: grep", "Argument 1: python",
        "Command: wc", "Argument 0: wc", "Argument 1: -l"
    ],
    'echo "user: $USER" | grep israel': [
        "Command: echo", "Argument 0: echo", "Argument 1: user: israel",
        "Command: grep", "Argument 0: grep", "Argument 1: israel"
    ],
    "ls | sort | uniq": [
        "Command: ls", "Argument 0: ls",
        "Command: sort", "Argument 0: sort",
        "Command: uniq", "Argument 0: uniq"
    ],
    "who | grep $(whoami)": [
        "Command: who", "Argument 0: who",
        "Command: grep", "Argument 0: grep", "Argument 1: israel"
    ],
    "df -h | grep '/dev/'": [
        "Command: df", "Argument 0: df", "Argument 1: -h",
        "Command: grep", "Argument 0: grep", "Argument 1: /dev/"
    ],
    "echo Hello | tee output.txt": [
        "Command: echo", "Argument 0: echo", "Argument 1: Hello",
        "Command: tee", "Argument 0: tee", "Argument 1: output.txt"
    ],
    "echo Hello | grep H | wc -l": [
        "Command: echo", "Argument 0: echo", "Argument 1: Hello",
        "Command: grep", "Argument 0: grep", "Argument 1: H",
        "Command: wc", "Argument 0: wc", "Argument 1: -l"
    ],

    # 41-50: Manejo de Comillas y Texto Complejo
    "echo 'Single quote'": ["Command: echo", "Argument 0: echo", "Argument 1: Single quote"],
    'echo "Double quote"': ["Command: echo", "Argument 0: echo", "Argument 1: Double quote"],
    'echo "Mix $HOME and $USER"': ["Command: echo", "Argument 0: echo", "Argument 1: Mix /home/israel and israel"],
    "echo 'Literal $HOME $USER'": ["Command: echo", "Argument 0: echo", "Argument 1: Literal $HOME $USER"],
    'echo "$USER is logged in"': ["Command: echo", "Argument 0: echo", "Argument 1: israel is logged in"],
    'echo "No expansion: \$HOME"': ["Command: echo", "Argument 0: echo", "Argument 1: No expansion: $HOME"],
    'echo "User is: $USER" | cat': [
        "Command: echo", "Argument 0: echo", "Argument 1: User is: israel",
        "Command: cat", "Argument 0: cat"
    ],
    'echo "\'Escaped\' quotes"': ["Command: echo", "Argument 0: echo", "Argument 1: 'Escaped' quotes"],
    'echo "\"Double inside single\" test"': ["Command: echo", "Argument 0: echo", "Argument 1: \"Double inside single\" test"],
    "echo 'Spaces should stay intact'": ["Command: echo", "Argument 0: echo", "Argument 1: Spaces should stay intact"],

    # 51-100: Pruebas Extensivas y de Errores

    # Agregue casos adicionales que involucran múltiples redirecciones, pipes,
    # comillas no balanceadas, comillas escapadas, diferentes tipos de variables
    # más comandos complejos combinados

    # Algunos ejemplos adicionales:
    "echo Hello; echo Bye": [
        "Command: echo", "Argument 0: echo", "Argument 1: Hello",
        "Command: echo", "Argument 0: echo", "Argument 1: Bye"
    ],
    "echo 'Nested `echo Hello`'": [
        "Command: echo", "Argument 0: echo", "Argument 1: Nested `echo Hello`"
    ],
    "export VAR='Some Value'": ["Command: export", "Argument 0: export", "Argument 1: VAR=Some Value"],
    "cat | cat | cat": ["Command: cat", "Argument 0: cat", "Command: cat", "Argument 0: cat", "Command: cat", "Argument 0: cat"],
    "| echo test": ["Error: unexpected token `|'"],
    "echo \"Escape quote \\\" should work\"": [
        "Command: echo", "Argument 0: echo", "Argument 1: Escape quote \" should work"
    ]
}

# Función para ejecutar el minishell y probar los comandos
def run_test(command, expected_output):
    # Abrir el minishell
    process = subprocess.Popen(["./minishell"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

    # Enviar el comando al minishell
    output, _ = process.communicate(input=command + "\nexit\n")

    # Dividir la salida por líneas y eliminar espacios extra
    output_lines = [line.strip() for line in output.strip().split("\n")]

    # Verificar si la salida es como se esperaba
    if all(item in output_lines for item in expected_output):
        print(f"✅ Test passed for: {command}")
    else:
        print(f"❌ Test failed for: {command}")
        print(f"Expected: {expected_output}")
        print(f"Got: {output_lines}")

# Ejecutar todas las pruebas
def run_all_tests():
    for command, expected_output in pruebas.items():
        run_test(command, expected_output)

if __name__ == "__main__":
    run_all_tests()
