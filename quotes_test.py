import subprocess
import re

# Rutas a tu minishell y bash
MINISHELL_PATH = "./minishell"
BASH_PATH = "/bin/bash"

# Definir los casos de prueba
tests = [
    ("echo 'hello world'", "Comillas simples"),
    ("echo \"hello world\"", "Comillas dobles"),
    ("echo '$USER'", "No expandir variable en comillas simples"),
    ("echo \"$USER\"", "Expandir variable en comillas dobles"),
    ("echo 'hola\"mundo'", "Comillas dobles dentro de comillas simples"),
    ("echo \"hola'mundo\"", "Comillas simples dentro de comillas dobles"),
    ("echo \"Escaped \\\"double quotes\\\"\"", "Escape de comillas dobles"),
    ("echo 'Escaped \\'single quotes\\''", "Escape de comillas simples"),
    ("echo 'Escaped \\$dollar sign'", "Escape de signo de dolar"),
    ("echo 'Escaped \\`backtick'", "Escape de acento grave"),
    ("echo 'Escaped \\\backslash'", "Escape de barra invertida"),
    ("echo 'Escaped \\nnewline'", "Escape de salto de línea"),
    ("echo 'Escaped \\ttab'", "Escape de tabulación"),
    ("echo 'Escaped \\rreturn'", "Escape de retorno de carro"),
    ("echo 'Escaped \\0null'", "Escape de carácter nulo"),
    ("echo 'Escaped \\aalert'", "Escape de alerta"),
    ("echo 'Escaped \\bbackspace'", "Escape de retroceso"),
    ("echo 'Escaped \\fform feed'", "Escape de avance de página"),
    ("echo 'Escaped \\vvertical tab'", "Escape de tabulación vertical"),
    ("echo 'Escaped \\eescape'", "Escape de escape"),
    ("echo 'Escaped \\?question mark'", "Escape de signo de interrogación"),
    ("echo 'Escaped \\!exclamation mark'", "Escape de signo de exclamación"),
    ("echo 'Escaped \\@at sign'", "Escape de signo de arroba"),
    ("echo 'Escaped \\#hash'", "Escape de almohadilla"),
    ("echo 'Escaped \\%percent sign'", "Escape de signo de porcentaje"),
    ("echo 'Escaped \\^caret'", "Escape de acento circunflejo"),
    ("echo 'Escaped \\&ampersand'", "Escape de signo de ampersand"),
    ("echo 'Escaped \\*asterisk'", "Escape de asterisco"),
    ("echo 'Escaped \\(open parenthesis'", "Escape de paréntesis de apertura"),
    ("echo 'Escaped \\)close parenthesis'", "Escape de paréntesis de cierre"),
    
        ("echo 'hello world'", "Comillas simples"),
    ("echo \"hello world\"", "Comillas dobles"),
    ("echo ''", "Comillas simples vacías"),
    ("echo \"\"", "Comillas dobles vacías"),
    ("echo 'hola \"mundo\"'", "Comillas dobles dentro de comillas simples"),
    ("echo \"hola 'mundo'\"", "Comillas simples dentro de comillas dobles"),
    ("echo '$USER'", "No expandir variable en comillas simples"),
    ("echo \"$USER\"", "Expandir variable en comillas dobles"),
    ("echo 'El usuario es $USER'", "No expandir variable dentro de comillas simples"),
    ("echo \"El usuario es $USER\"", "Expandir variable dentro de comillas dobles"),
    ("echo \"Escaped \\\"double quotes\\\"\"", "Escape de comillas dobles"),
    ("echo 'It\\'s a test'", "Escape de comillas simples"),
    ("echo \"\\$HOME\"", "Escape de expansión de variable en comillas dobles"),
    ("echo \"Hola $USER, tu directorio es $HOME\"", "Expansión de múltiples variables en comillas dobles"),
    ("echo '$USER $HOME'", "Variables no expandidas en comillas simples"),
    ("echo \"La ruta es: '$HOME'\"", "Comillas simples anidadas en comillas dobles con expansión"),
    ("echo 'El usuario es: \"$USER\"'", "Comillas dobles anidadas en comillas simples con expansión"),
    ("echo \"\\\"Comillas escapadas\\\"\"", "Comillas dobles escapadas dentro de comillas dobles"),
    ("echo '\\\"Comillas escapadas\\\"'", "Comillas dobles escapadas dentro de comillas simples"),
    ("echo '\"'", "Comillas dobles extremas dentro de comillas simples"),
    ("echo \"'\"", "Comillas simples extremas dentro de comillas dobles"),
    ("echo '\"Hola mundo\"'", "Comillas dobles completas dentro de comillas simples"),
    ("echo \"'Hola mundo'\"", "Comillas simples completas dentro de comillas dobles"),
    ("echo \"'Este es un texto' con 'comillas'\"", "Comillas simples dentro de comillas dobles"),
    ("echo '\"Este es un texto\" con \"comillas\"'", "Comillas dobles dentro de comillas simples"),
    ("echo '\"\"\"'", "Múltiples comillas dobles dentro de comillas simples"),
    ("echo \"'''''\"", "Múltiples comillas simples dentro de comillas dobles"),
    ("echo 'texto con \\\"escapado\\\"'", "Texto con comillas escapadas dentro de comillas simples"),
    ("echo \"texto con \\'escapado\\'\"", "Texto con comillas escapadas dentro de comillas dobles"),
    ("echo 'texto con \\$variable'", "Texto con variable escapada dentro de comillas simples"),
    ("echo \"texto con \\$variable\"", "Texto con variable escapada dentro de comillas dobles"),
    ("echo 'Hola'\" mundo\"", "Combinación de comillas simples y dobles"),
    ("echo \"Hola\"' mundo'", "Combinación de comillas dobles y simples"),
    ("echo \"hola 'mundo'\"", "Comillas simples dentro de comillas dobles, sin expansión"),
    ("echo 'hola \"mundo\"'", "Comillas dobles dentro de comillas simples, sin expansión"),
    ("echo 'Hola\\\"mundo'", "Escape de comillas dobles dentro de comillas simples"),
    ("echo \"Hola\\'mundo\"", "Escape de comillas simples dentro de comillas dobles"),
    ("echo 'El resultado es: \\$?'\"", "Escape del signo de interrogación dentro de comillas simples"),
    ("echo \"El resultado es: \\$?\"", "Escape del signo de interrogación dentro de comillas dobles"),
    ("echo \"Texto con comillas \\\"dobles\\\" y expansión: $USER\"", "Escape y expansión combinados dentro de comillas dobles"),
    ("echo 'Texto con comillas \\\"dobles\\\" y sin expansión: $USER'", "Escape dentro de comillas simples sin expansión"),
    ("echo 'Texto con \\`backticks\\` y $HOME'", "Backticks escapados dentro de comillas simples"),
    ("echo \"Texto con \\`backticks\\` y $HOME\"", "Backticks escapados dentro de comillas dobles"),
    ("echo 'Texto con \\(paréntesis\\)'", "Paréntesis escapados dentro de comillas simples"),
    ("echo \"Texto con \\(paréntesis\\)\"", "Paréntesis escapados dentro de comillas dobles"),
    ("echo 'Texto con \\[corchetes\\]'", "Corchetes escapados dentro de comillas simples"),
    ("echo \"Texto con \\[corchetes\\]\"", "Corchetes escapados dentro de comillas dobles"),
    ("echo 'Texto con \\{llaves\\}'", "Llaves escapadas dentro de comillas simples"),
    ("echo \"Texto con \\{llaves\\}\"", "Llaves escapadas dentro de comillas dobles"),
    ("echo 'Texto con múltiples \\escapes\\ y \\$variables'", "Múltiples escapes y variables dentro de comillas simples"),
    ("echo \"Texto con múltiples \\escapes\\ y \\$variables\"", "Múltiples escapes y variables dentro de comillas dobles"),
    ("echo 'Texto con \\$variables\\ no expandidas'", "Variables escapadas y no expandidas dentro de comillas simples"),
    ("echo \"Texto con \\$variables\\ expandidas: $HOME\"", "Variables escapadas y expandidas dentro de comillas dobles"),

    #expansion varibles comillas dobles y simples
    ("echo $USER", "Expandir variable sin comillas"),
    ("echo $USER $HOME", "Expandir múltiples variables sin comillas"),
    ("echo \"$USER\"", "Expandir variable en comillas dobles"),
    ("echo '$USER'", "No expandir variable en comillas simples"),
    ("echo \"$USER $HOME\"", "Expandir múltiples variables en comillas dobles"),
    
    # Expansión y manejo complejo de variables y comillas
("echo \"$USER is logged in on $(date)\"", "Expandir variable y ejecución de subshell en comillas dobles"),
("echo 'The current date is: $(date)'", "No expandir subshell en comillas simples"),
("echo \"The result is \\$(echo 123)\"", "Escape de subshell en comillas dobles"),
("echo 'The result is \\$(echo 123)'", "Escape de subshell en comillas simples"),

# Casos extremos de anidación de comillas
("echo \"'\"'\"'\"", "Anidación alterna extrema de comillas dobles y simples"),
("echo \"'Nested \"double\" quotes'\"", "Comillas dobles anidadas dentro de comillas simples"),
("echo '\"Nested 'single' quotes\"'", "Comillas simples anidadas dentro de comillas dobles"),

# Manejo de backticks y subshells
("echo `echo 'nested backticks'`", "Subshell con backticks dentro de comillas simples"),
("echo \"Output of `echo backtick test`\"", "Subshell con backticks dentro de comillas dobles"),
("echo 'Output of `echo backtick test`'", "Backticks escapados dentro de comillas simples"),

# Expansión de variables especiales
("echo \"The process ID is $$\"", "Expandir ID del proceso actual con comillas dobles"),
("echo 'The process ID is $$'", "No expandir ID del proceso con comillas simples"),
("echo \"Last command exit status: $?\"", "Expandir código de salida del último comando"),
("echo 'Last command exit status: $?'\"", "No expandir código de salida con comillas simples"),

# Casos complejos de escape
("echo \"Escaped \\\"nested \\\"double\\\" quotes\\\"\"", "Varios niveles de comillas dobles escapadas"),
("echo 'Escaped \\'nested \\'single\\' quotes\\''", "Varios niveles de comillas simples escapadas"),
("echo \"\\\\\\\"", "Triple barra invertida escapada"),
("echo '\\\\\\'", "Triple barra invertida escapada en comillas simples"),
("echo \"\\`Escaped backtick\\`\"", "Backtick escapado dentro de comillas dobles"),
("echo '`Escaped backtick`'", "Backtick escapado dentro de comillas simples"),

# Casos con metacaracteres y expansión
("echo \"Output: *\"", "Metacaracter * dentro de comillas dobles, no expandido"),
("echo '*'", "Metacaracter * dentro de comillas simples, no expandido"),
("echo \"Output: ?\"", "Metacaracter ? dentro de comillas dobles, no expandido"),
("echo '?'\"", "Metacaracter ? dentro de comillas simples, no expandido"),
("echo \"Path: ~/Documents\"", "Expandir tilde como ruta del home en comillas dobles"),
("echo '~/Documents'", "No expandir tilde en comillas simples"),

# Casos de redirección y tuberías
("echo 'output' | grep 'out'", "Redirección con tuberías y comillas simples"),
("echo \"output\" | grep \"out\"", "Redirección con tuberías y comillas dobles"),
("echo 'Searching * files' > 'output.txt'", "Redirección de salida con metacaracter escapado en comillas simples"),
("cat 'output.txt' | wc -l", "Lectura de archivo con redirección y comillas simples"),

# Casos de comillas no cerradas y errores de sintaxis
("echo 'This will fail because of unclosed quote", "Error de comillas simples no cerradas"),
("echo \"This will fail because of unclosed quote", "Error de comillas dobles no cerradas"),
("echo \"Incorrect 'quote nesting\"", "Error de anidación incorrecta de comillas"),
("echo 'Incorrect \"quote nesting'", "Error de anidación incorrecta de comillas"),

# Casos especiales con caracteres Unicode y escapes
("echo 'Texto con ñ y á'", "Manejo de caracteres especiales en comillas simples"),
("echo \"Texto con ü y ç\"", "Manejo de caracteres especiales en comillas dobles"),
("echo 'Emoji 😀 test'", "Manejo de emoji en comillas simples"),
("echo \"Emoji 😂 test\"", "Manejo de emoji en comillas dobles"),
("echo \"\\u263A Unicode smiley\"", "Escape de carácter Unicode en comillas dobles"),

# Casos con múltiples líneas y uso de `heredoc`
("cat <<EOF\nThis is a test with 'single quotes'\nEOF", "Heredoc con contenido de comillas simples"),
("cat <<EOF\nThis is a test with \"double quotes\"\nEOF", "Heredoc con contenido de comillas dobles"),
("cat <<EOF\nMixed content with $USER and `date`\nEOF", "Heredoc con variables y subshell"),

# Casos límite y estrés
("echo ' '", "Espacio dentro de comillas simples"),
("echo \" \"", "Espacio dentro de comillas dobles"),
("echo '\"\"\"\"\"'", "Múltiples comillas dobles en comillas simples"),
("echo \"''''''\"", "Múltiples comillas simples en comillas dobles"),
("echo '\"'", "Comilla doble dentro de comillas simples"),
("echo \"'\"", "Comilla simple dentro de comillas dobles"),

    


]

