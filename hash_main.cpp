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
#include <type_traits> //for hashing different types
#include <fstream>
#include <cstring> //for cstring operations
#include <random>
#include <ctime>
#include <chrono>  // For measuring time


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

        enum EntryType {ACTIVE, EMPTY, DELETED};
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
            EntryType info; // A flag indicating whether the entry is active or logically deleted.

            /**
             * @brief Default constructor for HashEntry.
             *
             * This constructor initializes the entry with an `EMPTY` status, meaning the slot in the hash table is unoccupied.
             * No key or value is assigned during this initialization. The entry is ready to be filled when the `insert()` method
             * places a new key-value pair and marks the entry as `ACTIVE`.
             */
            HashEntry() : info(EMPTY) {}
            
            /**
             * @brief Constructs a HashEntry.
             * @param k The key of the entry.
             * @param v The value associated with the key.
             * @param entry_type The status of the entry (ACTIVE, EMPTY, or DELETED).
             */
            HashEntry(const KeyType & k, const ValueType & v, EntryType entry_type = ACTIVE)
                : key(k), info(entry_type) {
                value_count.push_back(ValueCount(v, 1));  // Add the value with count 1
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
        // Helper function to find the next prime number greater than or equal to n
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
        static constexpr double LOAD_FACTOR = 0.4; //Constant LOAD FACTOR
        int tableSize;
        int currentSize;
        std::vector<HashEntry> table; // The underlying array represents the hash table
        

        /**
         * @brief Hash function for integers using a large prime constant derived from the golden ratio.
         *
         * This hash function takes an integer key and returns a hash value. It uses Knuth's multiplicative hashing technique,
         * where the key is multiplied by a large prime constant (related to the golden ratio) and then modded by the table size.
         * This method helps to distribute keys uniformly across the hash table, reducing the chances of collisions.
         *
         * @param key The integer key to be hashed.
         * @return A hash value representing the index in the hash table for the given key.
         */
        template <typename T = KeyType>
        typename std::enable_if<std::is_integral<T>::value, size_t>::type
        hash(const T& key) const {
            size_t largePrime = 2654435761;  // Prime constant (derived from golden ratio)
            return (key * largePrime) % tableSize;
        }
        /**
         * @brief Hash function for strings using a polynomial rolling hash algorithm.
         *
         * This hash function computes a hash value for a string by treating it as a polynomial, where each character
         * is assigned a weight based on its position in the string. The characters are processed from left to right,
         * and each character is multiplied by a prime number base (37), ensuring that different permutations of the string
         * result in different hash values.
         *
         * The function uses the modulo operation to keep the hash value within the bounds of the hash table, preventing overflow
         * for large strings.
         *
         * @param key The string key to be hashed.
         * @return A hash value representing the index in the hash table for the given string.
        */
        template <typename T = KeyType>
        typename std::enable_if<std::is_same<T, std::string>::value, size_t>::type
        hash(const T& key) const {
            size_t hashVal = 0;
            size_t prime = 37; // Prime base
            
            for (char ch : key) {
                hashVal = (prime * hashVal + ch) % tableSize;  // Modulo to prevent overflow
            }
            
            return hashVal;  // Modulo tableSize to get final hash within range
        }
        /**
         * @brief Rehashes the hash table when the load factor becomes too high.
         *
         * This function increases the size of the hash table to the next prime number
         * of double the old table size
         * Perform reinserting all existing entries into the new table. 
         */
        void rehash() {
            std::vector<HashEntry> oldTable = table;  // Store the current table

            // Increase table size to the next prime number roughly double the current size
            tableSize = nextPrime(2 * tableSize);
            table.resize(tableSize);  // Resize the table to the new size (table is std::vector)

            // Reinitialize all entries in the new table
            for (auto &entry : table) {
                entry.info = EMPTY;
            }

            // Reinsert all active entries from the old table into the new table
            for (const auto &entry : oldTable) {
                if (entry.info == ACTIVE) {
                    for (const auto &vc : entry.value_count) {
                        privateInsert(entry.key, vc.value);  // Rehash and reinsert the old key-value pair
                    }
                }
            }
        }

        /**
         * @brief Insert a key-value pair into the hash table.
         * Uses linear probing to resolve collisions.
         * @param k The key to be inserted.
         * @param v The value associated with the key.
         */
        void privateInsert(const KeyType& k, const ValueType& v) {
            size_t index = hash(k); // Hash the key to an index
            size_t firstDeleted = -1; // Track the first `DELETED` slot found during probing

            //Perform linear probing to resolve collisions if the index already taken. table[index] is a HashEntry
            while(table[index].info != EMPTY){
                //If the index is already holding the key that is adding 
                if((table[index].info == ACTIVE) && (table[index].key == k)){
                    //Check if the value adding is already in the key -> increament
                    for (auto & vc : table[index].value_count){
                        if(vc.value == v){
                            vc.count++;
                            return;
                        }
                    }
                    //If not just add a new value
                    table[index].value_count.push_back(ValueCount(v, 1));
                    return;
                }
                /**Record the first DELETED slot encountered. Continue searching to make sure we look at the entire table.
                * If the key is not in the table we can insert at first deleted slot later after the loop is finish
                */
                if (table[index].info == DELETED && firstDeleted == -1) {
                    firstDeleted = index;
                }
                //If the index is not holding the same key that is adding -> Probing
                index = (index + 1) % tableSize;
            }
                // If a DELETED slot was found during probing, reuse it for the new entry
                if (firstDeleted != -1) {
                    index = firstDeleted;
                }

            
            //After Checking. ie: found the index -> Add the HashEntry element to the index (new key only)
            table[index] = HashEntry(k, v); //Default constructor will turn it to active
            currentSize++; //increase the current size
        }

        /**
         * @brief Private method to find the index of a key in the hash table.
         *
         * This function performs linear probing to find the index of the given key.
         * If the key is found and is ACTIVE, it returns the index of the entry; 
         * if not, or if the key is marked as DELETED, it returns -1.
         *
         * @param k The key to search for.
         * @return A pointer to the HashEntry if found and ACTIVE, nullptr otherwise.
         */
        HashEntry* privateFind(const KeyType & k) const {
            size_t index = hash(k);  // Hash the key to an index

            // Perform linear probing to find the key
            while (table[index].info != EMPTY) {  // Continue until an EMPTY slot is found
                if (table[index].info == ACTIVE && table[index].key == k) {
                    return &table[index];  // Return a pointer to the HashEntry if it's ACTIVE and matches the key
                }
                // If the entry is DELETED or doesn't match, continue probing
                index = (index + 1) % tableSize;
            }

            return nullptr;  // Key not found
        }
        /**
         * @brief Removes a key and its associated values from the hash table.
         *
         * This function searches for the key using `find()`. If the key is found, it marks the entry as `DELETED`
         * and decrements `currentSize`. If the key is not found, an error condition is raised.
         *
         * @param k The key to be removed.
         * @return True if the key was successfully removed, false if the key was not found.
         */
        bool privateRemove(const KeyType & k) {
            HashEntry * entry = privateFind(k);
            if (entry != nullptr) {
                // Mark the entry as DELETED and adjust currentSize
                entry->info = DELETED;
                currentSize--;  // Decrement currentSize since the element is removed
                return true;
            } 
            else {
                return false;
            }
        }
       

        /**
         * @brief Private method to display the contents of the hash table.
         *
         * This method iterates over the hash table and prints out the keys and associated values along with their counts.
         * Only `ACTIVE` entries are displayed, while `EMPTY` and `DELETED` slots are skipped.
         * This method is intended for internal use and should be called by the public `display()` method.
         */
        void privateDisplay() const {
            for(size_t i = 0; i < tableSize; i++){
                if(table[i].info == ACTIVE){
                    std::cout << "Index: " << i << " | Key: " << table[i].key << "\n"; 
                    for (const auto& vc : table[i].value_count) {
                        std::cout << "[Value: \'" << vc.value << "\', Count: \'" << vc.count << "\'] ";
                    }
                std::cout << std::endl;    
                }
            }
        }

    public:
        /**
         * @brief Constructs an empty Hash Table
         */
        explicit HashTable(int size = 101) : tableSize(nextPrime(size)), table(tableSize) {}

        ~HashTable() {
            // Clear the table to ensure all entries are properly deallocated
            table.clear(); 
        }


        /**
         * @brief Public insert method that calls the private insert function.
         * @param k The key to be inserted.
         * @param v The value associated with the key.
         */
        void insert(const KeyType & k, const ValueType & v) {
            // Rehash if the load factor exceeds 0.7
            if (this->currentSize >= this->LOAD_FACTOR * tableSize) {
                rehash();
            }
            privateInsert(k, v);
        }
        /**
         * @brief Returns the number of active elements in the hash table.
         *
         * @return The number of active elements in the hash table.
         */
        int size() const {
            return this->currentSize;
        }

        /**
         * @brief Checks if the hash table is empty.
         *
         * This function returns true if the hash table contains no elements, i.e., if the `currentSize`
         * is zero. It provides a convenient way to check if the hash table is currently empty.
         *
         * @return True if the hash table is empty, false otherwise.
         */
        bool empty() const {
            return currentSize == 0;
        }
        /**
         * @brief Public method to find a key in the hash table and print its key and associated values with their counts.
         *
         * This method calls the `privateFind()` function. If the key is found and ACTIVE, it prints the key and its associated
         * values with their counts.
         *
         * @param k The key to search for.
         */
        void find(const KeyType &k) const {
            const HashEntry* entry = privateFind(k);
            
            if (entry != nullptr) {
                std::cout << "Key: " << entry->key << "\nValues: ";
                for (const auto& vc : entry->value_count) {
                    std::cout << "[Value: '" << vc.value << "', Count: " << vc.count << "] ";
                }
                std::cout << std::endl;
            } else {
                std::cout << "Key '" << k << "' not found." << std::endl;
            }
        }
        /**
         * @brief Public method to remove a key from the hash table.
         *
         * This method calls the `privateRemove()` function. If the key is not found, it throws a `std::runtime_error`.
         * Otherwise, the key is removed from the hash table.
         *
         * @param k The key to be removed.
         * @throws std::runtime_error if the key is not found.
         */
        void remove(const KeyType &k) {
            if (!privateRemove(k)) {
                throw std::runtime_error("Error: Key not found in the hash table.");
            }
        }

        /**
         * @brief Public display method that calls the private display function.
         */
        void display() const {
            privateDisplay();
        }

};





