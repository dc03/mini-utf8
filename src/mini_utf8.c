/* Copyright (C) 2020-2022  Dhruv Chawla */
/* See LICENSE at project root for license details */
#include "mini_utf8/mini_utf8.h"

MINI_UTF8_IMPL_ARRAY(utf_codepoint_arr, utf_codepoint_t);
MINI_UTF8_IMPL_ARRAY(utf8_str, utf8_t);

mini_utf8_str mini_utf8_from_char_str(const char *str) {
    size_t len = 0;
    while (str[len]) {
        if (((uint16_t)str[len]) < 0) {
            return utf8_str_make_invalid();
        }
        len++;
    }

    mini_utf8_str utf_str = utf8_str_make(len + 1);
    memcpy(utf_str.arr, str, len + 1);
    utf_str.len = len;
    return utf_str;
}
char *mini_utf8_to_char_str(mini_utf8_str str) {
    char *buf = malloc(sizeof(char) * str.len + 1);
    memcpy(buf, str.arr, str.len);
    buf[str.len] = '\0';
    return buf;
}

enum mini_utf8_head_type mini_utf8_head_type(utf8_t head) {
    if (head >> 7 == MINI_UTF8_SINGLE) {
        return MINI_UTF8_SINGLE;
    } else if (head >> 5 == MINI_UTF8_DOUBLE) {
        return MINI_UTF8_DOUBLE;
    } else if (head >> 4 == MINI_UTF8_TRIPLE) {
        return MINI_UTF8_TRIPLE;
    } else if (head >> 3 == MINI_UTF8_QUAD) {
        return MINI_UTF8_QUAD;
    } else {
        return MINI_UTF8_UNKNOWN;
    }
}

static inline uint8_t next_codepoint_len(const utf8_t *str) {
    utf8_t head = str[0];

    switch (mini_utf8_head_type(head)) {
        case MINI_UTF8_SINGLE: return 1;
        case MINI_UTF8_DOUBLE: return 2;
        case MINI_UTF8_TRIPLE: return 3;
        case MINI_UTF8_QUAD: return 4;
        case MINI_UTF8_UNKNOWN:
        default: return 0;
    }
}

static inline utf_codepoint_t head_data(utf8_t head) {
    switch (mini_utf8_head_type(head)) {
        case MINI_UTF8_SINGLE: return head & MINI_UTF8_SINGLE_DATA;
        case MINI_UTF8_DOUBLE: return head & MINI_UTF8_DOUBLE_DATA;
        case MINI_UTF8_TRIPLE: return head & MINI_UTF8_TRIPLE_DATA;
        case MINI_UTF8_QUAD: return head & MINI_UTF8_QUAD_DATA;
        case MINI_UTF8_UNKNOWN:
        default: return 0;
    }
}

static inline utf_codepoint_t tail_data(utf8_t tail) {
    return (utf_codepoint_t)tail & MINI_UTF8_TAIL_DATA;
}

static inline utf_codepoint_t tail_mask(utf8_t tail, int8_t pos) {
    return ((utf_codepoint_t)tail_data(tail)) << (6 * (pos - 1));
}

utf_codepoint_t mini_utf8_decode_one(const utf8_t *str) {
    utf8_t head = str[0];

    if (head >> 7 == 0) {
        return (utf_codepoint_t)head;
    } else if (head >> 5 == 0b110) {
        utf8_t tail = str[1];

        utf_codepoint_t code_point = (head_data(head) << 6) | tail_mask(tail, 1);
        return code_point;
    } else if (head >> 4 == 0b1110) {
        utf8_t tail1 = str[1];
        utf8_t tail2 = str[2];

        utf_codepoint_t code_point = (head_data(head) << 12) | tail_mask(tail1, 2) | tail_mask(tail2, 1);
        return code_point;
    } else if (head >> 3 == 0b11110) {
        utf8_t tail1 = str[1];
        utf8_t tail2 = str[2];
        utf8_t tail3 = str[3];

        utf_codepoint_t code_point =
            (head_data(head) << 18) | tail_mask(tail1, 3) | tail_mask(tail2, 2) | tail_mask(tail3, 1);
        return code_point;
    } else {
        return 0;
    }
}

