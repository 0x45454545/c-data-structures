#include "compact.c"

usize str_size(str text)
{
    usize size = 0;

    while (*text++) size += 1;

    return size;
}

u8 is_alpha(chr c) {
    return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z');
}

u8 is_not_alpha(chr c) {
    return !is_alpha(c);
}
