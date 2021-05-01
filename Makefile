# murmurhash
murmurhash.o: deps/murmurhash/murmurhash.c deps/murmurhash/murmurhash.h
	gcc -c deps/murmurhash/murmurhash.c

test_murmurhash: bloom/src/test_murmurhash.c murmurhash.o
	gcc -Wpedantic -Wall -Wextra -o test_murmurhash bloom/src/test_murmurhash.c murmurhash.o

# bloom
bloom.o: bloom/src/bloom.c bloom/src/bloom.h murmurhash.o
	gcc -c bloom/src/bloom.c

test_bloom: bloom/src/test_bloom_main.c bloom/src/test_bloom_main.h bloom/src/test_bloom.c bloom/src/test_bloom.h bloom.o
	gcc -Wpedantic -Wall -Wextra -o test_bloom bloom/src/test_bloom_main.c bloom/src/test_bloom.c bloom.o murmurhash.o

run_test_bloom: test_bloom
	./test_bloom -a bloom/data/in_test.txt -c bloom/data/out_test.txt -m 60 -k 3

prog_build_bloom: bloom/src/build_bloom_main.c bloom/src/build_bloom_main.h bloom/src/build_bloom.c bloom/src/build_bloom.h bloom.o
	gcc -Wpedantic -Wall -Wextra -o prog_build_bloom bloom/src/build_bloom_main.c bloom/src/build_bloom.c bloom.o murmurhash.o

run_build_bloom: prog_build_bloom
	./prog_build_bloom

# bitslicedsig
prog_bitslicedsig: bitslicedsig/src/main.c bitslicedsig/src/main.h bitslicedsig/src/try_bitslicedsig.c bitslicedsig/src/try_bitslicedsig.h bitslicedsig/src/bitslicedsig.c bitslicedsig/src/bitslicedsig.h murmurhash.o
	gcc -Wpedantic -Wall -Wextra -o prog_bitslicedsig bitslicedsig/src/main.c bitslicedsig/src/try_bitslicedsig.c bitslicedsig/src/bitslicedsig.c murmurhash.o

clean:
	rm *.o murmurhash.o test_* prog_*



