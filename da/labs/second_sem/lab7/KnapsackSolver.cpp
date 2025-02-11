#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>

class ItemNode {
public:
    uint64_t totalValue;
    int itemIndex;
    ItemNode* previousNode;

    ItemNode(uint64_t value, int index, ItemNode* prev)
        : totalValue(value), itemIndex(index), previousNode(prev) {}

    ~ItemNode() {
        if (previousNode != nullptr) {
            delete previousNode;
            previousNode = nullptr;
        }
    }
};

class KnapsackSolver {
public:
    KnapsackSolver(int itemCount, int capacity, const std::vector<int64_t>& weights, const std::vector<int64_t>& values)
        : numItems(itemCount), maxCapacity(capacity), itemWeights(weights), itemValues(values) {
        dpTable.resize(maxCapacity + 1, std::vector<std::vector<ItemNode*>>(2, std::vector<ItemNode*>(numItems + 1, nullptr)));
        baseNode = new ItemNode(0, -1, nullptr);
    }

    ~KnapsackSolver() {
        delete baseNode;
    }

    void solve() {
        initializeDPTable();
        populateDPTable();
        getMaxValueAndItems();
    }

    void printResult() const {
        std::cout << maxValue << "\n";
        if (maxValue != 0) {
            for (int idx : selectedItems) {
                std::cout << idx << " ";
            }
            std::cout << "\n";
        }
    }

private:
    int numItems;
    int maxCapacity;
    std::vector<int64_t> itemWeights;
    std::vector<int64_t> itemValues;
    std::vector<std::vector<std::vector<ItemNode*>>> dpTable;
    ItemNode* baseNode;
    uint64_t maxValue = 0;
    std::vector<int> selectedItems;

    void initializeDPTable() {
        for (int i = 0; i <= maxCapacity; i++) {
            for (int j = 0; j < 2; j++) {
                dpTable[i][j][0] = baseNode;
            }
        }
    }

    void populateDPTable() {
        for (int i = numItems - 1; i >= 0; i--) {
            for (int capacity = 0; capacity <= maxCapacity; capacity++) {
                for (int itemCount = 1; itemCount <= numItems; itemCount++) {
                    if (numItems - i < itemCount) {
                        dpTable[capacity][i % 2][itemCount] = nullptr;
                        continue;
                    }

                    if (itemWeights[i] > capacity) {
                        dpTable[capacity][i % 2][itemCount] = dpTable[capacity][(i + 1) % 2][itemCount];
                        continue;
                    }

                    ItemNode* nodeWithoutItem = dpTable[capacity][(i + 1) % 2][itemCount];
                    ItemNode* nodeWithItem = nullptr;

                    if (itemWeights[i] <= capacity) {
                        ItemNode* prevNode = dpTable[capacity - itemWeights[i]][(i + 1) % 2][itemCount - 1];
                        if (prevNode != nullptr) {
                            nodeWithItem = new ItemNode(prevNode->totalValue + itemValues[i], i + 1, prevNode);
                        }
                    }

                    if (nodeWithoutItem == nullptr) {
                        dpTable[capacity][i % 2][itemCount] = nodeWithItem;
                    } else if (nodeWithItem == nullptr || nodeWithoutItem->totalValue > nodeWithItem->totalValue) {
                        dpTable[capacity][i % 2][itemCount] = nodeWithoutItem;
                    } else {
                        dpTable[capacity][i % 2][itemCount] = nodeWithItem;
                    }
                }
            }
        }
    }

    void getMaxValueAndItems() {
        ItemNode* bestNode = nullptr;

        for (int itemCount = 0; itemCount <= numItems; itemCount++) {
            ItemNode* node = dpTable[maxCapacity][0][itemCount];
            if (node == nullptr) continue;

            uint64_t currentValue = node->totalValue * itemCount;
            if (currentValue >= maxValue) {
                bestNode = node;
                maxValue = currentValue;
            }
        }

        while (bestNode != nullptr && bestNode->itemIndex != -1) {
            selectedItems.push_back(bestNode->itemIndex);
            bestNode = bestNode->previousNode;
        }
    }
};

int main() {
    int numItems, maxCapacity;
    std::cin >> numItems >> maxCapacity;

    std::vector<int64_t> weights(numItems);
    std::vector<int64_t> values(numItems);

    for (int i = 0; i < numItems; i++) {
        std::cin >> weights[i] >> values[i];
    }

    KnapsackSolver solver(numItems, maxCapacity, weights, values);
    solver.solve();
    solver.printResult();

    return 0;
}
