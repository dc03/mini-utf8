#pragma once

/* Copyright (C) 2020-2022  Dhruv Chawla */
/* See LICENSE at project root for license details */

#ifndef MINI_UTF8_H
#define MINI_UTF8_H

#include "internal/bitmasks.h"
#include "internal/gen_array.h"

#include <stdint.h>

typedef uint32_t utf_codepoint_t;
typedef uint8_t utf8_t;

MINI_UTF8_GEN_ARRAY(utf_codepoint_arr, utf_codepoint_t);
MINI_UTF8_GEN_ARRAY(utf8_str, utf8_t);

typedef MINI_UTF8_ARRAY(utf_codepoint_arr) mini_utf8_codepoint_arr;
typedef MINI_UTF8_ARRAY(utf8_str) mini_utf8_str;

enum mini_utf8_head_type {
    MINI_UTF8_SINGLE = 0,
    MINI_UTF8_DOUBLE = 0b110,
    MINI_UTF8_TRIPLE = 0b1110,
    MINI_UTF8_QUAD = 0b11110,
    MINI_UTF8_UNKNOWN
};

enum mini_utf8_encoded_masks {
    MINI_UTF8_ENC_HEAD_MASK = 0b11111111,
    MINI_UTF8_ENC_TAIL1_MASK = 0b1111111100000000,
    MINI_UTF8_ENC_TAIL2_MASK = 0b111111110000000000000000,
    MINI_UTF8_ENC_TAIL3_MASK = 0b11111111000000000000000000000000,
};

mini_utf8_str mini_utf8_from_char_str(const char *str);
char *mini_utf8_to_char_str(mini_utf8_str str);

mini_utf8_codepoint_arr mini_utf8_decode(mini_utf8_str str);
utf_codepoint_t mini_utf8_decode_one(const utf8_t *str);
mini_utf8_str mini_utf8_encode(mini_utf8_codepoint_arr arr);
uint32_t mini_utf8_encode_one(utf_codepoint_t code_point);

enum mini_utf8_head_type mini_utf8_head_type(utf8_t head);

#endif