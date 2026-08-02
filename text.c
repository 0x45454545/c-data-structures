#pragma once

#include "compact.c"

u8 is_space(chr c) { return c == ' '; }
u8 is_alpha(chr c) {
    return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z');
}

u8 is_not_space(chr c) { return !is_space(c); }
u8 is_not_alpha(chr c) { return !is_alpha(c); }

usize str_size(str data)
{
    usize size = 0;

    while (*data++) { ++size; }

    return size;
}
