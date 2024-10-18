/**
* CS/SE 3345 - Programming Assignment 2 - Monkey Character Distribution
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
#include <string>
#include <stdexcept>
#include <limits>

/**
 * @class AVLTree
 * @brief Implements a self-balancing Binary Search Tree using the AVL Tree algorithm.
 *
 * The AVLTree class provides an efficient binary search tree where the tree automatically balances itself
 * after each insertion to ensure that it remains balanced. This balance ensures that the operations
 * like insertion, deletion, and lookup are performed in O(log n) time. The AVLTree maintains the height-balanced
 * property, which states that the height difference between the left and right subtree of any node is no more than 1.
 *
 * The class supports operations such as insertion and in-order traversal. Balancing is performed using 
 * rotations (single and double), and the height of each node is updated during insertion to maintain balance.
 *
 * @tparam Comparable The data type to store in the tree, which must support comparison operators.
 */
template <typename KeyType, typename ValueType>
class AVLTree {
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
         * @struct AvlNode
         * @brief Represents a node in the AVL Tree.
         *
         * The AvlNode struct is used to store individual elements of the AVL Tree. Each node contains an element (the key),
         * pointers to its left and right children, and its height within the tree.
         */
        struct AvlNode {
            KeyType key; //Any comparable types: int, float, double, char, std::string, or any custom comparable class or struct
            std::vector<ValueCount> value_count; // Vector of values and their counts
            AvlNode *left;
            AvlNode *right;
            int height;

        /**
         * @brief Constructs an AVL Node.
         * @param ele The element (key) to store in the node.
         * @param lt Pointer to the left child node (defaults to nullptr).
         * @param rt Pointer to the right child node (defaults to nullptr).
         * @param h Height of the node (defaults to 0).
         */
        AvlNode(const KeyType & key, const ValueType & value, AvlNode *lt = nullptr, AvlNode *rt = nullptr, int h = -1)
                : key(key), left(lt), right(rt), height(h) 
                { 
                    value_count.push_back(ValueCount(value, 1)); //Push the value a initalize its count
                }
        
        };
        
        /**
         * @brief Pointer to the root node of the AVLTree
         */
        AvlNode *root; 

        /**
         * @brief Function to properly delete a node from memory
         */
        void clear(AvlNode *t){
            if (t != nullptr){
                clear(t->left);
                clear(t->right);
                delete t;
                t = nullptr;
            }
        }
        /**
         * @brief Private function to return height of a node
         * @param AvlNode * t
         * @return int height, -1 if t is nullptr
         */
        int height(AvlNode *t) const {
            if (t == nullptr) {
                return -1;
            }
            else {
                return t->height;
            }
        }
        /**
         * @brief Private function to return the size of the tree (number of node) recursively
         * Function is marked as const to ensure no modification is performed in the Tree structure
         * @param AvlNode * t
         * @return int size
         */
        int size(AvlNode *t) const {
            if (t == nullptr) {
                return 0;
            }
            else {
                return (size(t->left) + size(t->right) +  1);
            }
        }

        /**
         * @brief Private function to check if the avl tree rooted at node t
         * Function is marked as const to ensure no modification is performed on the tree structure
         * @param AvlNode t
         * @return True if empty, False if not empty
         */
        bool empty(AvlNode * t) const{
            if (t == nullptr){
                return true;
            }
            else return false;
        }
        static const int ALLOWED_IMBALANCE = 1;
        /**
         * @brief Balances the subtree rooted at the given node.
         * 
         * This function checks the balance factor of the node and applies the appropriate rotations (single or double)
         * to ensure that the subtree rooted at the given node is height-balanced.
         *
         * @param t Reference to the pointer to the root of the subtree to be balanced.
         */
        void balance(AvlNode * & t){
            if (t == nullptr){
                return;
            }
            else if ((height(t->left) - height(t->right)) > ALLOWED_IMBALANCE){
                if(height(t->left->left) >= height(t->left->right)){
                    singleWithLeftChild(t);
                }
                else doubleWithLeftChild(t);
            }
            else if ((height(t->right) - height(t->left)) > ALLOWED_IMBALANCE){
                if(height(t->right->right) >= height(t->right->left)){
                    singleWithRightChild(t);
                }
                else doubleWithRightChild(t);
            }

            t->height = std::max(height(t->left), height(t->right)) + 1; //Update the height

        }

