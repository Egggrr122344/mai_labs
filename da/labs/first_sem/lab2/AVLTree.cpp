#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <cstdint> 

// Определение класса AVLTree
class AVLTree {
private:
    struct Node {
        std::string key;
        uint64_t value; 
        Node* left;
        Node* right;
        int height;

        Node(const std::string& key, uint64_t value) 
            : key(key), value(value), left(nullptr), right(nullptr), height(1) {}
    };

    Node* root;

    int height(Node* node);
    int balanceFactor(Node* node);
    Node* rotateRight(Node* node);
    Node* rotateLeft(Node* node);
    Node* balance(Node* node);
    Node* insert(Node* node, const std::string& key, uint64_t value); 
    Node* minValueNode(Node* node);
    Node* remove(Node* node, const std::string& key);
    void destroy(Node* node);
    void serialize(Node* node, std::ofstream& outFile);
    bool deserialize(Node*& node, std::ifstream& inFile);
    
    

public:
    AVLTree() : root(nullptr) {}
    ~AVLTree() { destroy(root); }
    void insert(const std::string& key, uint64_t value); 
    void remove(const std::string& key);
    bool search(const std::string& key, uint64_t& value); 
    bool search(const std::string& key);
    bool isEmpty();
    bool serialize(std::ofstream& outFile);
    bool deserialize(std::ifstream& inFile);
    int size(Node* node);
    Node* getRoot() const {
        return root;
    }

};

// Определение класса Dictionary
class Dictionary {
private:
    AVLTree tree;

public:
    void processCommands(const std::string& inputFile, const std::string& outputFile);
    void executeCommand(const std::string& command, std::ofstream& outputFile);
    void saveDictionary(const std::string& filePath);
    void loadDictionary(const std::string& filePath);

};

// Реализация методов класса AVLTree
int AVLTree::height(Node* node) {
    if (node == nullptr) return 0;
    return node->height;
}

int AVLTree::balanceFactor(Node* node) {
    if (node == nullptr) return 0;
    return height(node->left) - height(node->right);
}

AVLTree::Node* AVLTree::rotateRight(Node* node) {
    Node* newRoot = node->left;
    node->left = newRoot->right;
    newRoot->right = node;
    node->height = std::max(height(node->left), height(node->right)) + 1;
    newRoot->height = std::max(height(newRoot->left), height(newRoot->right)) + 1;
    return newRoot;
}

AVLTree::Node* AVLTree::rotateLeft(Node* node) {
    Node* newRoot = node->right;
    node->right = newRoot->left;
    newRoot->left = node;
    node->height = std::max(height(node->left), height(node->right)) + 1;
    newRoot->height = std::max(height(newRoot->left), height(newRoot->right)) + 1;
    return newRoot;
}

AVLTree::Node* AVLTree::balance(Node* node) {
    if (node == nullptr) return nullptr;

    node->height = std::max(height(node->left), height(node->right)) + 1;

    int bf = balanceFactor(node);

    if (bf > 1) {
        if (balanceFactor(node->left) < 0) {
            node->left = rotateLeft(node->left);
        }
        return rotateRight(node);
    } else if (bf < -1) {
        if (balanceFactor(node->right) > 0) {
            node->right = rotateRight(node->right);
        }
        return rotateLeft(node);
    }

    return node;
}

AVLTree::Node* AVLTree::insert(Node* node, const std::string& key, uint64_t value) {
    if (node == nullptr) return new Node(key, value);

    if (key < node->key) {
        node->left = insert(node->left, key, value);
    } else if (key > node->key) {
        node->right = insert(node->right, key, value);
    }

    return balance(node);
}


AVLTree::Node* AVLTree::minValueNode(Node* node) {
    Node* current = node;
    while (current->left != nullptr) {
        current = current->left;
    }
    return current;
}

AVLTree::Node* AVLTree::remove(Node* node, const std::string& key) {
    if (node == nullptr) return nullptr;

    

    if (key < node->key) {
        
        node->left = remove(node->left, key);
    } else if (key > node->key) {
        
        node->right = remove(node->right, key);
    } else {
        
        if (node->left == nullptr || node->right == nullptr) {
            Node* temp = node->left ? node->left : node->right;
            if (temp == nullptr) {
                temp = node;
                node = nullptr;
            } else {
                *node = *temp;
            }
            delete temp;
        } else {
            Node* temp = minValueNode(node->right);
            node->key = temp->key;
            node->value = temp->value;
            node->right = remove(node->right, temp->key);
        }
    }

    if (node == nullptr) return nullptr;

    return balance(node);
}


