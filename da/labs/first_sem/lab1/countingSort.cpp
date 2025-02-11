#include <iostream>
#include <vector>
#include <cstdint>
#include <utility>
#include <iomanip> // Для std::setw и std::setfill

using TPair = std::pair<uint32_t, uint64_t>;
const uint32_t KMaxValue = 999999;

void CountingSort(std::vector<TPair>& amount) {
    std::vector<uint64_t> keyCount(KMaxValue + 1, 0);

    for (size_t i = 0; i < amount.size(); ++i) {
        keyCount[amount[i].first]++;
    }

    for (size_t i = 1; i < keyCount.size(); ++i) {
        keyCount[i] += keyCount[i - 1];
    }

    std::vector<TPair> sortedArray(amount.size());

    for (int i = amount.size() - 1; i >= 0; --i) {
        uint32_t index = --keyCount[amount[i].first]; // позиция элемента 
        sortedArray[index] = amount[i];
    }
    std::swap(amount, sortedArray);
}

void PrintSort(const std::vector<TPair>& amount) {
    for (size_t i = 0; i < amount.size(); ++i) {
        std::cout << std::setw(6) << std::setfill('0') << amount[i].first << '\t' << amount[i].second << '\n';
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);

    std::vector<TPair> elements;
    elements.reserve(static_cast<size_t>(10e7));

    uint32_t key;
    uint64_t value;

    while (std::cin >> key >> value) {
        elements.emplace_back(key, value);
    }

    CountingSort(elements);
    PrintSort(elements);

    return 0;
}
