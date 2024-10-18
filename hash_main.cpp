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
            // If n is less than 1, return the first prime number (2)
            if (n <= 1) return 2;
            // If n is even, increment it by 1 to make it odd
            if (n % 2 == 0) n++;
            // Continue checking for primes starting from n
            while (!isPrime(n)) {
                n += 2; // Increment by 2 to skip even numbers
            }
            return n;
        }
        static constexpr double LOAD_FACTOR = 0.7; //Constant LOAD FACTOR
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
        hash(const T & key) const {
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
        hash(const T & key) const {
            size_t hashVal = 5381;  // A more suitable starting value for djb2 hash
            size_t prime = 33;  // A smaller prime number often used for hashing strings
            
            for (char ch : key) {
                // Use bitwise left shift to amplify the effect of each character and add the character value
                hashVal = ((hashVal*prime) + ch); 
            }
            
            hashVal = hashVal % tableSize;  // Ensure it fits within the table size
            //std::cout << "Hashing key: '" << key << "', Hash val: " << hashVal << std::endl;
            
            return hashVal;
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

            // Reset currentSize to 0 because we will reinsert everything
            currentSize = 0;

            // Reinsert all active entries from the old table into the new table
            for (const auto &entry : oldTable) {
                if (entry.info == ACTIVE) {
                    for (const auto &vc : entry.value_count) {
                        //std::cout << "Rehashing key: '" << entry.key << "'" << std::endl;
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
        void privateInsert(const KeyType & k, const ValueType & v) {
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
        HashEntry* privateFind(const KeyType & k) {
            size_t index = hash(k);  // Hash the key to an index

            // Perform linear probing to find the key
            while (table[index].info != EMPTY) {  // Continue until an EMPTY slot is found
                if ((table[index].info == ACTIVE) && (table[index].key == k)) {
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
         * @brief Function to get a value of a given key randomly. 
         * Value that has a higher count will has a higher chance of being returned.
         * 
         * @param Keytype & k key value
         * @return ValueType value
         */
        std::mt19937 rand_num_gen;  // Each instance gets its own rng (random number generator). Seeding happens in the constructor
        ValueType privateGetRandVar(const KeyType k) {
            HashEntry* entry = privateFind(k);
            if (entry == nullptr) {
                std::cerr << "Key not found in privateGetRandVar: \'" << k << "\'" << std::endl;
                throw std::runtime_error("Key not found");
            }
            int totalWeight = 0;
            //Calculate total weight
            for (const auto& vc : entry->value_count) {
                    totalWeight += vc.count;
            }
            // Generate a random number between 0 and totalWeight - 1
            std::uniform_int_distribution<int> dist(0, totalWeight - 1);
            int randNum = dist(rand_num_gen);
            // Traverse the array and select the value based on the random number
            int cumulativeWeight = 0;
            for (const auto& vc : entry->value_count) {
                cumulativeWeight += vc.count;
                if (randNum < cumulativeWeight) {
                    return vc.value; // Pick this value
                }
            }
            return ValueType(); //Fallback, though we should never reach here
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
                    std::cout << "Index: " << i << " | Key: \'" << table[i].key << "\'\n"; 
                    for (const auto& vc : table[i].value_count) {
                        std::cout << "[Value: \'" << vc.value << "\', Count: \'" << vc.count << "\'] ";
                    }
                std::cout << std::endl;    
                std::cout << std::endl;    
                }
            }
        }

    public:
        /**
         * @brief Constructs an empty Hash Table
         */
        explicit HashTable(int size = 101) : tableSize(nextPrime(size)), currentSize(0), table(tableSize) {
            //Change the seed everytime the new HashTable Object is created
            std::random_device ran_device;
            rand_num_gen.seed(ran_device()); //seed the rng
        }

        /* ~HashTable() {
            // Clear the table to ensure all entries are properly deallocated
            table.clear(); 
        } */


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
        void find(const KeyType & k) const {
            //Const cast the return value of 'privateFind' to remain encapsulation
            const HashEntry* entry = const_cast<const HashEntry*>(const_cast<HashTable*>(this)->privateFind(k)); 
            
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
        void remove(const KeyType k) {
            if (!privateRemove(k)) {
                throw std::runtime_error("Error: Key not found in the hash table.");
            }
        }
        /**
         * @brief Public method to get a random value weighted by the count
         * Function is marked as const to ensure no modification is performed on the Tree Structure
         * @param Keytype & k
         * @return ValueType value
         */
        ValueType getRandVar(const KeyType & k){
            return privateGetRandVar(k);
        }

        /**
         * @brief Public display method that calls the private display function.
         */
        void display() const {
            privateDisplay();
        }

};

int main(int argc, char* argv[]){
    if(argc != 2){
        std::cerr << "Usage: " << argv[0] << " <window-size>" << std::endl;
        return 1;
    }
    else if(atoi(argv[1]) <= 0){
        std::cerr << "Invalid Argument: " << argv[0]<< " " << argv[1] << ": <window-size> must be a positive integer" << std::endl;
        return 1;
    }
    
    std::ifstream file("merchant.txt"); // Open the file
    if (!file) {
        std::cerr << "Error opening input file!" << std::endl;
        return 1; // Exit if the file couldn't be opened
    }
    //==Capture file length to use later==//
    file.seekg(0, std::ios::end); // Seek to the end of the file
    std::streampos fileSize = file.tellg(); // Get the current position in the file, which is the size
    file.seekg(0, std::ios::beg); // Reset the stream position back to the beginning
    std::size_t infileLength = static_cast<std::size_t>(fileSize); // Cast fileSize to an integer (int or size_t)
    //===========================================================//

    HashTable<std::string,std::string> stringTable(1);//Declare the Hash table structure and initialize the length = file length
    
    char next_char;
    char peek_char;
    int window_size = atoi(argv[1]);
    char* buffer = new char[window_size + 1]; // Create a buffer to hold the window
    // Pre-fill the buffer with the first 'window_size' characters
    for (int i = 0; i < window_size; i++) {
        if (file.get(next_char)) {
            // Replace newline or tab with a space
            /* if (next_char == '\n' || next_char == '\t') {
                next_char = ' ';
            } */
            buffer[i] = next_char;
        } else {
            break; // If the file has fewer characters than the window size
        }
    }
    buffer[window_size] = '\0'; // Null-terminate the buffer to treat it as a C-string
    //NOTE: Save the first string to use for output later
    std::string firstString(buffer);
    // Peek the next character in the file stream without extracting it
    peek_char = file.peek();
    //Insert to the Hash Table
    stringTable.insert(std::string(buffer),std::string(1,peek_char));

    // Slide the window through the file, one character at a time
    while (file.get(next_char)) {
        // Replace newline or tab with a space
        /* if (next_char == '\n' || next_char == '\t') {
            next_char = ' ';
        } */
        // Shift the buffer to the left by 1 and append the new character
        for (int i = 0; i < window_size - 1; i++) {
            buffer[i] = buffer[i + 1];
        }
        buffer[window_size - 1] = next_char; //get the next char

        
        peek_char = file.peek();
        // Output the current window
        /* std::cout << "Insert Key: \'" << buffer <<"\'" << std::endl;
        std::cout << "Value: \'" << peek_char <<"\'" << std::endl; */
        //Insert to the Hash Table
        stringTable.insert(std::string(buffer),std::string(1,peek_char));
    }
    
    file.close();
    delete[] buffer; // Clean up dynamically allocated memory
    /* stringTable.display();
    std::cout << "GET RAND VAR" << std::endl;
    std::string key = "\n";
    std::cout << "Key: \'" << key << "\' | Value: \'" <<stringTable.getRandVar(std::string(key)) << "\'" << std::endl;  */
    //===================DONE STORING INPUT=====================//
    // Work on the output
    
    std::string outString = firstString;
    std::string windowString = firstString;
    std::string key = windowString;

    try {
        std::string toAdd = stringTable.getRandVar(key);

        while (outString.length() <= infileLength) {
            // Append to outString
            outString += toAdd;

            // Update the window by removing the first character and adding the next one
            // Remove the first character of windowString
            windowString.erase(0, 1);
            // Append the new string (toAdd) to the end of windowString
            windowString.append(toAdd);

            // Update the key for the next iteration
            key = windowString;

            // Get the next random value for the current window
            toAdd = stringTable.getRandVar(key);
        }
    } catch (const std::runtime_error &e) {
        std::cout << "Caught runtime_error: " << e.what() << std::endl;
    }

    //outString.pop_back(); outString.pop_back();  // Remove any garbage characters

    //std::cout << "====Final String====" << std::endl;
    //std::cout << "'" << outString << "'\n";

    // Create and open the output file
    std::ofstream outfile("out.txt");  
    if (!outfile) {
        std::cerr << "Error creating output file!" << std::endl;
        return 1;
    }
    // Write outString to the file
    outfile << outString;
    outfile.close();
    std::cout << "====Export to out.txt file successfully!====" << std::endl;

    return 0;
}