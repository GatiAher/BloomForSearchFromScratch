# BloomForSearchFromScratch

## Choice of Hash Function:

The hash function used in a bloom filter must be independent, uniformly distributed, and as fast as possible.

MurmurHash is a non-cryptographic hash function suitable for general hash-based lookup. In this project, I use [jwerle/murmurhash](https://fuchsia.googlesource.com/third_party/murmurhash.c/) which implements version 3 of MurmurHash.

You can try out murmurhash with
```
make test_murmurhash && ./test_murmurhash
```

## test_bloom

Run `make test_bloom` to compile an executable named `test_bloom`. Call `test_bloom` with the following arguments:

```
bloom [-v] [-a file_words_to_add_to_bloom] [-c file_words_to_check_in_bloom] [-m number_of_bits] [-k number_of_hash_functions] [-h]
```

**Example:**

```
make test_bloom && ./test_bloom -a bloom/data/in_test.txt -c bloom/data/out_test.txt -m 60 -k 3
```

**Result:**

```
---
Create Bloom filter with 60 bits and 3 hash functions

---
Add words to Bloom filter

*number of add warnings: 1

**results of lookup test: TP 20 | FP 12 | FN 0 | TN 38 

Sucessfully saved Bloom filter to test_save_bloom.txt!
---
Load Bloom Filter from test_save_bloom.txt

---------------------------------
Bloom filter

m = 64, k = 3
hash_seeds = 28 10 6 
array = a78fffff 867c8f3 0 0 0 0 1c701 0 0 0 ... [output truncated]
41/64 bits set
---------------------------------

**results of lookup test: TP 20 | FP 12 | FN 0 | TN 38 
```

See [./bloom/results/output_test_bloom_verbose.txt](./bloom/results/output_test_bloom_verbose.txt) for example verbose result.

See [./bloom/src/bloom.h](./bloom/src/bloom.h) for documentation.

## Analysis of test_bloom Results

Note that there are no False Negatives (FN). It is impossible for a bloom filter to produce a false negative.

You can control the number of false positives (reduce collision) by increasing number of bits and increasing number of hash functions.

The value of m configures how many bits will be allocated for the bit array. Ideally, when all the items are added, the bit array should be approximately half full. Worst case is if the bit array is completely full (all 1s), as it will say that every query may be in the filter (false positives). For performance reasons, the bit array rounds m to the nearest largest power of 2.

The value of k configures how my hash functions will be used in the Bloom filter hashing scheme. A higher number of hash functions means that there is a higher signal to noise ratio, where signal is the probability that if the term is a member of set given that all the term's probes are present in the Bloom filter, and noise is the probability of a false positive. Rarer terms have a higher likelihood of being a false positive, so increasing k improves the signal to noise ratio and allows for better retrieval of rate terms. However, a larger k leads more hash operations, and therefore slower insertion and lookup time. The seeds of the hash function are generated with a random number generator.
