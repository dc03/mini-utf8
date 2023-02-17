#pragma once

/* Copyright (C) 2020-2022  Dhruv Chawla */
/* See LICENSE at project root for license details */

#ifndef MINI_UTF8_GEN_ARRAY_H
#define MINI_UTF8_GEN_ARRAY_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define MINI_UTF8_ARRAY(NAME) struct MINI_UTF8_##NAME

#define MINI_UTF8_GEN_ARRAY(NAME, T)                                                                                   \
    struct MINI_UTF8_##NAME {                                                                                          \
        T *arr;                                                                                                        \
        size_t len;                                                                                                    \
        size_t capacity;                                                                                               \
    };                                                                                                                 \
                                                                                                                       \
    MINI_UTF8_ARRAY(NAME) NAME##_make(size_t initial_len);                                                             \
    MINI_UTF8_ARRAY(NAME) NAME##_make_invalid();                                                                       \
    void NAME##_destroy(MINI_UTF8_ARRAY(NAME) * arr);                                                                  \
    void NAME##_double_resize(MINI_UTF8_ARRAY(NAME) * arr);                                                            \
    void NAME##_push(MINI_UTF8_ARRAY(NAME) * arr, T value);

#define MINI_UTF8_IMPL_ARRAY(NAME, T)                                                                                  \
    MINI_UTF8_ARRAY(NAME) NAME##_make(size_t initial_len) {                                                            \
        MINI_UTF8_ARRAY(NAME) new_array;                                                                               \
        new_array.arr = (T *)malloc(sizeof(T) * initial_len);                                                          \
        new_array.len = 0;                                                                                             \
        new_array.capacity = initial_len;                                                                              \
        return new_array;                                                                                              \
    }                                                                                                                  \
                                                                                                                       \
    MINI_UTF8_ARRAY(NAME) NAME##_make_invalid() {                                                                      \
        return (MINI_UTF8_ARRAY(NAME)){.arr = NULL, .len = 0, .capacity = 0};                                          \
    }                                                                                                                  \
                                                                                                                       \
    void NAME##_destroy(MINI_UTF8_ARRAY(NAME) * arr) { free(arr->arr); }                                               \
                                                                                                                       \
    void NAME##_double_resize(MINI_UTF8_ARRAY(NAME) * arr) {                                                           \
        MINI_UTF8_ARRAY(NAME) new_array = NAME##_make(arr->capacity * 2);                                              \
        memcpy(new_array.arr, arr->arr, arr->len);                                                                     \
        NAME##_destroy(arr);                                                                                           \
        arr->arr = new_array.arr;                                                                                      \
        arr->capacity = new_array.capacity;                                                                            \
    }                                                                                                                  \
                                                                                                                       \
    void NAME##_push(MINI_UTF8_ARRAY(NAME) * arr, T value) {                                                           \
        if (arr->len >= arr->capacity) {                                                                               \
            NAME##_double_resize(arr);                                                                                 \
        }                                                                                                              \
        arr->arr[arr->len++] = value;                                                                                  \
    }

#endif