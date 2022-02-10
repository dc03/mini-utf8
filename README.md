# mini-utf8

## A small C library for encoding and decoding utf-8 strings

```c
const char *original = "😂";

mini_utf8_str str = from_char_str(original);
for (size_t i = 0; i < str.len; i++) {
        printf("0x%x ", str.arr[i]);
}
printf("\n");

mini_utf8_codepoint_arr as_codepoints = utf8_decode(str);
for (size_t i = 0; i < as_codepoints.len; i++) {
        printf("0x%x ", as_codepoints.arr[i]);
}
printf("\n");

mini_utf8_str as_utf8 = utf8_encode(as_codepoints);
for (size_t i = 0; i < as_utf8.len; i++) {
        printf("0x%x ", str.arr[i]);
}
printf("\n");

char *from_utf8 = to_char_str(as_utf8);
printf("%s\n", from_utf8);
```

Output

```none
0xf0 0x9f 0x98 0x82
0x1f602
0xf0 0x9f 0x98 0x82 
😂
```

## License

This project is licensed under the MIT license.
