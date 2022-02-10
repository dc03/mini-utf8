#include "mini_utf8.h"

#include <stdio.h>

int main() {
    const char *original = " ađở🇺🇸 ";
    printf("original: %s\n", original);
    mini_utf8_str str = from_char_str(original);

    printf("utf-8 encoded:\n");
    for (size_t i = 0; i < str.len; i++) {
        printf("0x%x ", str.arr[i]);
    }
    printf("\n");

    mini_utf8_codepoint_arr as_codepoints = utf8_decode(str);

    printf("utf-8 decoded to code points:\n");
    for (size_t i = 0; i < as_codepoints.len; i++) {
        printf("0x%x ", as_codepoints.arr[i]);
    }
    printf("\n");

    mini_utf8_str as_utf8 = utf8_encode(as_codepoints);
    printf("code points encoded to utf-8:\n");
    for (size_t i = 0; i < as_utf8.len; i++) {
        printf("0x%x ", str.arr[i]);
    }
    printf("\n");

    char *from_utf8 = to_char_str(as_utf8);
    printf("utf-8 converted to char str: ");
    printf("%s\n", from_utf8);

    free(from_utf8);
    utf8_str_destroy(&str);
    utf_codepoint_arr_destroy(&as_codepoints);
    utf8_str_destroy(&as_utf8);
}