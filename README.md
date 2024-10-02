# minishell

<p align="center">
  <img src="https://github.com/mcombeau/mcombeau/blob/main/42_badges/minishelle.png" alt="Minishell 42 project badge"/>
</p>

Minishell is a 42 school team project to create a basic shell program in C. It implements redirections and pipes, as well as environment variable expansions and the `cd`, `echo`, `env`, `exit`, `export`, `pwd` and `unset` builtin commands.

## Supported Features

Minishell is a miniature shell program based on Bash. Minishell supports:
* Prompt display
* Command history (up and down arrows)
* System executables available from the environment (`ls`, `cat`, `grep`, etc.)
* Local executables (`./minishell`)
* Builtin commands :
  * `echo` (and option `-n`)
  * `cd` (with only a relative or absolute path)
  * `pwd` (no options)
  * `export` (no options)
  * `unset` (no options)
  * `env` (no options or arguments)
  * `exit` (with exit number but no other options) 
* Pipes `|` which redirect output from one command to input for the next
* Redirections:
  * `>` redirects output
  * `>>` redirects output in append mode
  * `<` redirects input
  * `<< DELIMITER` displays a new prompt, reads user input until reaching `DELIMITER`, redirects user input to command input (does not update history)
* Environment variables (i.e. `$USER` or `$VAR`) that expand to their values.
  * `$?` expands to the exit status of the most recently executed foreground pipeline.
* User keyboard signals:
  * `ctrl-c` displays a new prompt line.
  * `ctrl-d` exits minishell
  * `ctrl-\` does nothing

However, Minishell does not support `\`, `;`, `&&`, `||`, or wildcards.

## Mini-guide for branch usage
### Available Branches
- main
- dev-pablooglez
- dev-israelalcaide

### Branch commands
- Check available branches: git branch
- Create new branch: git branch name | Ex: git branch dev-israelalcaide
- Change branches: git checkout name | Ex: git checkout dev-pablooglez
- Update actual branch with main branch: git rebase main

### Things to do before starting / Update dev branch to be like main
- [ ] Check [Network Graph](https://github.com/pablooglez/minishell/network)
- [ ] git pull (on main and your dev branch)
- [ ] Go to your working dev branch using git checkout
- [ ] Update your dev branch to be up to date with main -> git rebase main


### Things to do after coding
- [ ] Add and commit each change (as specified as possible) - on the dev branch
- [ ] Push your commits
- [ ] Merge dev branch with main with a merge pull request on Github (or merge on local + push)
- [ ] git checkout main
- [ ] git pull (on main) to check changes
