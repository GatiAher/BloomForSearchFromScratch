# BloomForSearchFromScratch

## Choice of Hash Function:

The hash function used in a bloom filter must be independent, uniformly distributed, and as fast as possible.

MurmurHash is a non-cryptographic hash function suitable for general hash-based lookup. In this project, I use [jwerle/murmurhash](https://fuchsia.googlesource.com/third_party/murmurhash.c/) which implements version 3 of MurmurHash.

## Set-Up

```
make
```
You should see an executable named `bloom`.

Call `bloom` with the following arguments:
```
bloom [-v] [-a file_words_to_add_to_bloom] [-c file_words_to_check_in_bloom] [-b number_of_bits] [-n number_of_hash_functions] [-h]
```

**Example:**

```
./bloom -a data/in_test.txt -c data/out_test.txt -b 32 -n 3
```

**Result:**

```
RESULTS: BAD_ADDs 5 | TP 17 | FP 21 | FN 0 | TN 34
```

## Analysis of Results

Note that there are no False Negatives (FN). It is impossible for a bloom filter to produce a false negative.

You can control the number of false positives (reduce collision) by increasing number of bits and increasing number of hash functions.


## Verbose Result:

```
 ------ 
 Create bloom filter with 32 bits and 3 hash functions 

 ------ 
 Adding words to bloom filter 

added Absolutely
added Abundance
added Abundant
added Accomplished
added Achieve
added Adventure
added Affluent
added Alive
WARNING: all bits have already been set for Alluring
added Always
added Amazing
added Astonishing
WARNING: all bits have already been set for Astounding
WARNING: all bits have already been set for Authoritative
WARNING: all bits have already been set for Authority
WARNING: all bits have already been set for Awe-inspiring
added Awesome
 ------ 
 Looking up words that are in bloom filter 

might be in: Absolutely
might be in: Abundance
might be in: Abundant
might be in: Accomplished
might be in: Achieve
might be in: Adventure
might be in: Affluent
might be in: Alive
might be in: Alluring
might be in: Always
might be in: Amazing
might be in: Astonishing
might be in: Astounding
might be in: Authoritative
might be in: Authority
might be in: Awe-inspiring
might be in: Awesome
 ------ 
 Looking up words that are not in bloom filter 

might be in: babble
might be in: back-logged
definitely not in back-wood
definitely not in back-woods
definitely not in backache
definitely not in backaches
definitely not in backbite
might be in: backbiting
definitely not in backward
definitely not in backwardness
definitely not in backwoods
definitely not in bad
might be in: badly
might be in: baffle
definitely not in baffled
definitely not in bafflement
definitely not in baffling
definitely not in bait
definitely not in balk
might be in: banal
definitely not in bane
definitely not in banish
might be in: banishment
might be in: bankrupt
definitely not in barbarian
definitely not in barbaric
definitely not in barbarically
definitely not in barbarity
definitely not in barbarous
might be in: barbarously
definitely not in barren
definitely not in baseless
might be in: bash
definitely not in bashed
definitely not in bashful
definitely not in bashing
might be in: battered
might be in: battering
definitely not in batty
definitely not in bearish
definitely not in beastly
definitely not in bedlam
might be in: bedlamite
might be in: befoul
might be in: beg
definitely not in beggar
might be in: beggarly
definitely not in begging
might be in: beguile
might be in: belabor
might be in: belated
might be in: beleaguer
definitely not in belie
definitely not in belittle
might be in: belittled
 ------ 
 RESULTS: BAD_ADDs 5 | TP 17 | FP 21 | FN 0 | TN 34 
 ```
