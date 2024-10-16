
#include <iostream>
#include <algorithm> 
#include <vector>
#include <fstream>
#include <cstring> //for cstring operations
#include <random>
#include <ctime>
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

    struct CountArray {
            char value;  // The value associated with the key
            int count;  // The count of how many times the value is associated with the key
            CountArray(char v, int c) : value(v), count(c){}
        };
    std::vector<CountArray> arg;
    
    char next_char;
    char peek_char;
    while(file.get(next_char)){
        bool found = false;
        for( auto & entry : arg){
            if(entry.value == next_char){
                entry.count++;
                found = true;
                break;
            }
        }
        if(found == false){
            arg.push_back(CountArray(next_char, 1));
        }
    }
    file.close();
    // Print the arg vector contents (character and count)
    std::cout << "Character counts from merchant.txt:\n";
    for( auto& entry : arg) {
        std::cout << entry.value << ": " << entry.count << std::endl;
    }
    
    
    return 0;
}





/*
    //TEST PART//
    std::cout << "======================================================="<< std::endl;
    // Test the AVL tree for integers
    AVLTree<int,int> intTree;
    std::cout << "Empty? "<< intTree.empty() << std::endl;

    // Insert integer keys with associated values
    intTree.insert(20, 1);
    intTree.insert(25, 2);
    intTree.insert(15, 3);
    intTree.insert(10, 4);
    intTree.insert(50, 5);
    intTree.insert(80, 6);
    intTree.insert(30, 7);
    intTree.insert(35, 8);
    

    // Insert some duplicate values to check count increments
    intTree.insert(20, 1); // Increment count for value 1 under key 20
    intTree.insert(50, 5); // Increment count for value 5 under key 50
    std::cout << "Size: " << intTree.size() << std::endl;   
    std::cout << "In-order traversal of integer AVL tree with values and counts:\n";
    intTree.display();
    std::cout << "SIZE: " << intTree.size() << std::endl;
    std::cout << "Empty? "<< intTree.empty() << std::endl;

    int valtofind = 20;
    std::cout << "Find key: " << valtofind << std::endl;
    intTree.find(20);

    int valtorm = 20;
    std::cout << "Remove key: " << valtorm << std::endl;
    intTree.remove(valtorm);
    intTree.display();

    // Test the AVL tree for strings
    AVLTree<std::string,std::string> stringTree;
    std::cout << "Empty? "<< stringTree.empty() << std::endl;

    // Insert string keys with associated string values
    stringTree.insert("banana", "ba");
    stringTree.insert("apple", "ap");
    stringTree.insert("orange", "or");

    // Insert some duplicate values to check count increments
    stringTree.insert("banana", "ba"); // Increment count for "ba" under "banana"
    stringTree.insert("orange", "or"); // Increment count for "or" under "orange"
    std::cout << "Size: " << stringTree.size() << std::endl;

    std::cout << "In-order traversal of string AVL tree with values and counts:\n";
    stringTree.display();
    std::cout << "SIZE: " << stringTree.size() << std::endl;
    std::cout << "Empty? "<< stringTree.empty() << std::endl;

    std::string strvaltofind = "banana";
    std::cout << "Find key: " << strvaltofind << std::endl;
    stringTree.find("banana");
    
    std::string strvaltorm = "banana";
    std::cout << "Remove key: " << strvaltorm << std::endl;
    stringTree.remove(strvaltorm);
    stringTree.display();
    
    return 0;
}
*/

