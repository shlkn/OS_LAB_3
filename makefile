command_shell: main.c 
	gcc -o command_shell -fsanitize=address -Wall main.c 

