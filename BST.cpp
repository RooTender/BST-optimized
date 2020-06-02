#include <iostream>

#define uInt unsigned int

struct Node {
    uInt value = 0;
    
    Node* parent = nullptr;
    Node* left = nullptr;
    Node* right = nullptr;
};

class BST {
private:
    Node* root = nullptr;
    Node* getNode(const uInt& value) {
        if (root == nullptr)
            return nullptr;

        if (mostLeftNode->value == value)
            return mostLeftNode;

        if (mostRightNode->value == value)
            return mostRightNode;

        Node* node = root;
        while (node->left || node->right) {
            if (node->left && node->value > value) {
                node = node->left;
                continue;
            }

            if (node->right && node->value < value) {
                node = node->right;
                continue;
            }

            break;
        }

        return (node->value == value) ? node : nullptr;
    }
    Node* getNearestNode(const uInt& value) {
        if (root == nullptr)
            return nullptr;

        if (mostLeftNode->value >= value)
            return mostLeftNode;

        if (mostRightNode->value <= value)
            return mostRightNode;

        Node* node = root;
        while (node->left || node->right) {
            if (node->value > value && node->left) {
                node = node->left;
                continue;
            }

            if (node->value < value && node->right) {
                node = node->right;
                continue;
            }

            break;
        }

        return node;
    }

    Node* mostLeftNode = nullptr;
    Node* mostRightNode = nullptr;

public:
    BST() = default;

    void insert(uInt value) {

        if (root == nullptr) {
            root = new Node;
            root->value = value;

            mostLeftNode = root;
            mostRightNode = root;

            return;
        }

        Node* node = getNearestNode(value);

        if (node->value == value) {
            return;
        }

        if (node->value > value) {
            node->left = new Node;
            node->left->parent = node;
            node = node->left;
        }
        else {
            node->right = new Node;
            node->right->parent = node;
            node = node->right;
        }

        node->value = value;

        // Update MIN & MAX if possible
        if (mostLeftNode->value > value) {
            mostLeftNode = node;
        }
        if (mostRightNode->value < value) {
            mostRightNode = node;
        }
    }
    bool searchFor(uInt value) {
        if (root == nullptr) {
            return false;
        }

        Node* node = getNode(value);

        return (node != nullptr) ? true : false;
    }
    void remove(uInt const& value) {

        if (root == nullptr) {
            return;
        }

        Node* node = getNearestNode(value);
        if (node->value != value)
            return;

        Node* toRemove = nullptr;

        // no nodes under
        if (node->left == nullptr && node->right == nullptr) {
            toRemove = node;

            if (node->parent) {
                node = node->parent;

                if (node->left == toRemove) {
                    if (mostLeftNode->value == toRemove->value)
                        mostLeftNode = node;

                    node->left = nullptr;
                }
                else {
                    if (mostRightNode->value == toRemove->value)
                        mostRightNode = node;

                    node->right = nullptr;
                }
            }
            else {
                root = nullptr;
                mostLeftNode = nullptr;
                mostRightNode = nullptr;
            }

            delete toRemove;
            return;
        }

        // left node only
        if (node->left && node->right == nullptr) {
            toRemove = node->left;

            if (node == mostRightNode && toRemove->right) {
                Node* helper = toRemove;

                while (helper->right)
                    helper = helper->right;

                mostRightNode = helper;
            }

            if (toRemove->left)
                toRemove->left->parent = node;

            if (toRemove->right)
                toRemove->right->parent = node;

            node->value = toRemove->value;
            node->left = toRemove->left;
            node->right = toRemove->right;

            if (node->value == mostLeftNode->value)
                mostLeftNode = node;

            delete toRemove;
            return;
        }

        // right node only
        if (node->right && node->left == nullptr) {
            toRemove = node->right;

            if (node == mostLeftNode && toRemove->left) {
                Node* helper = toRemove;

                while (helper->left)
                    helper = helper->left;

                mostLeftNode = helper;
            }

            if (toRemove->left)
                toRemove->left->parent = node;

            if (toRemove->right)
                toRemove->right->parent = node;

            node->value = toRemove->value;
            node->left = toRemove->left;
            node->right = toRemove->right;

            if (node->value == mostRightNode->value)
                mostRightNode = node;

            delete toRemove;
            return;
        }

        // left and right nodes under
        if (node->left && node->right) {
            toRemove = node->right;

            if (toRemove->left == nullptr && toRemove->right == nullptr) {

                if (mostRightNode == toRemove) {
                    mostRightNode = node;
                }

                node->value = toRemove->value;
                node->right = nullptr;

                delete toRemove;
                return;
            }

            if (toRemove->left == nullptr && toRemove->right) {
                node->value = toRemove->value;
                node->right = toRemove->right;
                toRemove->right->parent = node;

                delete toRemove;
                return;
            }

            if (toRemove->left) {
                while (toRemove->left)
                    toRemove = toRemove->left;

                node->value = toRemove->value;

                if (toRemove->right) {
                    toRemove->right->parent = toRemove->parent;
                    toRemove->parent->left = toRemove->right;
                }
                else {
                    toRemove->parent->left = nullptr;
                }

                delete toRemove;
                return;
            }
        }
    }
    uInt getMin() const {
        if (root)
            return mostLeftNode->value;
        return 0;
    }
    uInt getMax() const {
        if (root)
            return mostRightNode->value;
        return 0;
    }

    ~BST() {

        if (root == nullptr)
            return;

        Node* actual = root;

        while (actual->parent != nullptr || actual->left != nullptr || actual->right != nullptr) {
            
            if (actual->right) {
                actual = actual->right;
                continue;
            }

            if (actual->left) {
                actual = actual->left;
                continue;
            }

            if (actual->parent->right == actual) {
                actual = actual->parent;

                delete actual->right;
                actual->right = nullptr;
            }
            else {
                actual = actual->parent;

                delete actual->left;
                actual->left = nullptr;
            }
        }
        delete actual;
    }
};

int main()
{
    BST tree = BST();
    
    bool quit = false;
    while (!quit) {

        char input;
        uInt value;
        std::cin >> input;

        switch (input) {
        case '+':
            std::cin >> value;
            tree.insert(value);
            break;

        case '-':
            std::cin >> value;
            tree.remove(value);
            break;

        case '?':
            std::cin >> value;
            std::cout << tree.searchFor(value) << std::endl;
            break;

        case 'm':
            std::cout << tree.getMin() << std::endl;
            break;

        case 'M':
            std::cout << tree.getMax() << std::endl;
            break;

        default:
            quit = true;
            break;
        }
    }

    return 0;
}