        /**
         * @brief Performs a single rotation for a root node with the left child that has a left-heavy subtree.
         * Update the height
         * Update root node
         * @param k Reference to the pointer to the root of the subtree to rotate.
         */
        void singleWithLeftChild(AvlNode* & k){
            AvlNode * temp = k->left; //create a temp left child of k 
            k->left = temp->right; //move right child of temp to k left
            temp->right = k; //make k as temp right child
            k->height  = std::max(height(k->left), height(k->right)) + 1; //update the height
            temp->height = std::max(height(temp->left), height(k->right)) + 1; //update the height
            k = temp; //update root node of the subtree
        }
        /**
         * @brief Performs a single rotation for a root node with the right child that has right-heavy subtree.
         * Update the height
         * Update root node
         * @param k Reference to the pointer to the root of the subtree to rotate.
         */
        void singleWithRightChild(AvlNode* & k){
            AvlNode * temp = k->right; //create a temp left child of k 
            k->right = temp->left; //move right child of temp to k left
            temp->left = k; //make k as temp right child
            k->height  = std::max(height(k->left), height(k->right)) + 1; //update the height
            temp->height = std::max(height(temp->left), height(k->right)) + 1; //update the height
            k = temp; //update root node of the subtree
        }

        /**
         * @brief Performs a double rotation for a root node with the left child that has right heavy subtree
         * Update the height
         * Update root node
         * @param k Reference to the pointer to the root of the subtree to rotate.
         */
        void doubleWithLeftChild(AvlNode* & k){
            singleWithRightChild(k->left); //First rotation for the left node of k
            singleWithLeftChild(k); //Second rotation for k itseft
        }

        /**
         * @brief Performs a double rotation for a root node with the right child that has left heavy subtree
         * Update the height
         * Update root node
         * @param k Reference to the pointer to the root of the subtree to rotate.
         */
        void doubleWithRightChild(AvlNode* & k){
            singleWithLeftChild(k->right); //First rotation for the right node of k
            singleWithRightChild(k); //Second rotation for k itseft
        }

        /**
        * @brief Recursively inserts a new key and value into the subtree rooted at the given node.
        *
        * This function inserts the key and its associated value into the correct position in the subtree while maintaining the
        * binary search tree property. If the key already exists, it updates the value_count vector by adding or updating the value.
        * After insertion, the function calls `balance` to ensure that the subtree remains balanced.
        *
        * @param KeyType k The key to insert.
        * @param ValueType v The associated value for the key.
        * @param AvlNode t Reference to the pointer to the root of the subtree in which to insert the key-value pair.
        */
        void insert(const KeyType & k, const ValueType & v, AvlNode* & t) {
            if (t == nullptr) {
                // If no node exists, create a new node with the key and value
                t = new AvlNode(k, v);
            }
            else if (k < t->key) {
                insert(k, v, t->left);  // Insert into the left subtree
            }
            else if (k > t->key) {
                insert(k, v, t->right);  // Insert into the right subtree
            }
            // If the key already exists, check for the value in value_count
            else {
                bool valueExists = false;
                for (auto & vc : t->value_count) {
                    if (vc.value == v) {
                        vc.count++;  // Increment the count if the value exists
                        valueExists = true;
                        break;
                    }
                }
                if (valueExists == false) {
                    // If the value does not exist, add it to the value_count vector with count 1
                    t->value_count.push_back(ValueCount(v, 1));
                }
            }
            balance(t);  // Balance the subtree after insertion (height updated here)
        }

        /**
         * @brief Find the minimum node in a subtree (used for node replacement during deletion)
        */
        AvlNode* findMin(AvlNode* t) const {
            if (t == nullptr) return nullptr;
            if (t->left == nullptr) return t;
            return findMin(t->left);
        }

        /**
        * @brief Recursively removes a node with the specified key from the subtree rooted at the given node.
        *
        * This function removes the node with the given key from the subtree while maintaining the binary search tree property.
        * If the key exists in the tree, the node is removed, and the subtree is rebalanced if necessary.
        * If the key does not exist, an error condition occurs.
        * After removal, the function calls `balance` to ensure that the subtree remains balanced.
        *
        * @param KeyType k A reference to the key to remove from the AVL tree.
        * @param AvlNode* & t A pointer reference to the root of the subtree from which to remove the key.
        * @throw std::runtime_error if the key does not exist in the tree.
        */
        void remove(const KeyType & k, AvlNode * & t){
            if (t == nullptr) {
                throw std::runtime_error("Error: Key not found");  // Error if key does not exist
            }

            if(k < t->key){
                remove(k, t->left); //key is in the left subtree
            }
            else if(k > t->key){
                remove(k, t->right); //key is in the right subtree
            }
            //Key found
            else {
                //Node has 2 children
                if(t->left != nullptr && t->right != nullptr){
                    AvlNode* minNode = findMin(t->right); //find the minimum node of the right subtree
                    
                    t->key = minNode->key; //transfer the key
                    t->value_count = minNode->value_count; //transfer the array
                    remove(minNode->key, t->right); //remove the minNode from the right subtree
                }
                //Node has one or no child
                else{
                    AvlNode* oldNode = t;
                    //Promote the child (if there is)
                    if(t->left != nullptr){
                        t = t->left; //promote left child
                    }
                    else{
                        t = t->right; //promote right child
                    }
                    delete oldNode;
                }
            }
            //END ALL Checks//
                balance(t);  //Balance the tree
        }


