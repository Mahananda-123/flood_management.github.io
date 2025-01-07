#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Function to preprocess the pattern to create the LPS (Longest Prefix Suffix) array
void computeLPSArray(const string &pattern, vector<int> &lps) {
    int m = pattern.length();
    lps[0] = 0;  // The LPS value for the first character is always 0
    int j = 0;    // Length of the previous longest prefix suffix

    // Preprocess the pattern
    for (int i = 1; i < m; i++) {
        while (j > 0 && pattern[i] != pattern[j]) {
            j = lps[j - 1];  // Fallback to previous longest prefix suffix
        }
        if (pattern[i] == pattern[j]) {
            j++;
        }
        lps[i] = j;
    }
}

// Function to perform the KMP pattern matching
void KMPSearch(const string &text, const string &pattern) {
    int n = text.length();
    int m = pattern.length();

    // Create the LPS array for the pattern
    vector<int> lps(m, 0);
    computeLPSArray(pattern, lps);

    int i = 0;  // Index for text
    int j = 0;  // Index for pattern

    // Search the pattern in the text
    while (i < n) {
        if (pattern[j] == text[i]) {
            i++;
            j++;
        }

        // If we have found a match
        if (j == m) {
            cout << "Pattern found at index " << i - j << endl;
            j = lps[j - 1];  // Continue searching for next potential match
        }
        // Mismatch after j matches
        else if (i < n && pattern[j] != text[i]) {
            // Don't move i if j is not 0, just shift the pattern
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }
}

int main() {
    string text = "ABABDABACDABABCABAB";
    string pattern = "ABABCABAB";

    KMPSearch(text, pattern);

    return 0;
}
