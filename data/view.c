#include "compact.c"

#define cat(a, b) a ## b
#define mangle(a, b) cat(a, b)

#define view mangle(view_, using)

#define narrowl mangle(view, _narrowl)
#define narrowr mangle(view, _narrowr)

#define dropl mangle(view, _dropl)
#define dropr mangle(view, _dropr)

#define takel mangle(view, _takel)
#define taker mangle(view, _taker)

#define predicate mangle(predicate_, using)

typedef u8 (*predicate)(using);

typedef struct {

    using* data;
    usize  size;

} view;

u0 narrowl(view* self, usize n)
{
    if (self->size < n) n = self->size;

    self->data += n;
    self->size -= n;
}

u0 narrowr(view* self, usize n)
{
    if (self->size < n) n = self->size;

    self->size -= n;
}

u0 dropl(view* self, predicate fn)
{
    while (self->size && fn(*self->data)) {
        self->data += 1;
        self->size -= 1;
    }
}

u0 dropr(view* self, predicate fn)
{
    while (self->size && fn(self->data[self->size - 1]))
        self->size -= 1;
}

view takel(view* self, predicate fn)
{
    usize taken = 0;

    while (self->size && fn(self->data[taken]))
        taken += 1;

    view left = (view) { .data = NULL, .size = taken };

    if (taken)
        left.data = self->data;

    self->data += taken;
    self->size -= taken;

    return left;
}

view taker(view* self, predicate fn)
{
    usize taken = 0;

    while (self->size && fn(self->data[self->size - taken - 1]))
        taken += 1;

    view right = (view) { .data = NULL, .size = taken };

    if (taken)
        right.data = self->data + self->size - taken;

    self->size -= taken;

    return right;
}

#undef predicate

#undef taker
#undef takel

#undef dropr
#undef dropl

#undef narrowr
#undef narrowl

#undef view

#undef mangle
#undef cat
