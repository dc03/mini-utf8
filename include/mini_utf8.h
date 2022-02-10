#pragma once

/* Copyright (C) 2020-2022  Dhruv Chawla */
/* See LICENSE at project root for license details */

#ifndef MINI_UTF8_H
#define MINI_UTF8_H

#include "internal/gen_array.h"

#include <stdint.h>

typedef uint32_t utf_codepoint_t;
typedef uint8_t utf8_t;

MINI_UTF8_GEN_ARRAY(utf_codepoint_arr, utf_codepoint_t);
MINI_UTF8_GEN_ARRAY(utf8_str, utf8_t);

typedef MINI_UTF8_ARRAY(utf_codepoint_arr) mini_utf8_codepoint_arr;
typedef MINI_UTF8_ARRAY(utf8_str) mini_utf8_str;

mini_utf8_str from_char_str(const char *str);
char *to_char_str(mini_utf8_str str);

mini_utf8_codepoint_arr utf8_decode(mini_utf8_str str);
mini_utf8_str utf8_encode(mini_utf8_codepoint_arr arr);

#endif