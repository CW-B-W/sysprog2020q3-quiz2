#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

bool is_ascii_naive(const char str[], size_t size)
{
    if (size == 0)
        return false;
    int i = 0;
    for (i = 0; i < size; ++i) {
        if (str[i] & 0x80)
            return false;
    }
    return true;
}

bool is_ascii(const char str[], size_t size)
{
    if (size == 0)
        return false;
    int i = 0;
    while ((i + 8) <= size) {
        uint64_t payload;
        memcpy(&payload, str + i, 8);
        if (payload & 0x8080808080808080)
            return false;
        i += 8;
    }
    while (i < size) {
        if (str[i] & 0x80)
            return false;
        i++;
    }
    return true;
}

inline int isAlphabetChar(char c)
{
    return 
        (('A' <= c) && (c <= 'Z')) ||
        (('a' <= c) && (c <= 'z')) ;
}

int isAlphabetString_naive(const char* s, size_t n)
{
    for (int i = 0; i < n; ++i)
        if (!isAlphabetChar(s[i]))
            return false;
    return true;
}

/*
 * return whether s only contains english alphabets, i.e., 'a'~'z' or 'A'~'Z'
 */
int isAlphabetString(const char *s, size_t n)
{
#define PACKED_BYTE(b) (((uint64_t)(b) & (0xff)) * 0x0101010101010101u)    
    size_t k = n / 8;
    for (size_t i = 0; i < k; i++, s += 8) {
        uint64_t *chunk = (uint64_t *) s;
        if ((*chunk & PACKED_BYTE(0x80)) == 0) { /* is ASCII? */
            uint64_t A = *chunk + PACKED_BYTE(128 - ('A' + 0)); 
            uint64_t Z = *chunk + PACKED_BYTE(128 - ('Z' + 1));
            uint64_t a = *chunk + PACKED_BYTE(128 - ('a' + 0)); 
            uint64_t z = *chunk + PACKED_BYTE(128 - ('z' + 1));
            uint64_t isAlpha = (((A ^ Z) | (a ^ z)) & PACKED_BYTE(0x80));
            if (isAlpha != 0x8080808080808080) {
                return 0;
            }
        } else {
            return 0;
        }
    }
    
    k = n % 8;
    if (k)
        while (k--) {
            if (!isAlphabetChar(*s))
                return 0;
            s++;
        }
    return 1;
#undef PACKED_BYTE
}

int main()
{
    char s[26];

    for (int i = 0; i < 26; ++i) {
        s[i] = 'a'+i;
    }
    assert(true == is_ascii(s, 26));
    assert(true == is_ascii_naive(s, 26));

    for (int i = 0; i < 26; ++i) {
        s[i] = 130+i;
    }
    assert(false == is_ascii(s, 26));
    assert(false == is_ascii_naive(s, 26));

    for (int i = 0; i < 26; ++i) {
        s[i] = '0'+i;
    }
    assert(false == isAlphabetString(s, 26));
    assert(false == isAlphabetString_naive(s, 26));

    for (int i = 0; i < 26; ++i) {
        s[i] = 'a'+i;
    }
    assert(true == isAlphabetString(s, 26));
    assert(true == isAlphabetString_naive(s, 26));

    for (int i = 0; i < 26; ++i) {
        s[i] = 'A'+i;
    }
    assert(true == isAlphabetString(s, 26));
    assert(true == isAlphabetString_naive(s, 26));
    return 0;
}