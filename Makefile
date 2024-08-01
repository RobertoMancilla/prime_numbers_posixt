all: prime_n_posix prime_numbers

prime_n_posix: prime_n_posix.c
	gcc -o prime_n_posix prime_n_posix.c

prime_numbers: prime_numbers.c
	gcc -o prime_numbers prime_numbers.c

clean:
	rm prime_n_posix prime_numbers