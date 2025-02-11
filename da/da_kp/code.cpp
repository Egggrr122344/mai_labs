#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

struct Triplet {
    int offset = 0;  
    int len = 0;      
    char next_char;   
};


void print_encoded(const vector<Triplet>& encoded) {
    for (const auto& t : encoded) {
        cout << t.offset << " " << t.len;
        if (t.len == 0 || t.offset > 0) {
            if (t.next_char != '\0' && isalpha(t.next_char)) {
                cout << " " << t.next_char;
            }
        }
        cout << endl;
    }
}

vector<Triplet> encode_lz77(const string& data) {
    vector<Triplet> encoded;
    int pos = 0;

    while (pos < data.size()) {
        Triplet t;
        int max_len = 0;
        int best_offset = 0;

        for (int offset = 1; offset <= pos; ++offset) {
            int length = 0;
            while (pos + length < data.size() && data[pos - offset + length] == data[pos + length]) {
                ++length;
            }

            if (length > max_len) {
                max_len = length;
                best_offset = offset;
            }
        }

        if (max_len > 0) {
            t.offset = best_offset;
            t.len = max_len;
            t.next_char = (pos + max_len < data.size()) ? data[pos + max_len] : '\0';
        } else {
            t.offset = 0;
            t.len = 0;
            t.next_char = data[pos];
        }

        encoded.push_back(t);
        pos += max_len + 1;
    }

    return encoded;
}

string decode_lz77(const vector<Triplet>& encoded) {
    string text;

    for (const auto& code : encoded) {
        int startPos = text.length() - code.offset;
        for (int i = 0; i < code.len; ++i) {
            text += text[startPos + i];
        }
        if (code.next_char != '\0') {
            text += code.next_char;
        }
    }

    return text;
}


vector<Triplet> parse_encoded(const string& input) {
    vector<Triplet> encoded;
    stringstream ss(input);
    Triplet t;

    while (ss >> t.offset >> t.len) {
        if (ss >> t.next_char) {
            encoded.push_back(t);
        } else {
            t.next_char = '\0';
            encoded.push_back(t);
        }
    }

    return encoded;
}

int main() {
    string mode;
    getline(cin, mode);

    if (mode == "compress") {
        string data;
        getline(cin, data);

        vector<Triplet> encoded = encode_lz77(data);
        print_encoded(encoded);
    } else if (mode == "decompress") {
        string encoded_data, line;

        while (getline(cin, line)) {
            encoded_data += line + "\n";
        }

        vector<Triplet> parsed = parse_encoded(encoded_data);
        string decoded = decode_lz77(parsed);

        cout << decoded << endl;
    }

    return 0;
}
