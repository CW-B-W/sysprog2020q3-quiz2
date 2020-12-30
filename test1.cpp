#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

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


int main()
{
    char s[26];

    for (int i = 0; i < 26; ++i) {
        s[i] = 'a'+i;
    }
    printf("%d\n", is_ascii(s, 26));

    for (int i = 0; i < 26; ++i) {
        s[i] = 130+i;
    }
    printf("%d\n", is_ascii(s, 26));
    return 0;
}