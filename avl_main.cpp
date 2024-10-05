/**
* CS/SE 3345 - Programming Assignment 2 3 - Monkey Character Distribution
* Lecturer: Andrew Nemec
* Name: Tan Han Nguyen
* NetID: TXN 200004
* Date: October 1, 2024
* Due Date: October 18, 2024
*/ 
#include <iostream>



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
template <typename Comparable>
class AVLTree {
private:
    /**
     * @struct AvlNode
     * @brief Represents a node in the AVL Tree.
     *
     * The AvlNode struct is used to store individual elements of the AVL Tree. Each node contains an element (the key),
     * pointers to its left and right children, and its height within the tree.
     */
    struct AvlNode {
        Comparable element; //Any comparable types: int, float, double, char, std::string, or any custom comparable class or struct
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
    AvlNode(const Comparable& ele, AvlNode *lt = nullptr, AvlNode *rt = nullptr, int h = -1)
            : element(ele), left(lt), right(rt), height(h) {}
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
     * @brief Recursively inserts a new element into the subtree rooted at the given node.
     *
     * This function inserts the element into the correct position in the subtree while maintaining the binary search
     * tree property. After insertion, the function calls `balance` to ensure that the subtree remains balanced.
     *
     * @param x The element to insert.
     * @param t Reference to the pointer to the root of the subtree in which to insert the element.
     */
    void insert(const Comparable & x, AvlNode* & t){
        if(t == nullptr) {
            t = new AvlNode(x, nullptr, nullptr);
        }
        else if (x < t->element){
            insert(x, t->left);
        }
        else if (x > t->element){
            insert(x, t->right);
        }
        balance(t);

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
     * @brief Private method that
     * Traverses the subtree rooted at the given node 
     * Recursively performs an in-order traversal of the tree
     * Display elements in sorted order (left - root - right)
     * @note This function is marked as const to prevent modification to the internal structure of the AVLTree
     *
     * @param root Pointer to the root of the subtree to display.
     */
    void display(AvlNode * root) const{
        if(root != nullptr){
            display(root->left);
            std::cout << root->element << " ";
            display(root->right);
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
     * @brief Inserts an element into the AVL Tree.
     *
     * This function inserts the element into the AVL Tree, ensuring that the tree remains balanced after insertion.
     * The element is placed based on binary search tree insertion rules.
     *
     * @param x The element to insert into the tree.
     */
    void insert(const Comparable& x);

    /**
     * @brief Public method that displays the AVL Tree in in-order traversal.
     *
     * This function prints the elements of the AVL Tree in sorted order by performing an in-order traversal.
     */
    void display() const;
};

//***IMPLEMENTATION OF PUBLIC METHOD***//
//Implementation of public insert
template<typename Comparable>
void AVLTree<Comparable>::insert(const Comparable & x){
    insert(x,this->root);
}
//Implementation of public display
template<typename Comparable>
void AVLTree<Comparable>::display() const {
    if (this->root == nullptr) {
        std::cout << "The tree is empty" << std::endl;
    }
    else {
        display(this->root);
        std::cout << std::endl;
    }
}


int main() {
    // Use the AVL tree for integers
    AVLTree<int> tree;

    tree.insert(20);
    tree.insert(25);
    tree.insert(15);
    tree.insert(10);
    tree.insert(5);
    tree.insert(8);
    tree.insert(30);
    tree.insert(35);

    std::cout << "In-order traversal of integer AVL tree: ";
    tree.display();

    // Use the AVL tree for strings
    AVLTree<std::string> stringTree;
    stringTree.insert("banana");
    stringTree.insert("apple");
    stringTree.insert("orange");

    std::cout << "In-order traversal of string AVL tree: ";
    stringTree.display();

    return 0;
}

