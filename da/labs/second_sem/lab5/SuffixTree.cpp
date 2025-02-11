#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <set>
#include <list>
#include <algorithm>

using std::cin;
using std::cout;
using std::map;
using std::string;

class Node {
public:
    std::map<char, Node*> children;
    std::string::iterator start, end;
    Node* suffixLink;
    std::set<int> identifierSet;

    Node(std::string::iterator start, std::string::iterator end);
};

struct CommonSubstring {
    int length;
    std::vector<Node*> nodePath;
};

class SuffixTree {
private:
    string text1;
    string text2;

    Node* root;
    Node* activeNode;
    Node* activeLink;
    std::string::iterator activeEdge;

    int numStrings;
    int remainingChars;
    int activeLength;

    int longestLength;
    std::vector<CommonSubstring> commonSubstrings;

    void addCharacter(std::string::iterator it);
    int markNodes(Node* node);
    void calculateMaxLength(Node* node, int currentLength);
    void findCommonSubstrings(Node* node, int currentLength, std::vector<Node*> path);
    void cleanup(Node* node);

public:
    SuffixTree(const string& str1, const string& str2);
    ~SuffixTree() { cleanup(root); }
};

Node::Node(std::string::iterator start, std::string::iterator end)
    : start(start), end(end), suffixLink(nullptr) {}

SuffixTree::SuffixTree(const string& str1, const string& str2) 
    : text1(str1), text2(str2), longestLength(0), numStrings(1) {

    text1 += "#";
    text2 += "$";
    text1 += text2;

    root = new Node(text1.end(), text1.end());
    root->suffixLink = root;
    activeNode = root;
    activeLink = root;
    remainingChars = 0;
    activeLength = 0;

    for (auto it = text1.begin(); it != text1.end(); ++it) {
        addCharacter(it);
        if (*it == '#') {
            numStrings++;
        }
    }
    markNodes(root);
    calculateMaxLength(root, 0);

    std::vector<Node*> path;
    findCommonSubstrings(root, 0, path);

    std::list<string> results;
    for (const auto& common : commonSubstrings) {
        string substring;
        for (const auto& node : common.nodePath) {
            substring += string(node->start, node->end + 1);
        }
        results.push_back(substring);
    }

    results.sort();
    cout << longestLength << std::endl;
    for (const auto& result : results) {
        cout << result << std::endl;
    }
}

void SuffixTree::addCharacter(std::string::iterator it) {
    activeLink = root;
    remainingChars++;

    while (remainingChars > 0) {
        if (activeLength == 0) {
            activeEdge = it;
        }

        auto childIt = activeNode->children.find(*activeEdge);
        if (childIt == activeNode->children.end()) {
            Node* newNode = new Node(it, text1.end());
            activeNode->children[*activeEdge] = newNode;
            if (activeLink != root) {
                activeLink->suffixLink = activeNode;
            }
            activeLink = activeNode;
        } else {
            Node* nextNode = childIt->second;
            int edgeLength = nextNode->end - nextNode->start + 1;

            if (activeLength >= edgeLength) {
                activeEdge += edgeLength;
                activeLength -= edgeLength;
                activeNode = nextNode;
                continue;
            }

            if (*(nextNode->start + activeLength) == *it) {
                activeLength++;
                if (activeLink != root) {
                    activeLink->suffixLink = activeNode;
                }
                activeLink = activeNode;
                break;
            }

            Node* splitNode = new Node(nextNode->start, nextNode->start + activeLength - 1);
            Node* leafNode = new Node(it, text1.end());
            activeNode->children[*activeEdge] = splitNode;
            splitNode->children[*it] = leafNode;
            nextNode->start += activeLength;
            splitNode->children[*nextNode->start] = nextNode;

            if (activeLink != root) {
                activeLink->suffixLink = splitNode;
            }
            activeLink = splitNode;
        }

        remainingChars--;
        if (activeNode == root && activeLength > 0) {
            activeLength--;
            activeEdge++;
        } else {
            activeNode = activeNode->suffixLink ? activeNode->suffixLink : root;
        }
    }
}

int SuffixTree::markNodes(Node* node) {
    if (node->start != text1.end()) {
        if (*node->start == '#')
            node->identifierSet.insert(1);
        else if (*node->start == '$')
            node->identifierSet.insert(2);
    }
    
    if (std::distance(node->start, node->end) > 0) {
        if (std::distance(node->start, node->end) > text2.size())
            node->identifierSet.insert(1);
        else if (node->end == text1.end())
            node->identifierSet.insert(2);
    }

    for (auto& child : node->children) {
        Node* nextChild = child.second;
        int result = markNodes(nextChild);
        if (node->identifierSet.size() < 2) {
            if (result == 3) {
                node->identifierSet.insert(1);
                node->identifierSet.insert(2);
            } else if (result == 1) {
                node->identifierSet.insert(1);
            } else if (result == 2) {
                node->identifierSet.insert(2);
            }
        }
    }

    if (node->identifierSet.size() == 2) {
        return 3;
    }
    if (node->identifierSet.size() == 1) {
        return (*node->identifierSet.begin() == 1) ? 1 : 2;
    }
    return 0;
}

void SuffixTree::calculateMaxLength(Node* node, int currentLength) {
    if (node->identifierSet.size() == 2) {
        if (node != root) {
            currentLength += std::distance(node->start, node->end) + 1;
            if (currentLength > longestLength) {
                longestLength = currentLength;
            }
        }
        for (auto& child : node->children) {
            calculateMaxLength(child.second, currentLength);
        }
    }
}

void SuffixTree::findCommonSubstrings(Node* node, int currentLength, std::vector<Node*> path) {
    if (node->identifierSet.size() == 2) {
        if (node != root) {
            if (currentLength == 0) {
                path.clear();
            }
            path.push_back(node);
            currentLength += std::distance(node->start, node->end) + 1;
            if (currentLength == longestLength) {
                CommonSubstring common;
                common.nodePath = path;
                common.length = currentLength;
                commonSubstrings.push_back(common);
                path.clear();
            }
        }
        for (auto& child : node->children) {
            findCommonSubstrings(child.second, currentLength, path);
        }
    }
}

void SuffixTree::cleanup(Node* node) {
    for (auto& child : node->children) {
        cleanup(child.second);
    }
    delete node;
}

int main() {
    string string1, string2;
    while (cin >> string1 >> string2) {
        SuffixTree tree(string1, string2);
    }
    return 0;
}