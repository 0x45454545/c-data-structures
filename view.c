#pragma once

#include "compact.c"

#define vfmt   "%.*s"
#define sfmt "\"%.*s\""
#define show_view(self) (self).size, (self).data

typedef u8 (*ischr) (chr);

typedef struct {

    str   data;
    usize size;

} view;


u8 view_empty(view* this)
{
    return this->size == 0;
}

u8 view_equal(view* this, view* that)
{
    if (this->size != that->size) return 0;

    if (this->data == that->data) return 1;

    for (usize i = 0; i < this->size; ++i)
        if (this->data[i] != that->data[i])
            return 0;

    return 1;
}

view view_of(str data, usize size)
{
    return (view) { .data = data, .size = size };
}

u0 view_narrowl(view* self, usize n)
{
    if (n > self->size) n = self->size;

    self->data += n;
    self->size -= n;
}

u0 view_narrowr(view* self, usize n)
{
    if (n > self->size) n = self->size;

    self->size -= n;
}

u0 view_dropl(view* self, ischr fn)
{
    while (self->size > 0 && fn(*self->data)) {
        self->data += 1;
        self->size -= 1;
    }
}

u0 view_dropr(view* self, ischr fn)
{
    while (self->size > 0 && fn(self->data[self->size - 1])) {
        self->size -= 1;
    }
}

view view_takel(view* self, ischr fn) 
{
    usize position = 0;

    while (position < self->size && fn(self->data[position])) { 
        position += 1;
    }

    view left = { .data = self->data, .size = position };

    view_narrowl(self, position);

    return left;
}

view view_taker(view* self, ischr fn) 
{
    isize position = self->size - 1;

    while (position >= 0 && fn(self->data[position])) { 
        position -= 1;
    }

    view right = { .data = self->data + position + 1, .size = self->size - position - 1 };

    view_narrowr(self, right.size);

    return right;
}
