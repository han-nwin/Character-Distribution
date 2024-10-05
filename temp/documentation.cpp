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
     *
     * @tparam Comparable The data type to store in the node, which must support comparison operators.
     */
    struct AvlNode {
        Comparable element;       ///< The value stored in the node.
        AvlNode* left;            ///< Pointer to the left child node.
        AvlNode* right;           ///< Pointer to the right child node.
        int height;               ///< The height of the node in the tree.

        /**
         * @brief Constructs an AVL Node.
         * @param ele The element (key) to store in the node.
         * @param lt Pointer to the left child node (defaults to nullptr).
         * @param rt Pointer to the right child node (defaults to nullptr).
         * @param h Height of the node (defaults to 0).
         */
        AvlNode(const Comparable& ele, AvlNode* lt = nullptr, AvlNode* rt = nullptr, int h = 0)
            : element(ele), left(lt), right(rt), height(h) {}
    };

    AvlNode* root;  ///< Pointer to the root node of the AVL Tree.

    /**
     * @brief Helper function to get the height of a node.
     * @param t Pointer to the node.
     * @return The height of the node. Returns -1 if the node is nullptr.
     */
    int height(AvlNode* t) const;

    /**
     * @brief Balances the subtree rooted at the given node.
     * 
     * This function checks the balance factor of the node and applies the appropriate rotations (single or double)
     * to ensure that the subtree rooted at the given node is height-balanced.
     *
     * @param t Reference to the pointer to the root of the subtree to be balanced.
     */
    void balance(AvlNode*& t);

    /**
     * @brief Performs a single rotation with the left child to balance a left-heavy subtree.
     * @param k2 Reference to the pointer to the root of the subtree to rotate.
     */
    void rotateWithLeftChild(AvlNode*& k2);

    /**
     * @brief Performs a single rotation with the right child to balance a right-heavy subtree.
     * @param k2 Reference to the pointer to the root of the subtree to rotate.
     */
    void rotateWithRightChild(AvlNode*& k2);

    /**
     * @brief Performs a double rotation: first a right rotation on the left child, then a left rotation.
     * 
     * This is used to balance a left-heavy subtree where the left child itself is right-heavy.
     *
     * @param k3 Reference to the pointer to the root of the subtree to rotate.
     */
    void doubleWithLeftChild(AvlNode*& k3);

    /**
     * @brief Performs a double rotation: first a left rotation on the right child, then a right rotation.
     * 
     * This is used to balance a right-heavy subtree where the right child itself is left-heavy.
     *
     * @param k3 Reference to the pointer to the root of the subtree to rotate.
     */
    void doubleWithRightChild(AvlNode*& k3);

    /**
     * @brief Recursively inserts a new element into the subtree rooted at the given node.
     *
     * This function inserts the element into the correct position in the subtree while maintaining the binary search
     * tree property. After insertion, the function calls `balance` to ensure that the subtree remains balanced.
     *
     * @param x The element to insert.
     * @param t Reference to the pointer to the root of the subtree in which to insert the element.
     */
    void insert(const Comparable& x, AvlNode*& t);

    /**
     * @brief Recursively performs an in-order traversal of the tree to display elements in sorted order.
     *
     * This function traverses the subtree rooted at the given node in in-order fashion (left, root, right)
     * and prints each element.
     *
     * @param root Pointer to the root of the subtree to display.
     */
    void display(AvlNode* root) const;

public:
    /**
     * @brief Constructs an empty AVL Tree.
     */
    AVLTree();

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
     * @brief Displays the AVL Tree in in-order traversal.
     *
     * This function prints the elements of the AVL Tree in sorted order by performing an in-order traversal.
     */
    void display() const;
};
