/**
* CS/SE 3345 - Programming Assignment 2 3 - Monkey Character Distribution
* Lecturer: Andrew Nemec
* Name: Tan Han Nguyen
* NetID: TXN 200004
* Date: October 1, 2024
* Due Date: October 18, 2024
*/ 
#include <iostream>
#include <algorithm> 
#include <vector>

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
         * @param c count of the value initialized  = 0
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
     * 
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
    /**
    * @brief Recursively inserts a new key and value into the subtree rooted at the given node.
    *
    * This function inserts the key and its associated value into the correct position in the subtree while maintaining the
    * binary search tree property. If the key already exists, it updates the value_count vector by adding or updating the value.
    * After insertion, the function calls `balance` to ensure that the subtree remains balanced.
    *
    * @param KeyType x The key to insert.
    * @param ValueType v The associated value for the key.
    * @param AvlNode t Reference to the pointer to the root of the subtree in which to insert the key-value pair.
    */
    void insert(const KeyType & x, const ValueType & v, AvlNode* & t) {
        if (t == nullptr) {
            // If no node exists, create a new node with the key and value
            t = new AvlNode(x, v);
        }
        else if (x < t->key) {
            insert(x, v, t->left);  // Insert into the left subtree
        }
        else if (x > t->key) {
            insert(x, v, t->right);  // Insert into the right subtree
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
        balance(t);  // Balance the subtree after insertion
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
     * @brief Private method that traverses the subtree rooted at the given node.
     * Recursively performs an in-order traversal of the tree.
     * Displays keys, values, and their counts in sorted order (left - root - right).
     * @param AvlNode root: Pointer to the root of the subtree to display.
     */
    void display(AvlNode* root) const {
        if (root != nullptr) {
            display(root->left);  // Traverse the left subtree

            // Display the key
            std::cout << "Key: " << root->key << " -> ";

            // Display all values and their counts for this key
            std::cout << "Values and counts: ";
            for (const auto& vc : root->value_count) {
                std::cout << "[Value: " << vc.value << ", Count: " << vc.count << "] ";
            }
            std::cout << std::endl;

            display(root->right);  // Traverse the right subtree
        }
    }


public:
    /**
     * @brief Constructs an empty AVL Tree.
     */
    AVLTree() : root(nullptr){}

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
     * @param KeyType x The key to insert into the tree.
     * @param ValueType v The value of the key
     */
    void insert(const KeyType & x, const ValueType & v);

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


int main() {
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

    std::cout << "In-order traversal of integer AVL tree with values and counts:\n";
    intTree.display();
    std::cout << "SIZE: " << intTree.size() << std::endl;
    std::cout << "Empty? "<< intTree.empty() << std::endl;

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

    std::cout << "In-order traversal of string AVL tree with values and counts:\n";
    stringTree.display();
    std::cout << "SIZE: " << stringTree.size() << std::endl;
    std::cout << "Empty? "<< stringTree.empty() << std::endl;

    return 0;
}


