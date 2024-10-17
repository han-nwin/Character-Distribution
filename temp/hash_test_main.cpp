

int main(int argc, char* argv[]){
    // 1. Testing int keys and int values
    std::cout << "=== Testing int keys and int values ===\n";
    HashTable<int, int> hashTable1(1);

    std::cout << "Is table1 empty? " << (hashTable1.empty() ? "Yes" : "No") << std::endl;

    // Insert key-value pairs
    hashTable1.insert(1, 100);
    hashTable1.insert(2, 200);
    hashTable1.insert(3, 300);

    // Test size
    std::cout << "Size after 3 insertions: " << hashTable1.size() << std::endl;

    // Test find()
    std::cout << "\nFinding key 2:\n";
    hashTable1.find(2); // Should find key 2

    // Test remove()
    std::cout << "\nRemoving key 2:\n";
    hashTable1.remove(2); // Should remove key 2
    std::cout << "Size after removing key 2: " << hashTable1.size() << std::endl;

    std::cout << "\nFinding key 2 after removal:\n";
    hashTable1.find(2); // Should not find key 2 anymore

    // Display
    std::cout << "\nDisplaying table1:\n";
    hashTable1.display();

    std::cout << "\nIs table1 empty? " << (hashTable1.empty() ? "Yes" : "No") << std::endl;


    // 2. Testing int keys and string values
    std::cout << "\n=== Testing int keys and string values ===\n";
    HashTable<int, std::string> hashTable2(1);

    std::cout << "Is table2 empty? " << (hashTable2.empty() ? "Yes" : "No") << std::endl;

    // Insert key-value pairs
    hashTable2.insert(1, "apple");
    hashTable2.insert(2, "banana");
    hashTable2.insert(3, "cherry");

    // Test size
    std::cout << "Size after 3 insertions: " << hashTable2.size() << std::endl;

    // Test find()
    std::cout << "\nFinding key 2:\n";
    hashTable2.find(2); // Should find key 2

    // Test remove()
    std::cout << "\nRemoving key 2:\n";
    hashTable2.remove(2); // Should remove key 2
    std::cout << "Size after removing key 2: " << hashTable2.size() << std::endl;

    std::cout << "\nFinding key 2 after removal:\n";
    hashTable2.find(2); // Should not find key 2 anymore

    // Display
    std::cout << "\nDisplaying table2:\n";
    hashTable2.display();

    std::cout << "\nIs table2 empty? " << (hashTable2.empty() ? "Yes" : "No") << std::endl;


        std::cout << "\n=== Testing string keys and string values ===\n";
    HashTable<std::string, std::string> hashTable3(1);  // Start with a small size to trigger rehash quickly

    std::cout << "Is table3 empty? " << (hashTable3.empty() ? "Yes" : "No") << std::endl;

    // Insert key-value pairs
    hashTable3.insert("apple", "fruit");
    hashTable3.insert("banana", "fruit");
    hashTable3.insert("carrot", "vegetable");

    // Test size
    std::cout << "Size after 3 insertions: " << hashTable3.size() << std::endl;

    // Insert more to trigger multiple rehashes
    hashTable3.insert("dragonfruit", "fruit");
    hashTable3.insert("elderberry", "fruit");
    hashTable3.insert("fig", "fruit");
    hashTable3.insert("grape", "fruit");
    hashTable3.insert("honeydew", "fruit");
    hashTable3.insert("kiwi", "fruit");
    hashTable3.insert("lemon", "fruit");
    hashTable3.insert("mango", "fruit");
    hashTable3.insert("nectarine", "fruit");
    hashTable3.insert("orange", "fruit");
    hashTable3.insert("papaya", "fruit");
    hashTable3.insert("quince", "fruit");
    hashTable3.insert("raspberry", "fruit");
    hashTable3.insert("strawberry", "fruit");
    hashTable3.insert("tangerine", "fruit");
    hashTable3.insert("ugli fruit", "fruit");
    hashTable3.insert("vanilla", "fruit");
    hashTable3.insert("watermelon", "fruit");

    // Test size after more insertions
    std::cout << "Size after multiple insertions: " << hashTable3.size() << std::endl;

    // Test find()
    std::cout << "\nFinding key 'banana':\n";
    hashTable3.find("banana"); // Should find key 'banana'

    // Test remove()
    std::cout << "\nRemoving key 'banana':\n";
    hashTable3.remove("banana"); // Should remove key 'banana'
    std::cout << "Size after removing key 'banana': " << hashTable3.size() << std::endl;

    std::cout << "\nFinding key 'banana' after removal:\n";
    hashTable3.find("banana"); // Should not find key 'banana' anymore

    // Display
    std::cout << "\nDisplaying table3:\n";
    hashTable3.display();

    std::cout << "\nIs table3 empty? " << (hashTable3.empty() ? "Yes" : "No") << std::endl;

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
    // Create a hash table for int keys and string values with an initial size of 101
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
