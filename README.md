# LFU Cache
## Description
An LFU (Least Frequently Used) cache is a type of cache eviction algorithm that removes the least frequently accessed items when the cache reaches its capacity. The key idea is to track how often each item in the cache is accessed. When the cache is full and a new item needs to be added, the item with the fewest accesses (lowest frequency) is evicted to make space.
## Key Characteristics
1) **Frequency tracking:** Each cached item is associated with a count of how many times it has been accessed.
2) **Eviction policy:** When the cache is full, the item with the lowest access frequency is removed.
3) **Use case:** Suitable for scenarios where frequently accessed items are more valuable, and items accessed infrequently should be removed.

# Ideal Cache
## Description
An Ideal Cache is a theoretical caching algorithm that evicts the item which will not be needed for the longest time in the future. It provides the optimal eviction strategy because it has perfect knowledge of future access patterns.

## Key Characteristics
1) **Future knowledge:** The algorithm knows exactly when each item will be accessed again.
2) **Optimal eviction:** When the cache is full, it removes the item that won't be used for the longest period.
Theoretical use: It sets an upper bound for the performance of real-world cache algorithms, such as LRU or LFU, since no actual algorithm can predict future accesses.

***

## Build and Run

1) To compile, type:
```
cmake -DCMAKE_BUILD_TYPE=Release -S . -B build
```
```
cmake --build build
```
2) To run the project:

*LFU Cache*
```
build/source/LFUCache
```
*Ideal Cache*
```
build/source/IdealCache
```
*Tests*
```
build/tests/cache_tests
```