def clean_output(output):
    """Elimina el prompt y líneas vacías de la salida."""
    lines = output.splitlines()
    cleaned_lines = [line for line in lines if not re.match(r'.*Minishell ➜', line)]
    return "\n".join(cleaned_lines).strip()

def run_interactive_shell(shell_path, command):
    """Ejecuta el shell en modo interactivo y retorna la salida."""
    try:
        process = subprocess.Popen(
            shell_path,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
        )

        # Ejecutar el comando y capturar la salida en modo binario
        stdout, stderr = process.communicate(input=(command + "\nexit\n").encode(), timeout=5)

        # Decodificar la salida usando 'utf-8' con manejo de errores
        stdout = stdout.decode('utf-8', errors='replace')
        stderr = stderr.decode('utf-8', errors='replace')

        return clean_output(stdout + stderr)
    except subprocess.TimeoutExpired:
        return "ERROR: Timeout"

def run_test(command, description):
    """Ejecuta una prueba comparando la salida de minishell y bash."""
    minishell_output = run_interactive_shell(MINISHELL_PATH, command)
    bash_output = run_interactive_shell(BASH_PATH, command)

    if minishell_output == bash_output:
        print(f"✅ {description}: PASSED")
    else:
        print(f"❌ {description}: FAILED")
        print(f"    Comando:         {command}")
        print(f"    Minishell Output: {minishell_output}")
        print(f"    Bash Output:      {bash_output}")

# Ejecutar los tests
if __name__ == "__main__":
    print("Running interactive tests on ./minishell and comparing with bash...\n")
    for command, description in tests:
        run_test(command, description)
