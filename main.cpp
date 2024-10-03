#include <iostream>
#include <algorithm>

class AVLTree {
private:
    // AVL Tree Node
    struct AvlNode {
        int element;
        AvlNode* left;
        AvlNode* right;
        int height;

        AvlNode(int ele, AvlNode* lt = nullptr, AvlNode* rt = nullptr, int h = 0)
            : element(ele), left(lt), right(rt), height(h) {}
    };

    // Helper function to get the height of a node
    int height(AvlNode* t) const {
        return (t == nullptr) ? -1 : t->height;
    }

    // Function to balance the AVL tree
    void balance(AvlNode*& t) {
        static const int ALLOWED_IMBALANCE = 1;

        if (t == nullptr) return;  // Empty node, nothing to balance

        // Left-heavy
        if (height(t->left) - height(t->right) > ALLOWED_IMBALANCE) {
            if (height(t->left->left) >= height(t->left->right)) {
                rotateWithLeftChild(t);  // Single rotation
            } else {
                doubleWithLeftChild(t);  // Double rotation
            }
        }
        // Right-heavy
        else if (height(t->right) - height(t->left) > ALLOWED_IMBALANCE) {
            if (height(t->right->right) >= height(t->right->left)) {
                rotateWithRightChild(t);  // Single rotation
            } else {
                doubleWithRightChild(t);  // Double rotation
            }
        }

        // Update height after balancing
        t->height = std::max(height(t->left), height(t->right)) + 1;
    }

    // Single rotation with left child
    void rotateWithLeftChild(AvlNode*& k2) {
        AvlNode* k1 = k2->left;
        k2->left = k1->right;
        k1->right = k2;

        // Update heights
        k2->height = std::max(height(k2->left), height(k2->right)) + 1;
        k1->height = std::max(height(k1->left), height(k2)) + 1;

        // Set new root
        k2 = k1;
    }

    // Single rotation with right child
    void rotateWithRightChild(AvlNode*& k2) {
        AvlNode* k1 = k2->right;
        k2->right = k1->left;
        k1->left = k2;

        // Update heights
        k2->height = std::max(height(k2->left), height(k2->right)) + 1;
        k1->height = std::max(height(k1->right), height(k2)) + 1;

        // Set new root
        k2 = k1;
    }

    // Double rotation with left child
    void doubleWithLeftChild(AvlNode*& k3) {
        rotateWithRightChild(k3->left);
        rotateWithLeftChild(k3);
    }

    // Double rotation with right child
    void doubleWithRightChild(AvlNode*& k3) {
        rotateWithLeftChild(k3->right);
        rotateWithRightChild(k3);
    }

    // Insert function
    void insert(const int& x, AvlNode*& t) {
        if (t == nullptr)
            t = new AvlNode(x);
        else if (x < t->element)
            insert(x, t->left);
        else if (t->element < x)
            insert(x, t->right);

        balance(t);  // Balance the tree after insertion
    }

    // In-order display
    void display(AvlNode* root) const {
        if (root != nullptr) {
            display(root->left);
            std::cout << root->element << " ";
            display(root->right);
        }
    }

    // Root node of the AVL Tree
    AvlNode* root;

public:
    // Constructor
    AVLTree() : root(nullptr) {}

    // Public method to insert an element
    void insert(int x) {
        insert(x, root);
    }

    // Public method to display the tree (in-order traversal)
    void display() const {
        display(root);
        std::cout << std::endl;
    }
};

int main() {
    AVLTree tree;

    // Insert elements into the AVL tree
    tree.insert(20);
    tree.insert(25);
    tree.insert(15);
    tree.insert(10);
    tree.insert(5);

    // Display the AVL tree (in-order traversal)
    std::cout << "In-order traversal of AVL tree: ";
    tree.display();

    return 0;
}
