import subprocess
import re
from colorama import Fore, Style, init

init(autoreset=True)

def test_tokenize(command, expected_tokens):
    print(f"Testing tokenization: {command}")
    result = subprocess.run(
        ['./minishell'], input=command, text=True, capture_output=True, timeout=5
    )
    output = result.stdout.strip()
    tokens = extract_tokens_from_output(output)
    
    if tokens == expected_tokens:
        print(Fore.GREEN + "PASSED")
    else:
        print(Fore.RED + f"FAILED. Expected tokens: {expected_tokens}, but got: {tokens}")

def extract_tokens_from_output(output):
    tokens = []
    lines = output.splitlines()
    for line in lines:
        token_match = re.search(r"Token\[\d+\]: '(.*?)'", line)
        if token_match:
            tokens.append(token_match.group(1))
    return tokens

def run_tokenize_tests():
    print("Running tokenize tests...")
    test_cases = [
        ("echo hello world", ["echo", "hello", "world"]),
        ("ls -la /home", ["ls", "-la", "/home"]),
        ("echo 'nested quotes'", ["echo", "nested quotes"]),
        ("cat < input.txt", ["cat", "<", "input.txt"]),
        ("echo \"double quotes\"", ["echo", "double quotes"]),
        ("echo 'single quotes'", ["echo", "single quotes"]),
        ("echo \"Escaped \\\" quotes\"", ["echo", "Escaped \" quotes"]),
        ("echo 'Escaped \\' quotes'", ["echo", "Escaped ' quotes"]),
        ("mkdir new_folder", ["mkdir", "new_folder"]),
        ("cd ..", ["cd", ".."]),
        ("ls | grep txt", ["ls", "|", "grep", "txt"]),
        ("echo hello > file.txt", ["echo", "hello", ">", "file.txt"]),
        ("echo goodbye >> file.txt", ["echo", "goodbye", ">>", "file.txt"]),
        ("cat < input.txt | sort", ["cat", "<", "input.txt", "|", "sort"]),
        ("echo $USER", ["echo", "$USER"]),
        ("echo $HOME", ["echo", "$HOME"]),
        ("unset $VAR", ["unset", "$VAR"]),
        ("ls; pwd", ["ls", ";", "pwd"]),
        ("export VAR=value", ["export", "VAR=value"]),
        ("echo 'multi line \n test'", ["echo", "multi line \n test"]),
        ("echo \"Hello; World\"", ["echo", "Hello; World"]),
        ("ls && echo done", ["ls", "&&", "echo", "done"]),
        ("echo hi || echo fail", ["echo", "hi", "||", "echo", "fail"]),
        ("rm -rf /tmp/test", ["rm", "-rf", "/tmp/test"]),
        ("touch \"new file.txt\"", ["touch", "new file.txt"])
    ]
    for command, expected in test_cases:
        test_tokenize(command, expected)

if __name__ == "__main__":
    run_tokenize_tests()
