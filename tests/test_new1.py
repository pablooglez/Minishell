tests = [
    "", "Minishell: command not found", 127,
    "   ", "Minishell: command not found", 127,
    "echo 'Unclosed quote", "Minishell: syntax error", 2,
    "echo \"Unclosed quote", "Minishell: syntax error", 2,
    "| echo test", "Minishell: syntax error", 2,
    "echo test |", "Minishell: syntax error", 2,
    ">", "Minishell: syntax error", 2,
    "<", "Minishell: syntax error", 2,
    "echo test >", "Minishell: syntax error", 2,
    "echo test <", "Minishell: syntax error", 2,
    "echo 'Hello World'", "Hello World", 0,
    "echo \"Hello World\"", "Hello World", 0,
    "echo ''", "", 0,
    "echo \"\"", "", 0,
    "cd nonexistent_directory", "Minishell: cd: nonexistent_directory: No such file or directory", 1,
    "exit invalid_argument", "Minishell: exit: invalid_argument: numeric argument required", 2,
    "pwd", "/current/directory/path", 0
]