        /**
         * @brief Private method that traverses the subtree rooted at root to find an element of key k 
         * Return the element (AvlNode)
         * Function is marked as const -> ensure no modification
         * @param KeyType & k: Reference of a key
         * @return pointer to the AvlNode: element. nullptr if no key found
         */
        AvlNode* find(const KeyType & k, AvlNode* root) const {
            //Check for the key value of the current node
            if((root->key) == k){
                return root; //return if key is found
            }
            //Search left 
            else if (k < root->key && root->left != nullptr){
                return find(k, root->left);
            }
            //Search right
            else if(k > root->key && root->right != nullptr){
                return find(k, root->right);
            }
            else return nullptr;
        }

        /**
         * @brief Private method that traverses the subtree rooted at the given node.
         * Recursively performs an in-order traversal of the tree.
         * Displays keys, values, and their counts in sorted order (left - root - right).
         * @param AvlNode root: Pointer to the root of the subtree to display.
         */
        void display(AvlNode* root) const {
            if (root != nullptr) {
                display(root->left);  // Traverse the left subtree

                // Display the key
                std::cout << "Key: \'" << root->key << "\' -> ";

                // Display all values and their counts for this key
                for (const auto& vc : root->value_count) {
                    std::cout << "[Value: \'" << vc.value << "\', Count: \'" << vc.count << "\'] ";
                }
                std::cout << std::endl;

                display(root->right);  // Traverse the right subtree
            }
        }

        /**
         * @brief Private method to seed the random generator
         * Encapsulate the seeding method to avoid manipulation
         * The function is marked as const to ensure no modification if perform on the Tree structure
         */
        /* void initializeRandomGenerator() const {
            std::srand(static_cast<unsigned>(std::time(NULL))); // Seed the random number generator globally
        } */

        std::mt19937 rand_num_gen;  // Each instance gets its own rng (random number generator). Seeding happens in the constructor
    
        /**
         * @brief Function to get a value of a given key randomly. 
         * Value that has a higher count will has a higher chance of being returned.
         * 
         * @param Keytype & k key value
         * @param AvlNode * root: Pointer to the root Node of the substree
         * @return ValueType value
         */
        ValueType getRandVar(const KeyType & k, AvlNode* root) {
            AvlNode* node = find(k, this->root);
            if (node == nullptr) {
                std::cerr << "Key not found in getRandVar: \'" << k << "\'" << std::endl;
                throw std::runtime_error("Key not found");
            }
            int totalWeight = 0;
            //Calculate total weight
            for (const auto& vc : node->value_count) {
                    totalWeight += vc.count;
            }
            // Generate a random number between 0 and totalWeight - 1
            std::uniform_int_distribution<int> dist(0, totalWeight - 1);
            int randNum = dist(rand_num_gen);
            // Traverse the array and select the value based on the random number
            int cumulativeWeight = 0;
            for (const auto& vc : node->value_count) {
                cumulativeWeight += vc.count;
                if (randNum < cumulativeWeight) {
                    return vc.value; // Pick this value
                }
            }
            return ValueType(); //Fallback, though we should never reach here
        }


    public:
        /**
         * @brief Constructs an empty AVL Tree.
         */
        AVLTree() : root(nullptr){
            //Change the seed everytime the new AVLTree Object is created
            std::random_device ran_device;
            rand_num_gen.seed(ran_device()); //seed the rng
        }

        /**
         * @brief Constructs an empty AVL Tree.
         */
        ~AVLTree() {
            clear(root);
        }

        // PUBLIC METHOD //
        /**
         * @brief Function that return number of keys in the Avl Tree
         */
        int size()const;

        /**
         * @brief Function that return True if the tree is empty, and False otherwise
         */
        bool empty() const;

        /**
         * @brief Inserts an element into the AVL Tree.
         *
         * This function inserts the element into the AVL Tree, ensuring that the tree remains balanced after insertion.
         * The element is placed based on binary search tree insertion rules.
         *
         * @param KeyType k The key to insert into the tree.
         * @param ValueType v The value of the key
         */
        void insert(const KeyType & k, const ValueType & v);

