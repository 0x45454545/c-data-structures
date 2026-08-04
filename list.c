#ifdef using

#include "compact.c"

#include "list_define_mangle.h"

u8 copy(using* dst, using* src);
u8 drop(using* elm);

typedef struct {

    using* data;
    usize  size;
    usize  capacity;

} list;

u8 reserve(list* self, usize n)
{
    using* new = realloc(self->data, (self->capacity + n) * sizeof (using));

    if (new == NULL) return 0;

    self->data = new;
    self->capacity += n;

    return 1;
}

u8 with_capacity(list* self, usize capacity)
{
    *self = (list) { 0 };

    if (!reserve(self, capacity)) return 0;

    return 1;
}

u8 add_give(list* self, using* element)
{
    if (self->size == self->capacity)
        if (!reserve(self, self->capacity))
            return 0;

    self->data[self->size++] = *element;

    *element = (using) { 0 };

    return 1;
}

u8 add_copy(list* self, using* element)
{
    if (self->size == self->capacity)
        if (!reserve(self, self->capacity))
            return 0;

    if (!copy(self->data + self->size, element))
        return 0;

    self->size += 1;

    return 1;
}

u8 pop(list* self)
{
    if (self->size == 0) return 0;
        
    return drop(self->data + --self->size);
}

u8 pop_take(list* self, using* element)
{
    if (self->size == 0) return 0;

    *element = self->data[--self->size]; // logically inexistent

    return 1;
}

u8 insert_give(list* self, usize at, using* element)
{
    if (at > self->size) return 0;

    if (self->size == self->capacity)
        if (!reserve(self, self->capacity))
            return 0;

    memmove(self->data + at + 1, self->data + at, (self->size - at) * sizeof (using));

    self->data[at] = *element;

    self->size += 1;

    *element = (using) { 0 };

    return 1;
}

u8 insert_copy(list* self, usize at, using* element)
{
    if (at > self->size) return 0;

    if (self->size == self->capacity)
        if (!reserve(self, self->capacity))
            return 0;

    using new = (using) { 0 };

    if (!copy(&new, element)) return 0;

    memmove(self->data + at + 1, self->data + at, (self->size - at) * sizeof (using));

    self->data[at] = new;

    self->size += 1;

    return 1;
}

u8 remove(list* self, usize at)
{
    if (at >= self->size) return 0;

    using element = self->data[at];

    memmove(self->data + at, self->data + at + 1, (self->size - at - 1) * sizeof (using));

    drop(&element);

    self->size -= 1;

    return 1;
}

u8 remove_take(list* self, usize at, using* element)
{
    if (at >= self->size) return 0;

    *element = self->data[at];

    memmove(self->data + at, self->data + at + 1, (self->size - at - 1) * sizeof (using));

    self->size -= 1;

    return 1;
}

#include "list_undef_mangle.h"

#endif
