import subprocess
import re
from colorama import Fore, Style, init

init(autoreset=True)

def test_redirections(command, expected_redirections):
    print(f"Testing redirections: {command}")
    result = subprocess.run(
        ['./minishell'], input=command, text=True, capture_output=True, timeout=5
    )
    output = result.stdout.strip()
    redirections = extract_redirections_from_output(output)
    
    if redirections == expected_redirections:
        print(Fore.GREEN + "PASSED")
    else:
        print(Fore.RED + f"FAILED. Expected redirections: {expected_redirections}, but got: {redirections}")

def extract_redirections_from_output(output):
    redirections = []
    lines = output.splitlines()
    for line in lines:
        redirect_type_match = re.search(r"Redirection:\s(.*)", line)
        file_match = re.search(r"File:\s(.*)", line)
        if redirect_type_match and file_match:
            redirections.append((redirect_type_match.group(1).strip(), file_match.group(1).strip()))
    return redirections

def run_redirections_tests():
    print("Running redirections tests...")
    test_cases = [
        ("echo hello > output.txt", [(">", "output.txt")]),
        ("cat < input.txt", [("<", "input.txt")]),
        ("cat << heredoc", [("<<", "heredoc")]),
        ("echo goodbye >> file.txt", [(">>", "file.txt")]),
        ("grep pattern < input.txt", [("<", "input.txt")]),
        ("ls > /dev/null", [(">", "/dev/null")]),
        ("echo message 2> error.log", [("2>", "error.log")]),
        ("echo hello 1> output.txt", [("1>", "output.txt")]),
        ("cat < input.txt > output.txt", [("<", "input.txt"), (">", "output.txt")]),
        ("cat < input.txt | sort > sorted.txt", [("<", "input.txt"), (">", "sorted.txt")]),
        ("grep 'pattern' < input.txt", [("<", "input.txt")]),
        ("ls >> append.txt", [(">>", "append.txt")]),
        ("echo $USER > user.txt", [(">", "user.txt")]),
        ("echo $HOME > home_path.txt", [(">", "home_path.txt")]),
        ("cat << EOF", [("<<", "EOF")]),
        ("echo success > success.log 2> error.log", [(">", "success.log"), ("2>", "error.log")]),
        ("exec > all_output.txt 2>&1", [(">", "all_output.txt"), ("2>&1", "")]),
        ("cat < /etc/passwd", [("<", "/etc/passwd")]),
        ("echo 'redirect test' > test_output.txt", [(">", "test_output.txt")]),
        ("find . -name '*.py' > python_files.txt", [(">", "python_files.txt")]),
        ("tr a-z A-Z < input.txt > upper.txt", [("<", "input.txt"), (">", "upper.txt")]),
        ("wc -l < input.txt >> line_count.txt", [("<", "input.txt"), (">>", "line_count.txt")]),
        ("sort < unsorted.txt > sorted.txt", [("<", "unsorted.txt"), (">", "sorted.txt")]),
        ("uniq < file.txt > unique.txt", [("<", "file.txt"), (">", "unique.txt")])
    ]
    for command, expected in test_cases:
        test_redirections(command, expected)

if __name__ == "__main__":
    run_redirections_tests()
