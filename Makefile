all: run_bloom

murmurhash.o: deps/murmurhash/murmurhash.c deps/murmurhash/murmurhash.h
	gcc -c deps/murmurhash/murmurhash.c

try_murmurhash: bloom/try_murmurhash.c murmurhash.o
	gcc -Wpedantic -Wall -Wextra -o try_murmurhash bloom/try_murmurhash.c murmurhash.o

run_bloom: bloom/main.c bloom/main.h bloom/try_bloom.c bloom/try_bloom.h bloom/bloom.c bloom/bloom.h murmurhash.o
	gcc -Wpedantic -Wall -Wextra -o run_bloom bloom/main.c bloom/try_bloom.c bloom/bloom.c murmurhash.o

clean:
	rm *.o run_bloom try_murmurhash



