all: bloom

try_murmurhash: src/try_murmurhash.c deps/murmurhash/*
	gcc src/try_murmurhash.c deps/murmurhash/murmurhash.c -o try_murmurhash

bloom: src/main.c src/bloom.c src/bloom.h deps/murmurhash/* 
	gcc src/main.c src/bloom.c deps/murmurhash/murmurhash.c -o bloom

clean:
	rm bloom

clean_all:
	rm bloom try_murmurhash