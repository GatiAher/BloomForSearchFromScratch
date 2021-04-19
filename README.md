# BloomForSearchFromScratch

## Choice of Hash Function:

The hash function used in a bloom filter must be independent, uniformly distributed, and as fast as possible.

MurmurHash is a non-cryptographic hash function suitable for general hash-based lookup. In this project, I use [jwerle/murmurhash](https://fuchsia.googlesource.com/third_party/murmurhash.c/) which implements version 3 of MurmurHash.

You can try out murmurhash with
```
make try_murmurhash && ./try_murmurhash
```

## Set-Up

```
make
```
You should see an executable named `run_bloom`.

Call `run_bloom` with the following arguments:
```
bloom [-v] [-a file_words_to_add_to_bloom] [-c file_words_to_check_in_bloom] [-b number_of_bits] [-n number_of_hash_functions] [-h]
```

**Example:**

```
./run_bloom -a data/in_test.txt -c data/out_test.txt -b 32 -n 3
```

**Result:**

```
 ------ 
 RESULTS: 
 BAD_ADDs 7 
 TP 20 
 FP 20 
 FN 0 
 TN 30
```

See `data/verbose_result.txt` for example verbose result.

## Analysis of Results

Note that there are no False Negatives (FN). It is impossible for a bloom filter to produce a false negative.

You can control the number of false positives (reduce collision) by increasing number of bits and increasing number of hash functions.
