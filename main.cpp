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
 * @struct AvlNode
 * @brief Represents a node in the AVL Tree.
 *
 * The AvlNode struct is used to store individual elements of the AVL Tree. Each node contains an element (the key),
 * pointers to its left and right children, and its height within the tree.
 *
 * @tparam Comparable The data type to store in the node, which must support comparison operators.
 */
template <typename Comparable>
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
    AvlNode *root; // Pointer to the root node

    //Helper function declarations
    int height(AvlNode *t) const;

public:
    /**
     * @brief Constructs an empty AVL Tree.
     */
    AVLTree() : root(nullptr);

    /**
     * @brief Constructs an empty AVL Tree.
     */
    ~AVLTree() {
        clear(root);
    };

};