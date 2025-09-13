CC = gcc
CFLAGS += -g

a.out : main.c mylib.c
	${CC} ${CFLAGS} -o $@ $^

.PHONY: clean
clean:
	@rm -rvf *.o
