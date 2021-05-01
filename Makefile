all: test_murmurhash test_bloom run_bitslicedsig

# murmurhash
murmurhash.o: deps/murmurhash/murmurhash.c deps/murmurhash/murmurhash.h
	gcc -c deps/murmurhash/murmurhash.c

test_murmurhash: bloom/src/test_murmurhash.c murmurhash.o
	gcc -Wpedantic -Wall -Wextra -o test_murmurhash bloom/src/test_murmurhash.c murmurhash.o

# bloom
test_bloom: bloom/src/main.c bloom/src/main.h bloom/src/test_bloom.c bloom/src/test_bloom.h bloom/src/bloom.c bloom/src/bloom.h murmurhash.o
	gcc -Wpedantic -Wall -Wextra -o test_bloom bloom/src/main.c bloom/src/test_bloom.c bloom/src/bloom.c murmurhash.o

# bitslicedsig
run_bitslicedsig: bitslicedsig/src/main.c bitslicedsig/src/main.h bitslicedsig/src/try_bitslicedsig.c bitslicedsig/src/try_bitslicedsig.h bitslicedsig/src/bitslicedsig.c bitslicedsig/src/bitslicedsig.h murmurhash.o
	gcc -Wpedantic -Wall -Wextra -o run_bitslicedsig bitslicedsig/src/main.c bitslicedsig/src/try_bitslicedsig.c bitslicedsig/src/bitslicedsig.c murmurhash.o

clean:
	rm *.o murmurhash.o test_* run_*  



