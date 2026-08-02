#ifdef using

#include "compact.c"

#include "list_define_mangle.h"

// base

typedef struct {

    using* data;
    usize  size;
    usize  tail;

} list;

u8 res(list* self, usize size)
{
    using* new = realloc(self->data, size * sizeof (using));

    if (size != 0 && new == NULL) return 0;

    self->data = new;
    self->tail = size;

    return 1;
}

u8 fit(list* self) { return res(self, self->size); }

u0 end(list* self) { free(self->data); *self = (list) { 0 }; }

u0 clr(list* self) { self->size = 0; }

u0 fil(list* self, using element)
{
    for (usize i = 0; i < self->size; ++i)
        self->data[i] = element;
}

u8 add(list* self, using element)
{
    if (self->size >= self->tail)
        if (!res(self, self->tail ? self->tail * 2 : 1))
            return 0;

    self->data[self->size++] = element;

    return 1;
}

u8 pop(list* self, /*optional out*/ using* element)
{
    if (self->size == 0) return 0;

    --self->size;

    if (element != NULL) *element = self->data[self->size];

    if (self->size < self->tail / 4) fit(self);

    return 1;
}

u8 get(list* this, usize index, /*out*/ using* element)
{
    if (index >= this->size) return 0;

    *element = this->data[index];

    return 1;
}

u8 set(list* self, usize index, using element)
{
    if (index >= self->size) return 0;

    self->data[index] = element;

    return 1;
}

u8 ins(list* self, usize index, using element)
{
    if (index > self->size) return 0;

    if (self->size >= self->tail)
        if (!res(self, self->tail ? self->tail * 2 : 1))
            return 0;

    memmove(self->data + index + 1,
            self->data + index,
            (self->size - index) * sizeof (using));

    ++self->size;

    self->data[index] = element; 

    return 1;
}

u8 rem(list* self, usize index, /*optional out*/ using* element)
{
    if (index >= self->size) return 0;

    if (element != NULL) *element = self->data[index];

    memmove(self->data + index, 
            self->data + index + 1, 
            (self->size - index - 1) * sizeof (using));

    --self->size;

    if (self->size < self->tail / 4) fit(self);

    return 1;
}

u8 ext(list* self, list* other)
{
    if (other->size > self->tail - self->size)
        if (!res(self, self->size + other->size))
            return 0;

    memmove(self->data + self->size, other->data, other->size * sizeof (using));

    self->size += other->size;

    return 1;
}

u8 cat(list* this, list* that, /*out*/ list* new)
{
    if (new == this) return ext(this, that);

    if (new == that) return 0; // todo: impl

    usize sum = this->size + that->size;

    if (!res(new, sum)) return 0;

    memcpy(new->data             , this->data, this->size * sizeof (using));
    memcpy(new->data + this->size, that->data, that->size * sizeof (using));

    new->size = sum;

    return 1;
}

u8 cpy(list* this, /*out*/ list* new)
{
    if (new == this) return 1;

    if (!res(new, this->size)) return 0;

    memcpy(new->data, this->data, this->size * sizeof (using));

    new->size = this->size;

    return 1;
}

// extra

u8 empty(list* this) { return this->size == 0; }

u8 equal(list* this, list* that, compare fn)
{
    if (this->size != that->size) return 0;

    if (this->data == that->data) return 1;

    for (usize i = 0; i < this->size; ++i)
        if (!fn(this->data[i], that->data[i]))
            return 0;

    return 1;
}

u8 each(list* self, endo fn)
{
    for (usize i = 0; i < self->size; ++i)
        self->data[i] = fn(self->data[i]);

    return 1;
}

using foldl(list* this, using accumulator, operator fn)
{
    for (usize i = 0; i < this->size; ++i)
        accumulator = fn(accumulator, this->data[i]);

    return accumulator;
}

using foldr(list* this, using accumulator, operator fn)
{
    for (usize i = this->size; i-- > 0;)
        accumulator = fn(this->data[i], accumulator);

    return accumulator;
}

u8 map(list* this, endo fn, /*out*/ list* new)
{
    if (new == this) return each(this, fn);

    if (!res(new, this->size)) return 0;

    for (usize i = 0; i < this->size; ++i)
        new->data[i] = fn(this->data[i]);

    new->size = this->size;

    return 1;
}

u8 any(list* this, compare fn, using element)
{
    for (usize i = 0; i < this->size; ++i)
        if (fn(this->data[i], element))
            return 1;

    return 0;
}

u8 keep(list* self, predicate fn)
{
    usize kept = 0;

    for (usize i = 0; i < self->size; ++i)
        if (fn(self->data[i]))
            self->data[kept++] = self->data[i];

    self->size = kept;

    fit(self);

    return 1;
}

u8 filter(list* this, predicate fn, list* new)
{
    if (new == this || !res(new, this->size)) return 0;

    new->size = 0;

    for (usize i = 0; i < this->size; ++i)
        if (fn(this->data[i]))
            if (!add(new, this->data[i]))
                return 0;

    return 1;
}

#include "list_undef_mangle.h"

#endif
