#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <vector>
#include <queue>
#include <tuple>

namespace FindSubstrings {
    using Word = unsigned int;
    using String = std::vector<Word>;

    std::vector<size_t> ZFunction(const String& str) {
        size_t length = str.size();
        std::vector<size_t> z(length, 0);
        for (size_t i = 1, left = 0, right = 0; i < length; ++i) {
            if (i <= right) {
                z[i] = std::min(z[i - left], right - i + 1);
            }
            while (i + z[i] < length && str[i + z[i]] == str[z[i]]) {
                ++z[i];
            }
            if (i + z[i] - 1 > right) {
                left = i;
                right = i + z[i] - 1;
            }
        }
        return z;
    }

    std::vector<size_t> PreprocessZ(const String& str) {
        auto zFunctionValues = ZFunction(str);
        size_t length = zFunctionValues.size();
        std::vector<size_t> strongSuffixPrefix(length, 0);
        for (size_t j = length - 1; j > 0; --j) {
            size_t i = j + zFunctionValues[j] - 1;
            strongSuffixPrefix[i] = zFunctionValues[j];
        }
        return strongSuffixPrefix;
    }

    void UpdatePosition(std::pair<size_t, size_t>& position, char delimiter) {
        if (delimiter == ' ') {
            ++(position.second);
        } else {
            ++(position.first);
            position.second = 0;
        }
        return;
    }

    void UpdatePosition(std::istream& inputStream, std::pair<size_t, size_t>& position) {
        char delimiter;
        while (inputStream.get(delimiter)) {
            if ((delimiter >= '0') && (delimiter <= '9')) {
                inputStream.unget();
                break;
            }
            if (delimiter == '\n') {
                UpdatePosition(position, delimiter);
            }
        }
        UpdatePosition(position, ' ');
    }

    std::queue<std::pair<size_t, size_t>> FindSubstringsPositions(const String& pattern, std::istream& inputStream) {
        std::queue<std::pair<size_t, size_t>> positions;
        if (pattern.size() == 0) return positions;
        auto patternPreprocessing = PreprocessZ(pattern);
        std::istream_iterator<Word> eos;
        size_t patternIndex = 0;
        std::pair<size_t, size_t> currentPosition(1, 0);
        UpdatePosition(inputStream, currentPosition);
        std::queue<std::pair<size_t, size_t>> entry;
        std::istream_iterator<Word> inputIterator(inputStream);
        while (inputIterator != eos) {
            while (patternIndex < pattern.size() && (pattern[patternIndex] == *inputIterator)) {
                ++patternIndex;
                entry.push(currentPosition);
                if (entry.size() > pattern.size()) entry.pop();
                UpdatePosition(inputStream, currentPosition);
                ++inputIterator;
            }
            if (patternIndex == pattern.size()) positions.push(entry.front());
            if (patternIndex == 0) {
                entry.push(currentPosition);
                if (entry.size() > pattern.size()) entry.pop();
                UpdatePosition(inputStream, currentPosition);
                ++inputIterator;
            }
            patternIndex = (patternIndex == 0) ? 0 : (patternPreprocessing[patternIndex - 1]);
        }
        return positions;
    }
}

int main() {
    using namespace FindSubstrings;
    std::string patternString;
    std::getline(std::cin, patternString);
    std::stringstream stream(patternString);
    String pattern;
    Word word;
    while (stream >> word) {
        pattern.push_back(word);
    }
    std::queue<std::pair<size_t, size_t>> positions = FindSubstringsPositions(pattern, std::cin);
    while (!positions.empty()) {
        auto position = positions.front();
        positions.pop();
        std::cout << position.first << ", " << position.second << '\n';
    }
}