void AVLTree::destroy(Node* node) {
    if (node != nullptr) {
        destroy(node->left);
        destroy(node->right);
        delete node;
    }
}

void AVLTree::insert(const std::string& key, uint64_t value) { 
    root = insert(root, key, value);
}

void AVLTree::remove(const std::string& key) {
    root = remove(root, key);
}

bool AVLTree::search(const std::string& key, uint64_t& value) {
    Node* current = root;
    while (current != nullptr) {
        if (key == current->key) {
            value = current->value;
            return true;
        } else if (key < current->key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return false;
}



bool AVLTree::search(const std::string& key) {
    uint64_t value; 
    return search(key, value);
}

bool AVLTree::isEmpty() {
    return root == nullptr;
}

void AVLTree::serialize(Node* node, std::ofstream& outFile) {
    if (node == nullptr) {
        outFile << "# "; 
    } else {
        outFile << node->key << " " << node->value << " ";
        serialize(node->left, outFile);
        serialize(node->right, outFile);
    }
}

bool AVLTree::deserialize(Node*& node, std::ifstream& inFile) {
    std::string key;
    uint64_t value; 
    if (!(inFile >> key)) return false;
    if (key == "#") {
        node = nullptr;
    } else {
        if (!(inFile >> value)) return false;
        node = new Node(key, value);
        if (!deserialize(node->left, inFile)) return false;
        if (!deserialize(node->right, inFile)) return false;
    }
    return true;
}

bool AVLTree::serialize(std::ofstream& outFile) {
    serialize(root, outFile);
    return true;
}

bool AVLTree::deserialize(std::ifstream& inFile) {
    if (!deserialize(root, inFile)) {
        destroy(root);
        root = nullptr;
        return false;
    }
    return true;
}

int AVLTree::size(Node* node) {
    if (node == nullptr) return 0;

    return 1 + size(node -> left) + size(node -> right);
}





// Реализация методов класса Dictionary
void Dictionary::processCommands(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream inFile(inputFile);
    std::ofstream outFile(outputFile);

    std::string line;
    while (std::getline(inFile, line)) {
        executeCommand(line, outFile);
    }

    inFile.close();
    outFile.close();
}

void Dictionary::executeCommand(const std::string& command, std::ofstream& outputFile) {
    std::istringstream iss(command);
    std::string operation;
    iss >> operation;

    if (operation == "+") {
        std::string word;
        uint64_t value; 
        iss >> word >> value;
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);

        if (!tree.search(word)) {
            tree.insert(word, value);
            outputFile << "OK" << std::endl;
        } else {
            outputFile << "Exist" << "\n";
        }
        
    } else if (operation == "-") {
        std::string word;
        iss >> word;
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);

        if (tree.search(word)) {
            tree.remove(word);
            outputFile << "OK" << std::endl;
        } else {
            outputFile << "NoSuchWord" << std::endl;
        }
    } else if (operation == "!") {
        std::string commandType, filePath;
        iss >> commandType >> filePath;
        if (commandType == "Save") {
            saveDictionary(filePath);
            outputFile << "OK" << std::endl;
        } else if (commandType == "Load") {
            loadDictionary(filePath);
            outputFile << "OK" << std::endl;
        }
    } else {
        std::string word = operation;
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        uint64_t value; 
        if (tree.search(word, value)) {
            outputFile << "OK: " << value << std::endl;
        } else {
            outputFile << "NoSuchWord" << std::endl;
        }
    }
}


void Dictionary::saveDictionary(const std::string& filePath) {
    std::ofstream outFile(filePath, std::ios::binary);

    // Сохраняем дерево в файл
    if (tree.serialize(outFile)) {
        std::cout << "OK" << std::endl;
    } else {
        std::cerr << "ERROR: Failed to write dictionary to file" << std::endl;
    }

    outFile.close();
}

void Dictionary::loadDictionary(const std::string& filePath) {
    std::ifstream inFile(filePath, std::ios::binary);

    // Загружаем дерево из файла
    if (!tree.deserialize(inFile)) {
        std::cerr << "ERROR: Failed to read dictionary from file" << std::endl;
    } else {
        std::cout << "OK" << std::endl;
    }

    inFile.close();
}




int main() {
    
    Dictionary dictionary;

    dictionary.processCommands("input.txt", "output.txt");


    

}