        /**
         * @brief Remove an element from the AVL Tree
         * 
         * This function remove the element in the AVL Tree with the given key
         * 
         * @param KeyType k
         */
        void remove(const KeyType & k);

        /**
         * @brief Public method to find an element with a given key and print it's value_count.
         * This function is marked as const to ensure no modification is performed on the AvlTree structure
         * 
         * @param Keytype k
         */
        void find(const KeyType & k) const;

        /**
         * @brief Public method to get a random value weighted by the count
         * Function is marked as const to ensure no modification is performed on the Tree Structure
         * @param Keytype & k
         * @return ValueType value
         */
        ValueType getRandVar(const KeyType & k);

        /**
         * @brief Public method that displays the AVL Tree in in-order traversal.
         *
         * This function prints the elements of the AVL Tree in sorted order by performing an in-order traversal.
         */
        void display() const;
};

//***IMPLEMENTATION OF PUBLIC METHOD***//
// Implementation of public insert(key, value)
template <typename KeyType, typename ValueType>
void AVLTree<KeyType, ValueType>::insert(const KeyType & x, const ValueType & v) {
    insert(x, v, this->root);
}

// Implementation of public size()
template <typename KeyType, typename ValueType>
int AVLTree<KeyType, ValueType>::size() const {
    return size(this->root);
}

// Implementation of public empty()
template <typename KeyType, typename ValueType>
bool AVLTree<KeyType, ValueType>::empty() const {
    return empty(this->root);
}

//Impletementation of public find(key)
template <typename KeyType, typename ValueType>
void AVLTree<KeyType, ValueType>::find(const KeyType & k) const {
    AvlNode* node = find(k, this->root);
    if (node == nullptr){
        std::cout << "No element found" << std::endl;
    }
    else {
        for(const auto & vc : node->value_count){
            std::cout << "[Value: " << vc.value << ", Count: " << vc.count << "] " << std::endl;
        }
    }
    std::cout << std::endl;
}

//Implementation of public remove(key)
template <typename KeyType, typename ValueType>
void AVLTree<KeyType, ValueType>::remove(const KeyType & k){
    try {
        remove(k, this->root);  // Call private remove method
        std::cout << "Remove successful" << std::endl;  // Print success message if no error occurs
    } catch (const std::runtime_error &e) {
        std::cerr << "Error: " << e.what() << std::endl;  // Handle error if key not found
    }
}

//Implementation of public getRandVar(key)
template <typename KeyType, typename ValueType>
ValueType AVLTree<KeyType, ValueType>::getRandVar(const KeyType & k) {
    return getRandVar(k,this->root);
}

// Implementation of public display()
template <typename KeyType, typename ValueType>
void AVLTree<KeyType, ValueType>::display() const {
    if (this->root == nullptr) {
        std::cout << "The tree is empty" << std::endl;
    } else {
        display(this->root);
        std::cout << std::endl;
    }
}


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

    AVLTree<std::string,std::string> stringTree;//Declare the Tree structure
    
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
    //Insert to the Tree
    stringTree.insert(std::string(buffer),std::string(1,peek_char));
    
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
        //Insert to the Tree
        stringTree.insert(std::string(buffer),std::string(1,peek_char));
    }
    file.close();
    delete[] buffer; // Clean up dynamically allocated memory
    //stringTree.display();
    //===================DONE STORING INPUT=====================//
    /// Work on the output
    std::string outString = firstString; // Create an output string and initialize it with firstString
    std::string windowString = firstString; // Window of sliding characters
    std::cout << "Initial Output String: " << outString << std::endl;
    // Get initial random value based on the first key
    std::string key = windowString; // Initialize the key with windowString
    

    try {
        std::string toAdd = stringTree.getRandVar(key); // Get random value for key
        int k = 0;
        while (outString.length() < infileLength) {
            outString += toAdd;          // Append the random value to the output string
            windowString.erase(0, 1);     // Remove the first character of windowString
            windowString += toAdd;        // Append the new character at the end

            // Update the key with the updated windowString
            key = windowString;
            toAdd = stringTree.getRandVar(key); // Get new random value based on updated key
            k++;
        }
    } catch (const std::runtime_error & e) {
        std::cout << "Caught runtime_error: " << e.what() << std::endl;
    }
    //outString.pop_back(); outString.pop_back();  // Remove garbage
    
    std::cout << "====Final String====" << std::endl;
    //std::cout << "\'" << outString << "\'" << std::endl;

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

