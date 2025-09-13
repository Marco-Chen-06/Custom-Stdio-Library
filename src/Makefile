CC = gcc

a.out : main.c mylib.c
	${CC} -o $@ $^

.PHONY: clean
clean:
	@rm -rvf *.o
