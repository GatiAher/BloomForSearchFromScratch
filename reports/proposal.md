# Bloom For Search (From Scratch)

> Gati Aher  
> Software Systems SP2020  
> Final Project

## Background

### Bloom Filters

The Bloom Filter is a space-efficient data structure that can tell you with certainty whether an object is in a collection or not. False positive matches are possible, but false negatives are not: so given a query it can either return "possibly in set" or "definitely not in set". 

Unlike most implementations of set membership, the Bloom Filter does not actually store the data items at all. Instead it uses a bit array that allows hash collisions. If all the hashed bits of the query signature appear set in the bit array, then the query is possibly present in the data set. There is a small chance that the bit array set those bits due to the signature of other documents. This false positive rate can be controlled and optimized by controlling the length of the bit vector.

The probabilistic feature allows Bloom Filters to do membership validation in very little time and very little space. So these Bloom Filter lookup tables can even be stored in a browser cache without taking up a significant amount of space.

In the real world, Bloom Filters are used to quickly tell you if a username is taken, or to quickly check if a website is known to be malicious. The start-up I was interning for last summer implemented a Bloom Filter to speed up their database querying times.

Resources on Bloom Filters:

* [Wikipedia: Bloom Filters](https://en.wikipedia.org/wiki/Bloom_filter)
* [GeeksforGeeks: Bloom Filters – Introduction and Implementation in Python and C++](https://www.geeksforgeeks.org/bloom-filters-introduction-and-python-implementation/)
* [Richard Startin's Blog: Building a Bloom Filter from Scratch in Java](https://richardstartin.github.io/posts/building-a-bloom-filter-from-scratch)

### BitFunnel

More interestingly, Bing uses Bloom Filters in their search engine indexing algorithm, BitFunnel. This algorithm extends Bloom Filters with the concept of bit-sliced signatures to reduce overall query cost. Bing open-sourced this implementation via GitHub in 2016. The code in this repository is in an incomplete state, but it was made available to let people play with an incomplete and buggy version of the final algorithm that went into production. 

The BitFunnel Git repository contains the three major components:
* BitFunnel: the text search / retrieval system
* WorkBench: a tool for preparing test for use in BitFunnel
* NativeJIT: software that takes expressions that use C data structures and transforms them into highly optimized assembly code

Resources on Bit-Slicing and BitFunnel:

* [Wikipedia: BitFunnel](https://en.wikipedia.org/wiki/BitFunnel)
* [BitFunnel Performance Estimation talk + transcript by Dan Luu, works on BitFunnel](http://bitfunnel.org/strangeloop/)
* [BitFunnel: Revisiting Signatures for Search talk by Micheal Hopcroft, creator of BitFunnel](https://www.youtube.com/watch?v=1-Xoy5w5ydM)
* [BitFunnel Git Repo is Dormant](https://github.com/BitFunnel/BitFunnel/issues/433)
* [Richard Startin's Blog: Bit-Sliced Signatures and Bloom Filters](https://richardstartin.github.io/posts/bit-sliced-signatures-and-bloom-filters)
* [Lee Holmes Blog: Using Bloom Filters to Efficient Filter Out “Known Good”](https://www.leeholmes.com/blog/2021/03/24/using-bloom-filters-to-efficient-filter-out-known-good/)
* [GeeksforGeeks: Bloom Filters – Introduction and Implementation in Python and C++](https://www.geeksforgeeks.org/bloom-filters-introduction-and-python-implementation/)

## Learning Goals

I want to learn about Bloom Filters because I have seen them used in the real world and I am curious about them.

I have chosen this as a SoftSys project because in the talk ["BitFunnel: Revisiting Signatures for Search" talk by Micheal Hopcroft, creator of BitFunnel](https://www.youtube.com/watch?v=1-Xoy5w5ydM) he mentioned bit operations and reading specific memory locations using pointers, and I imagine implementing that in C would be fun. 

## Project Goal

**Minimum Viable Product:**

* A program that uses a *Bloom Filter* to quickly check if a query term is in a set.

**Upper Bound 1:**

* A program that uses a document signature for information retrieval given a query
* Uses a *Bloom Filter + Bit-Sliced Signatures + Parallelism* to reduce the number of words which must be processed to evaluate a query.

**Upper Bound 2:**

* A program that more closely resembles BitFunnel for information retrieval.
* Uses *Bloom Filter + Bit-Sliced Signatures + Parallelism + Blocked Signatures* to further reduce the amount of processing per query while maintaining an acceptable false positive rate.

## First Steps

**MVP:**

1. Implement a Bloom Filter in C
- if I get stuck, look at the Java, Python, or C++ implementations
2. Write test cases

**Upper Bound 1:**

3. Watch the videos by Micheal Hopcroft and Dan Luu to understand  Bit-Sliced Signatures and Blocked Signatures
4. Implement Bit-Sliced Signatures in C
5. Write test cases
6. Run time tests

**Upper Bound 2:**

7. Implement Blocked Signatures in C
8. Write test cases
9. Run time comparisons