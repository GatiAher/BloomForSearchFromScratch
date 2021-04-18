all: try_murmurhash

try_murmurhash: src/try_murmurhash.c deps/murmurhash/*
	gcc src/try_murmurhash.c deps/murmurhash/murmurhash.c -o try_murmurhash

clean:
	rm try_murmurhash