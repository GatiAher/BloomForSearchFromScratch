# BloomForSearchFromScratch

## Choice of Hash Function:

The hash function used in a bloom filter must be independent, uniformly distributed, and as fast as possible.

MurmurHash is a non-cryptographic hash function suitable for general hash-based lookup. 

In this project, I use [jwerle/murmurhash](https://fuchsia.googlesource.com/third_party/murmurhash.c/) which implements version 3 of MurmurHash.