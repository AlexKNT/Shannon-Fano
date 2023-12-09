#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <map>

/**
 * @brief Node structure representing a character, its frequency, and its code.
 */
struct Node {
    char character;
    float freq;
    std::string code;
};

/**
 * @brief Calculate the frequency of each character in the input string.
 *
 * @param nodes Vector of Node structures to store character frequencies.
 * @param input Input string for frequency calculation.
 */
void calculateFrequency(std::vector<Node>& nodes, const std::string& input) {
    std::map<char, int> frequencyMap;
    for (char c : input) {
        frequencyMap[c]++;
    }
    for (auto& pair : frequencyMap) {
        Node node;
        node.character = pair.first;
        node.freq = static_cast<float>(pair.second) / input.length();
        nodes.push_back(node);
    }
}

/**
 * @brief Sort the nodes in descending order based on character frequencies.
 *
 * @param nodes Vector of Node structures to be sorted.
 */
void sortByFrequency(std::vector<Node>& nodes) {
    std::sort(nodes.begin(), nodes.end(), [](const Node& a, const Node& b) {
        return a.freq > b.freq;
        });
}

/**
 * @brief Perform Shannon-Fano encoding on the given nodes.
 *
 * @param nodes Vector of Node structures to be encoded.
 * @param start Index of the start of the current node range.
 * @param end Index of the end of the current node range.
 */
void shannonFano(std::vector<Node>& nodes, int start, int end) {
    if (start >= end) return;

    float total = 0.0;
    for (int i = start; i <= end; ++i) {
        total += nodes[i].freq;
    }

    float sum = 0.0;
    int split;
    for (split = start; split <= end; ++split) {
        sum += nodes[split].freq;
        if (sum >= total / 2) break;
    }

    std::cout << "Split range [" << start << ", " << end << "] at index " << split << "\n";

    for (int i = start; i <= split; ++i) {
        nodes[i].code += "0"; 
        std::cout << "Assign code '0' to character '" << nodes[i].character << "'\n";
    }

    for (int i = split + 1; i <= end; ++i) { 
        nodes[i].code += "1"; 
        std::cout << "Assign code '1' to character '" << nodes[i].character << "'\n"; 
    }


    shannonFano(nodes, start, split);
    shannonFano(nodes, split + 1, end);
}

/**
 * @brief Encode the input string using the provided nodes and their codes.
 *
 * @param nodes Vector of Node structures with assigned codes.
 * @param input Input string to be encoded.
 * @return Encoded string.
 */
std::string encode(const std::vector<Node>& nodes, const std::string& input) {
    std::string encoded;
    std::map<char, std::string> lookup;
    for (const auto& node : nodes) {
        lookup[node.character] = node.code;
    }
    for (char c : input) {
        encoded += lookup[c];
        std::cout << "Character '" << c << "' is encoded as '" << lookup[c] << "'\n";
    }
    return encoded;
}

/**
 * @brief Decode the encoded string using the provided nodes and their codes.
 *
 * @param nodes Vector of Node structures with assigned codes.
 * @param encoded Encoded string to be decoded.
 * @return Decoded string.
 */
std::string decode(const std::vector<Node>& nodes, const std::string& encoded) {
    std::string decoded;
    std::string currentCode;
    std::map<std::string, char> lookup;
    for (const auto& node : nodes) {
        lookup[node.code] = node.character;
    }

    std::cout << "\nDecoding Process:\n";
    for (char c : encoded) {
        currentCode += c;
        if (lookup.find(currentCode) != lookup.end()) {
            std::cout << "Code '" << currentCode << "' is decoded as character '" << lookup[currentCode] << "'\n";
            decoded += lookup[currentCode];
            currentCode.clear();
        }
    }
    return decoded;
}

/**
 * @brief Entry point of the program.
 *
 * Reads the input string, performs Shannon-Fano encoding, and prints results.
 */
int main() {
    std::string input;
    std::cout << "Enter the string to encode: ";
    std::getline(std::cin, input);

    std::vector<Node> nodes;
    calculateFrequency(nodes, input);
    sortByFrequency(nodes);

    std::cout << "\nCharacter frequencies:\n";
    for (const auto& node : nodes) {
        std::cout << "Character: " << node.character << " Frequency: " << node.freq << "\n";
    }

    shannonFano(nodes, 0, nodes.size() - 1);

    std::cout << "\nCharacter codes:\n";
    for (const auto& node : nodes) {
        std::cout << "Character: " << node.character << " Code: " << node.code << "\n";
    }

    std::string encoded = encode(nodes, input);
    std::cout << "\nEncoded string: " << encoded << "\n";

    std::string decoded = decode(nodes, encoded);
    std::cout << "Decoded string: " << decoded << "\n";

    return 0;
}



/*

    Calculate Frequency: The calculateFrequency function takes the input string and calculates the frequency of each character. It creates a Node for each unique character and calculates its frequency by dividing the number of occurrences by the total length of the input string.

    Sort by Frequency: The sortByFrequency function sorts the nodes in descending order of frequency. This helps in building the Shannon-Fano tree later.

    Shannon-Fano Encoding: The shannonFano function is a recursive function that splits the nodes into two parts such that the sum of frequencies is as equal as possible. It assigns '0' to the first part and '1' to the second part, and then recurses on both halves until each character has a unique code.

    Encode: The encode function creates a lookup table from characters to their codes and then encodes the input string by replacing each character with its corresponding code.

    Decode: The decode function creates a lookup table from codes to characters and then decodes the encoded string by mapping each code back to its character until the whole string is decoded.

    Main Function: The main function is the entry point of the program. It reads the input string from the user, performs the Shannon-Fano encoding, and then prints the encoded and decoded strings along with the codes for each character.

*/