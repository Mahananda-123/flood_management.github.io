def rabin_karp(text, pattern, prime=101):
    """
    Rabin-Karp algorithm for substring search.
    :param text: The text to search within.
    :param pattern: The pattern to search for.
    :param prime: A prime number used for hashing.
    :return: List of starting indices where the pattern matches the text.
    """
    n = len(text)
    m = len(pattern)
    d = 256  # Number of characters in the input alphabet
    h = 1    # Hash value for pattern
    p = 0    # Hash value for text
    t = 0    # Hash value for text window
    result = []

    # Calculate h = pow(d, m-1) % prime
    for _ in range(m - 1):
        h = (h * d) % prime

    # Calculate initial hash values for pattern and first text window
    for i in range(m):
        p = (d * p + ord(pattern[i])) % prime
        t = (d * t + ord(text[i])) % prime

    # Slide the pattern over the text
    for i in range(n - m + 1):
        # Check the hash values of the pattern and current window
        if p == t:
            # Check characters one by one for confirmation
            if text[i:i + m] == pattern:
                result.append(i)

        # Calculate hash value for next window
        if i < n - m:
            t = (d * (t - ord(text[i]) * h) + ord(text[i + m])) % prime
            if t < 0:
                t += prime

    return result

# Example usage
text = "ababcababcabc"
pattern = "abc"
matches = rabin_karp(text, pattern)
print("Pattern found at indices:", matches)
