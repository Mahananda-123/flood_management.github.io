#include <iostream>
#include <string>
#include <vector>
using namespace std;

vector<int> rabinKarp(const string& text, const string& pattern, int prime = 101) {
    int n = text.size();
    int m = pattern.size();
    int d = 256; // Number of characters in the input alphabet
    int p = 0;   // Hash value for pattern
    int t = 0;   // Hash value for text window
    int h = 1;   // The value of d^(m-1) % prime
    vector<int> result;

    // Calculate h = pow(d, m-1) % prime
    for (int i = 0; i < m - 1; i++) {
        h = (h * d) % prime;
    }

    // Calculate initial hash values for pattern and first window of text
    for (int i = 0; i < m; i++) {
        p = (d * p + pattern[i]) % prime;
        t = (d * t + text[i]) % prime;
    }

    // Slide the pattern over the text
    for (int i = 0; i <= n - m; i++) {
        // Check the hash values of the current window and the pattern
        if (p == t) {
            // Confirm by checking characters one by one
            if (text.substr(i, m) == pattern) {
                result.push_back(i);
            }
        }

        // Calculate the hash value for the next window of text
        if (i < n - m) {
            t = (d * (t - text[i] * h) + text[i + m]) % prime;

            // If t becomes negative, make it positive
            if (t < 0) {
                t += prime;
            }
        }
    }

    return result;
}

int main() {
    string text = "ababcababcabc";
    string pattern = "abc";
    vector<int> matches = rabinKarp(text, pattern);

    cout << "Pattern found at indices: ";
    for (int index : matches) {
        cout << index << " ";
    }
    cout << endl;

    return 0;
}
