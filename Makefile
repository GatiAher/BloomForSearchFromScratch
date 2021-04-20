all: run_bloom run_bitslicedsig

murmurhash.o: deps/murmurhash/murmurhash.c deps/murmurhash/murmurhash.h
	gcc -c deps/murmurhash/murmurhash.c

try_murmurhash: bloom/src/try_murmurhash.c murmurhash.o
	gcc -Wpedantic -Wall -Wextra -o try_murmurhash bloom/src/try_murmurhash.c murmurhash.o

run_bloom: bloom/src/main.c bloom/src/main.h bloom/src/try_bloom.c bloom/src/try_bloom.h bloom/src/bloom.c bloom/src/bloom.h murmurhash.o
	gcc -Wpedantic -Wall -Wextra -o run_bloom bloom/src/main.c bloom/src/try_bloom.c bloom/src/bloom.c murmurhash.o

run_bitslicedsig: bitslicedsig/src/main.c bitslicedsig/src/main.h bitslicedsig/src/try_bitslicedsig.c bitslicedsig/src/try_bitslicedsig.h bitslicedsig/src/bitslicedsig.c bitslicedsig/src/bitslicedsig.h murmurhash.o
	gcc -Wpedantic -Wall -Wextra -o run_bitslicedsig bitslicedsig/src/main.c bitslicedsig/src/try_bitslicedsig.c bitslicedsig/src/bitslicedsig.c murmurhash.o

clean:
	rm *.o run_bitslicedsig run_bloom try_murmurhash



