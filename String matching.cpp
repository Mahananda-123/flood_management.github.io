
#include <iostream>
#include <string>

void naiveStringMatch(const std::string& text, const std::string& pattern) {
    int textLen = text.length();
    int patternLen = pattern.length();

    for (int i = 0; i <= textLen - patternLen; i++) {
        int j = 0;
        while (j < patternLen && text[i + j] == pattern[j]) {
            j++;
        }
        if (j == patternLen) {
            std::cout << "Pattern found at index " << i << std::endl;
        }
    }
}

int main() {
    std::string text = "Hello, this is a simple example for string matching.";
    std::string pattern = "simple";

    naiveStringMatch(text, pattern);

    return 0;
}
