CC = gcc
CFLAGS += -g

tabstop : tabstop.c mylib.c
	${CC} ${CFLAGS} -o $@ $^

.PHONY: clean
clean:
	@rm -rvf *.o
