.PHONY: clean run compile
run: shell
	@./shell $(ARGS)
compile shell: shell.o main.o
	@gcc -o shell -Wall shell.o main.o
main.o: main.c shell.h
	@gcc -c main.c
shell.o: shell.c
	@gcc -c shell.c
clean:
	rm *.o
	rm shell