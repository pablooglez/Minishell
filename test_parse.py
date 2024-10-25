import subprocess
import re
from colorama import Fore, Style, init

init(autoreset=True)

def test_parse(command, expected_command, expected_args):
    print(f"Testing parse: {command}")
    result = subprocess.run(
        ['./minishell'], input=command, text=True, capture_output=True, timeout=5
    )
    output = result.stdout.strip()
    parsed_command, parsed_args = extract_parsed_info(output)
    
    if parsed_command == expected_command and parsed_args == expected_args:
        print(Fore.GREEN + "PASSED")
    else:
        print(Fore.RED + f"FAILED. Expected command: {expected_command} with args: {expected_args}, but got: {parsed_command} with args: {parsed_args}")

def extract_parsed_info(output):
    command = None
    args = []
    lines = output.splitlines()
    for line in lines:
        command_match = re.search(r"Command:\s*(.*)", line)
        if command_match:
            command = command_match.group(1).strip()

        arg_match = re.search(r"Argument\s\d+:\s(.*)", line)
        if arg_match:
            args.append(arg_match.group(1).strip())

    return command, args

def run_parse_tests():
    print("Running parse tests...")
    test_cases = [
        ("echo hello world", "echo", ["echo", "hello", "world"]),
        ("ls -la", "ls", ["ls", "-la"]),
        ("cat < input.txt", "cat", ["cat"]),
        ("echo \"Hello World\"", "echo", ["echo", "Hello World"]),
        ("echo 'single quoted'", "echo", ["echo", "single quoted"]),
        ("echo $USER", "echo", ["echo", "$USER"]),
        ("grep 'pattern' file.txt", "grep", ["grep", "pattern", "file.txt"]),
        ("pwd", "pwd", ["pwd"]),
        ("mkdir new_folder", "mkdir", ["mkdir", "new_folder"]),
        ("export PATH=/usr/bin", "export", ["export", "PATH=/usr/bin"]),
        ("unset VAR", "unset", ["unset", "VAR"]),
        ("ls | grep .txt", "ls", ["ls"]),
        ("echo hello | cat", "echo", ["echo", "hello"]),
        ("cat << heredoc", "cat", ["cat"]),
        ("echo 'Escaped \\' quotes'", "echo", ["echo", "Escaped ' quotes"]),
        ("echo \"Escaped \\\" quotes\"", "echo", ["echo", "Escaped \" quotes"]),
        ("ls && echo success", "ls", ["ls"]),
        ("echo success || echo fail", "echo", ["echo", "success"]),
        ("echo 'multi line \n test'", "echo", ["echo", "multi line \n test"]),
        ("ls; pwd", "ls", ["ls"]),
        ("rm -rf /", "rm", ["rm", "-rf", "/"]),
        ("date", "date", ["date"]),
        ("kill -9 1234", "kill", ["kill", "-9", "1234"]),
        ("man ls", "man", ["man", "ls"]),
        ("echo $PATH", "echo", ["echo", "$PATH"])
    ]
    for command, expected_cmd, expected_args in test_cases:
        test_parse(command, expected_cmd, expected_args)

if __name__ == "__main__":
    run_parse_tests()
