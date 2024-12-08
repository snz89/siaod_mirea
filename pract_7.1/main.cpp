#include <iostream>
#include <iomanip>
#include <queue>
#include <vector>

using namespace std;

class AVLTree {
private:
    struct Node {
        double data;
        int height;
        Node* left;
        Node* right;

        Node(double data) : data(data), height(1), left(nullptr), right(nullptr) {}
    };

    Node* root;

    int height(Node* node) const {
        return node ? node->height : 0;
    }

    int getBalance(Node* node) const {
        return node ? height(node->left) - height(node->right) : 0;
    }

    Node* rightRotate(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        return x;
    }

    Node* leftRotate(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        return y;
    }

    void deleteAllNodes(Node* node) {
        if (node) {
            deleteAllNodes(node->left);
            deleteAllNodes(node->right);
            delete node;
        }
    }

public:
    AVLTree() : root(nullptr) {}

    ~AVLTree() {
        deleteAllNodes(root);
        root = nullptr;
    }

    void insert(double data) {
        if (root == nullptr) {
            root = new Node(data);
            return;
        }

        Node* current = root;
        Node* parent = nullptr;

        while (current != nullptr) {
            parent = current;
            if (data < current->data) {
                current = current->left;
            } else if (data > current->data) {
                current = current->right;
            } else {
                return; // Duplicates are not allowed
            }
        }

        if (data < parent->data) {
            parent->left = new Node(data);
        } else {
            parent->right = new Node(data);
        }

        // Rebalance the tree after insertion
        current = root;
        while (current != nullptr) {
            current->height = 1 + max(height(current->left), height(current->right));
            int balance = getBalance(current);

            // Left Left Case
            if (balance > 1 && data < current->left->data) {
              if(current == root){
                root = rightRotate(current);
              } else {
                (data < parent->data ? parent->left : parent->right) = rightRotate(current);
              }
            }

            // Right Right Case
            else if (balance < -1 && data > current->right->data) {
              if(current == root){
                root = leftRotate(current);
              } else {
                (data < parent->data ? parent->left : parent->right) = leftRotate(current);
              }
            }

            // Left Right Case
            else if (balance > 1 && data > current->left->data) {
              current->left = leftRotate(current->left);
              if(current == root){
                root = rightRotate(current);
              } else {
                (data < parent->data ? parent->left : parent->right) = rightRotate(current);
              }
            }

            // Right Left Case
            else if (balance < -1 && data < current->right->data) {
              current->right = rightRotate(current->right);
              if(current == root){
                root = leftRotate(current);
              } else {
                (data < parent->data ? parent->left : parent->right) = leftRotate(current);
              }
            }
          
            if(data < current->data){
              parent = current;
              current = current->left;
            } else {
              parent = current;
              current = current->right;
            }
        }
    }

    vector<double> postOrder() const {
        vector<double> result;
        vector<Node*> stack;
        Node* current = root;
        Node* lastVisited = nullptr;

        while (current != nullptr || !stack.empty()) {
            if (current != nullptr) {
                stack.push_back(current);
                current = current->left;
            } else {
                Node* peekNode = stack.back();
                if (peekNode->right != nullptr && lastVisited != peekNode->right) {
                    current = peekNode->right;
                } else {
                    result.push_back(peekNode->data);
                    lastVisited = stack.back();
                    stack.pop_back();
                }
            }
        }
        return result;
    }

    vector<double> inOrder() const {
        vector<double> result;
        vector<Node*> stack;
        Node* current = root;

        while (current != nullptr || !stack.empty()) {
            while (current != nullptr) {
                stack.push_back(current);
                current = current->left;
            }
            current = stack.back();
            stack.pop_back();
            result.push_back(current->data);
            current = current->right;
        }
        return result;
    }

    double sumOfLeaves() const {
        double sum = 0;
        vector<Node*> stack;
        Node* current = root;

        while (current != nullptr || !stack.empty()) {
            while (current != nullptr) {
                stack.push_back(current);
                current = current->left;
            }
            current = stack.back();
            stack.pop_back();

            if (current->left == nullptr && current->right == nullptr) {
                sum += current->data;
            }

            current = current->right;
        }

        return sum;
    }

    double calculateAverage() const {
        double sum = 0;
        int count = 0;
        vector<Node*> stack;
        Node* current = root;
        while (current != nullptr || !stack.empty()) {
            while (current != nullptr) {
                stack.push_back(current);
                current = current->left;
            }
            current = stack.back();
            stack.pop_back();
            sum += current->data;
            count++;
            current = current->right;
        }
        return count > 0 ? sum / count : 0;
    }
};

int main() {
    AVLTree tree;
    int choice;

    do {
        cout << "\nMenu:\n";
        cout << "1. Insert element\n";
        cout << "2. PostOrder traversal\n";
        cout << "3. InOrder traversal\n";
        cout << "4. Find the sum of leaf values\n";
        cout << "5. Find the average of all nodes\n";
        cout << "0. Exit\n";
        cout << "Select an action: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                double value;
                cout << "Enter a real number to insert: ";
                cin >> value;
                tree.insert(value);
                break;
            }
            case 2: {
                vector<double> postOrderResult = tree.postOrder();
                cout << "PostOrder (Reverse traversal): ";
                for (double val : postOrderResult) {
                    cout << setprecision(2) << val << " ";
                }
                cout << endl;
                break;
            }
            case 3: {
                vector<double> inOrderResult = tree.inOrder();
                cout << "InOrder (Symmetric traversal): ";
                for (double val : inOrderResult) {
                    cout << setprecision(2) << val << " ";
                }
                cout << endl;
                break;
            }
            case 4: {
              double sum = tree.sumOfLeaves();
              cout << "Sum of leaf values: " << setprecision(2) << sum << endl;
              break;
            }
            case 5: {
              double avg = tree.calculateAverage();
              cout << "Average of all nodes: " << setprecision(2) << avg << endl;
              break;
            }
            case 0:
                cout << "Exiting the program.\n";
                break;
            default:
                cout << "Incorrect choice. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}