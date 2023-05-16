# About validate.sh

- `xargs` is a command on Unix and most Unix-like operating systems used to build and execute commands from standard input.
- It converts input from standard input into arguments to a command.
- `xargs` also handles a large number of parameters that would go beyond the system's command line length limit.

`rm -r result answer`
`mkdir answer result`
`cp /home/share/hw2/* answer/`
`echo 1 2 3 | xargs -n 1 >> args`
- This command involves the use of xargs. The echo 1 2 3 outputs "1 2 3" to stdout.
- The >> args appends this output to the file named args. In the end, args contains 
three lines: "1", "2", and "3".
- '|' is a pipe. To send the output of one command to another command as input.
   For example, in echo "Hello World" | wc, echo "Hello World" generates a string, and wc (word count) takes this string as input and counts the words.
- xargs -n 1 reads from stdin, takes one argument at a time, and outputs it. 
- -n [number] example. `echo 1 2 3 4 5 | xargs -n 2` will print
  1 2
  3 4
  5

`cat args | xargs -I {} bash -c "./hw2 answer/hw2_test{}.csv > result/result_{}"`
- This command reads the contents of args and pipes it to xargs. xargs -I {} uses the    replace-str operator {}.
 For example:`echo "Hello" | xargs -I {} echo {} World` will print Hello World
- For each line in args, it runs bash -c "./hw2 answer/hw2_test{}.csv > result/result{}. In the bash command, {} is replaced by the line from args.

`cat args | xargs -I {} bash -c "diff result/result_{} answer/result_{}"`