mini_utf8_codepoint_arr mini_utf8_decode(mini_utf8_str str) {
    if (str.len == 0) {
        return utf_codepoint_arr_make_invalid();
    }

    mini_utf8_codepoint_arr arr = utf_codepoint_arr_make(8);
    size_t i = 0;
    while (i < str.len) {
        utf_codepoint_t next = mini_utf8_decode_one(str.arr + i);
        utf_codepoint_arr_push(&arr, next);

        uint8_t next_len = next_codepoint_len(str.arr + i);
        if (next_len == 0) {
            utf_codepoint_arr_destroy(&arr);
            return utf_codepoint_arr_make_invalid();
        }
        i += next_len;
    }

    return arr;
}

static inline uint8_t make_head(utf_codepoint_t code_point, enum mini_utf8_head_type head_type) {
    switch (head_type) {
        case MINI_UTF8_SINGLE: return code_point | MINI_UTF8_SINGLE_MASK;
        case MINI_UTF8_DOUBLE: return code_point | MINI_UTF8_DOUBLE_MASK;
        case MINI_UTF8_TRIPLE: return code_point | MINI_UTF8_TRIPLE_MASK;
        case MINI_UTF8_QUAD: return code_point | MINI_UTF8_QUAD_MASK;
        case MINI_UTF8_UNKNOWN:
        default: return 0;
    }
}

static inline utf8_t make_tail(utf_codepoint_t code_point) {
    return (code_point & MINI_UTF8_TAIL_DATA) | MINI_UTF8_TAIL_MASK;
}

uint32_t mini_utf8_encode_one(utf_codepoint_t code_point) {
    // clang-format off
    if (code_point <= 0x7f) {
        return (uint32_t)make_head(code_point, MINI_UTF8_SINGLE);
    } else if (code_point <= 0x7ff) {
        return ((uint32_t)make_tail(code_point) << 8)
             | make_head(code_point >> 6, MINI_UTF8_DOUBLE);
    } else if (code_point <= 0xffff) {
        return ((uint32_t)make_tail(code_point) << 16)
             | ((uint32_t)make_tail(code_point >> 6) << 8)
             | make_head(code_point >> 12, MINI_UTF8_TRIPLE);
    } else if (code_point <= 0x10ffff) {
        return ((uint32_t)make_tail(code_point) << 24)
             | ((uint32_t)make_tail(code_point >> 6) << 16)
             | ((uint32_t)make_tail(code_point >> 12) << 24)
             | make_head(code_point >> 18, MINI_UTF8_QUAD);
    } else {
        return 0;
    }
    // clang-format on
}


mini_utf8_str mini_utf8_encode(mini_utf8_codepoint_arr arr) {
    if (arr.len == 0) {
        return utf8_str_make_invalid();
    }

    mini_utf8_str str = utf8_str_make(8);
    size_t i = 0;
    while (i < arr.len) {
        utf_codepoint_t code_point = arr.arr[i];
        if (code_point <= 0x7f) {
            utf8_t value = make_head(code_point, MINI_UTF8_SINGLE);
            utf8_str_push(&str, value);
        } else if (code_point <= 0x7ff) {
            utf8_t tail = make_tail(code_point);
            utf8_t head = make_head(code_point >> 6, MINI_UTF8_DOUBLE);
            utf8_str_push(&str, head);
            utf8_str_push(&str, tail);
        } else if (code_point <= 0xffff) {
            utf8_t tail2 = make_tail(code_point);
            utf8_t tail1 = make_tail(code_point >> 6);
            utf8_t head = make_head(code_point >> 12, MINI_UTF8_TRIPLE);
            utf8_str_push(&str, head);
            utf8_str_push(&str, tail1);
            utf8_str_push(&str, tail2);
        } else if (code_point <= 0x10ffff) {
            utf8_t tail3 = make_tail(code_point);
            utf8_t tail2 = make_tail(code_point >> 6);
            utf8_t tail1 = make_tail(code_point >> 12);
            utf8_t head = make_head(code_point >> 18, MINI_UTF8_QUAD);
            utf8_str_push(&str, head);
            utf8_str_push(&str, tail1);
            utf8_str_push(&str, tail2);
            utf8_str_push(&str, tail3);
        } else {
            utf8_str_destroy(&str);
            return utf8_str_make_invalid();
        }
        i++;
    }

    return str;
}
