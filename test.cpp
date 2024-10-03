#include <iostream>
#include <algorithm>

template <typename Comparable>
class AVLTree {
private:
    // AVL Tree Node
    struct AvlNode {
        Comparable element;
        AvlNode* left;
        AvlNode* right;
        int height;

        AvlNode(const Comparable& ele, AvlNode* lt = nullptr, AvlNode* rt = nullptr, int h = 0)
            : element(ele), left(lt), right(rt), height(h) {}
    };

    AvlNode* root;

    // Helper function declarations
    int height(AvlNode* t) const;
    void balance(AvlNode*& t);
    void rotateWithLeftChild(AvlNode*& k2);
    void rotateWithRightChild(AvlNode*& k2);
    void doubleWithLeftChild(AvlNode*& k3);
    void doubleWithRightChild(AvlNode*& k3);
    void insert(const Comparable& x, AvlNode*& t);
    void display(AvlNode* root) const;
    void clear(AvlNode* t);

public:
    // Constructor and Destructor inside the class
    AVLTree() : root(nullptr) {} 

    ~AVLTree() {
        clear(root);
    }

    // Public method declarations
    void insert(const Comparable& x);
    void display() const;
};

// Method implementations outside the class

template <typename Comparable>
int AVLTree<Comparable>::height(AvlNode* t) const {
    return (t == nullptr) ? -1 : t->height;
}

template <typename Comparable>
void AVLTree<Comparable>::balance(AvlNode*& t) {
    static const int ALLOWED_IMBALANCE = 1;

    if (t == nullptr) return;

    if (height(t->left) - height(t->right) > ALLOWED_IMBALANCE) {
        if (height(t->left->left) >= height(t->left->right))
            rotateWithLeftChild(t);
        else
            doubleWithLeftChild(t);
    } else if (height(t->right) - height(t->left) > ALLOWED_IMBALANCE) {
        if (height(t->right->right) >= height(t->right->left))
            rotateWithRightChild(t);
        else
            doubleWithRightChild(t);
    }

    t->height = std::max(height(t->left), height(t->right)) + 1;
}

template <typename Comparable>
void AVLTree<Comparable>::rotateWithLeftChild(AvlNode*& k2) {
    AvlNode* k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;

    k2->height = std::max(height(k2->left), height(k2->right)) + 1;
    k1->height = std::max(height(k1->left), height(k2)) + 1;

    k2 = k1;
}

template <typename Comparable>
void AVLTree<Comparable>::rotateWithRightChild(AvlNode*& k2) {
    AvlNode* k1 = k2->right;
    k2->right = k1->left;
    k1->left = k2;

    k2->height = std::max(height(k2->left), height(k2->right)) + 1;
    k1->height = std::max(height(k1->right), height(k2)) + 1;

    k2 = k1;
}

template <typename Comparable>
void AVLTree<Comparable>::doubleWithLeftChild(AvlNode*& k3) {
    rotateWithRightChild(k3->left);
    rotateWithLeftChild(k3);
}

template <typename Comparable>
void AVLTree<Comparable>::doubleWithRightChild(AvlNode*& k3) {
    rotateWithLeftChild(k3->right);
    rotateWithRightChild(k3);
}

template <typename Comparable>
void AVLTree<Comparable>::insert(const Comparable& x, AvlNode*& t) {
    if (t == nullptr)
        t = new AvlNode(x);
    else if (x < t->element)
        insert(x, t->left);
    else if (t->element < x)
        insert(x, t->right);

    balance(t);
}

template <typename Comparable>
void AVLTree<Comparable>::insert(const Comparable& x) {
    insert(x, root);
}

template <typename Comparable>
void AVLTree<Comparable>::display(AvlNode* root) const {
    if (root != nullptr) {
        display(root->left);
        std::cout << root->element << " ";
        display(root->right);
    }
}

template <typename Comparable>
void AVLTree<Comparable>::display() const {
    display(root);
    std::cout << std::endl;
}

template <typename Comparable>
void AVLTree<Comparable>::clear(AvlNode* t) {
    if (t != nullptr) {
        clear(t->left);
        clear(t->right);
        delete t;
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
