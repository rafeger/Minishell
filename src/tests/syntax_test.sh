#!/bin/bash

# Adjust these to your actual test binary or test harness
SYNTAX1="./minishell_syntax1" # should call src/syntax/check_syntax_core.c
SYNTAX2="./minishell_syntax2" # should call src/syntax2/check_syntax_core.c

# If you don't have separate binaries, you can temporarily swap out the files and recompile between runs.

tests=(
    ""                                  # Empty input
    "|"                                 # Pipe at start
    "ls |"                              # Pipe at end
    "ls || wc"                          # Double pipe
    "ls | | wc"                         # Pipe with space
    ">"                                 # Redir at start
    "ls >"                              # Redir at end
    "ls > file"                         # Valid redir
    "ls >>> file"                       # Too many redirs
    "ls >> file"                        # Valid append
    "ls >< file"                        # Invalid sequence
    "ls < > file"                       # Invalid sequence
    "ls > | wc"                         # Redir followed by pipe
    "ls >    | wc"                      # Redir followed by spaces then pipe
    "ls >file"                          # Valid
    "ls '>' file"                       # Redir in quotes
    "ls \"<\" file"                     # Redir in quotes
    "ls >"                              # Redir at end
    "ls >    "                          # Redir with trailing spaces
    "ls < infile > outfile"             # Valid
    "ls << EOF"                         # Valid heredoc
    "ls <<< file"                       # Too many heredoc
    "ls >file1 >file2"                  # Multiple redirs
    "ls < infile | grep foo"            # Valid
    "ls | grep foo | wc"                # Valid
    "ls >file |"                        # Redir then pipe at end
    "ls >file | wc"                     # Redir then pipe
    "ls >file | | wc"                   # Redir then double pipe
    "ls >file < infile"                 # Multiple redirs
    "ls >file < infile | wc"            # Multiple redirs with pipe
)

for input in "${tests[@]}"; do
    echo "Test: '$input'"
    echo -n "syntax1: "
    echo "$input" | $SYNTAX1
    ret1=$?
    echo -n "syntax2: "
    echo "$input" | $SYNTAX2
    ret2=$?
    if [ "$ret1" != "$ret2" ]; then
        echo "==> Exit codes differ: $ret1 vs $ret2"
    fi
    echo "-----------------------------"
done