int main(int argc, char* argv[]){

    // Create a hash table for int keys and string values
    HashTable<int, std::string> hashTable1(1);

    // Inserting key-value pairs into the hash table
    hashTable1.insert(1, "apple");
    hashTable1.insert(2, "banana");
    hashTable1.insert(1, "apple");  // Increment count for "apple"
    hashTable1.insert(3, "orange");
    hashTable1.insert(2, "grape");
    hashTable1.insert(4, "banana");

    // Display the contents of the hash table
    hashTable1.display();

        // Create a hash table for string keys and string values
    HashTable<std::string, std::string> hashTable2(1);

    // Inserting key-value pairs into the hash table
    hashTable2.insert("apple", "fruit");
    hashTable2.insert("banana", "fruit");
    hashTable2.insert("carrot", "vegetable");
    hashTable2.insert("apple", "fruit");  // Increment count for "apple" and "fruit"
    hashTable2.insert("banana", "yellow");
    hashTable2.insert("carrot", "orange");

    // Display the contents of the hash table
    hashTable2.display();

    //SPEED TESTING//
    /* // Create a hash table for int keys and string values with an initial size of 101
    HashTable<int, std::string> hashTable(atoi(argv[1]));

    const int numElements = atoi(argv[2]);  // Number of elements to insert (enough to trigger rehashing)
    
    // Start timing before insertions
    auto start = std::chrono::high_resolution_clock::now();

    // Insert elements into the hash table
    for (int i = 0; i < numElements; i++) {
        hashTable.insert(i, "test_value_" + std::to_string(i));  // Insert unique key-value pairs
    }

    // Stop timing after all insertions
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate the time taken
    std::chrono::duration<double> duration = end - start;

    // Display the time taken
    std::cout << "Time taken to insert " << numElements << " elements: " << duration.count() << " seconds" << std::endl; */

    // Optionally display the final hash table contents (useful for debugging, but slow for large data)
    //hashTable.display();

    
    return 0;

}