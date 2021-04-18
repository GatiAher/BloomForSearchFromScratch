all: bloom

murmurhash.o: deps/murmurhash/murmurhash.c deps/murmurhash/murmurhash.h
	gcc -c deps/murmurhash/murmurhash.c

bloom.o: src/bloom.c src/bloom.h
	gcc -c src/bloom.c

try_bloom.o: src/try_bloom.c src/try_bloom.h
	gcc -c src/try_bloom.c

bloom: src/main.c src/main.h try_bloom.o bloom.o murmurhash.o
	gcc -o bloom src/main.c try_bloom.o bloom.o murmurhash.o

try_murmurhash: src/try_murmurhash.c deps/murmurhash/*
	gcc src/try_murmurhash.c deps/murmurhash/murmurhash.c

clean:
	rm bloom *.o

clean_all:
	rm bloom try_murmurhash