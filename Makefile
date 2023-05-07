all:
	@gcc -Wall -g -o test *.c
run:
	@./test
clean:
	@rm -f test
valgrind:
	@valgrind --leak-check=full ./test
gdb:
	@gdb ./test