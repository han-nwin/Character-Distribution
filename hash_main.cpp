/**
* CS/SE 3345 - Programming Assignment 3 - Monkey Character Distribution
* Lecturer: Andrew Nemec
* Name: Tan Han Nguyen
* NetID: TXN 200004
* Date: October 1, 2024
* Due Date: October 18, 2024
*/ 
#include <iostream>
#include <algorithm> 
#include <vector>
#include <fstream>
#include <cstring> //for cstring operations
#include <random>
#include <ctime>

/**
 * @class HashTable
 * @brief Implements a hash table with linear probing for collision resolution.
 *
 * The HashTable class provides an efficient data structure for storing key-value pairs.
 * The hash table uses a hash function to map keys to an array index, and resolves collisions
 * using linear probing. This ensures that each key is associated with a unique index in the table,
 * and if collisions occur (i.e., multiple keys hash to the same index), the table finds the next
 * available index in a linear sequence.
 *
 * The class supports basic operations such as insertion, deletion, lookup, and rehashing.
 * Insertion and lookup operations are performed in average O(1) time, while rehashing occurs
 * when the table's load factor exceeds a certain threshold. Rehashing dynamically resizes the table
 * and redistributes the elements to maintain efficiency.
 *
 * The class uses a prime number for the table size to help reduce the likelihood of collisions.
 *
 * @tparam KeyType The data type of the keys to be stored in the hash table. The type must support hashing and comparison operators.
 * @tparam ValueType The data type of the values associated with each key.
 */
template <typename KeyType, typename ValueType>
class HashTable{
    private:
        /**
         * @struct ValueCount
         * @brief Store all values associated with the key and the count of each value association
         *
         * The ValueCount is used to stored associated values of each AvlNode
         * Count how many time each value accociate with the AvlNode
         */
        struct ValueCount {
            ValueType value;  // The value associated with the key
            int count;  // The count of how many times the value is associated with the key

            /**
             * @brief Constructor
             * @param Comparable v Reference to the value
             * @param c count of the value
             */
            ValueCount(const ValueType & v, int c) : value(v), count(c) {}
        };
        /**
         * @struct HashEntry
         * @brief Represents a key with associated values and their counts in the hash table.
         *
         * The HashEntry stores a key along with a vector of associated values and their respective counts.
         * It also includes a flag indicating whether the entry is active or has been logically deleted.
         * This structure is particularly useful for open addressing collision resolution techniques
         * such as linear probing, where deleted entries need to be marked and re-used.
         */
        struct HashEntry {
            KeyType key; // The key of the entry.
            std::vector<ValueCount> value_count; // Vector of values and their counts.
            bool isActive; // A flag indicating whether the entry is active or logically deleted.
            
            /**
             * @brief Constructs a HashEntry.
             * @param k The key of the entry.
             * @param vc The vector of associated values and their counts.
             * @param active A boolean flag indicating if the entry is active (defaults to true).
             */
            HashEntry(const KeyType & key = KeyType(), const ValueType & value, bool active = true)
                : key(k), isActive(active) {
                    value_count.pushback(value,1);//Add the value to the array and initialize the count to 1
                }
        };
        

        // Helper function to check if a number is prime
        bool isPrime(int n) {
            if (n <= 1) return false;
            if (n <= 3) return true;
            if (n % 2 == 0 || n % 3 == 0) return false;
            //Any prime number larger than 3 is either 1 less than a multiple of 6 (i.e., 6k - 1) or 1 more than a multiple of 6 (i.e., 6k + 1).
            for (int i = 5; i * i <= n; i += 6) { //check up to sqrt(n)
                if (n % i == 0 || n % (i + 2) == 0)
                    return false;
            }
            return true;
        }
        // Function to find the next prime number greater than or equal to n
        int nextPrime(int n) {
            // If n is less than 2, return the first prime number (2)
            if (n <= 2) return 2;
            // If n is even, increment it by 1 to make it odd
            if (n % 2 == 0) n++;
            // Continue checking for primes starting from n
            while (!isPrime(n)) {
                n += 2; // Increment by 2 to skip even numbers
            }
            return n;
        }

        int tableSize;
        std::vector<ValueCount> value_count; // Vector of values and their counts
        std::vector<KeyType, ValueCount> * table; // pointer to the hashtable

    public:
        /**
         * @brief Constructs an empty AVL Tree.
         */
        explicit HashTable(int size = 100){
            this->tableSize = nextPrime(size);
        }

        // Destructor to free allocated memory
        ~HashTable() {
            delete[] table;
        }

};