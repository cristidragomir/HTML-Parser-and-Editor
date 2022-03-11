build:
	gcc SDTema3.c interpret.c init_tree.c tree_delete.c append_override.c -Wall -Wextra -g -o tema3
run:
	./tema3 input.html commands.in commands.out
clean:
	rm -rf *.o tema3
