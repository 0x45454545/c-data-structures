#include "compact.c"

#define cat(a, b) a ## b
#define mangle(a, b) cat(a, b)

#define list mangle(list_, using)

#define reserve mangle(list, _reserve)

#define copy mangle(copy_, list)
#define drop mangle(drop_, list)

#define add mangle(list, _add)
#define pop mangle(list, _pop)

#define insert mangle(list, _insert)
#define remove mangle(list, _remove)

#define extend mangle(list, _extend)

typedef struct {

    using* data;
    usize  size;
    usize  capacity;

} list;

u8 reserve(list* self, usize n)
{
    using* data = realloc(self->data, (self->capacity + n) * sizeof (using));

    if (data == NULL) return 0;

    self->data = data;
    self->capacity += n;

    return 1;
}

u8 copy(list* dst, list* src)
{
    if (!reserve(dst, src->size)) return 0;

    memcpy(dst->data, src->data, src->size * sizeof (using));

    return 1;
}

u8 drop(list* this)
{
    free(this->data);

    *this = (list) { 0 };

    return 1;
}

u8 add(list* self, using element)
{
    if (self->size == self->capacity)
        if (!reserve(self, self->capacity))
            return 0;

    self->data[self->size++] = element;

    return 1;
}

u8 pop(list* self, using* element)
{
    if (self->size == 0) return 0;

    self->size -= 1;

    if (element) *element = self->data[self->size];

    return 1;
}

u8 insert(list* self, usize at, using element)
{
    if (at > self->size) return 0;

    if (self->size == self->capacity)
        if (!reserve(self, self->capacity))
            return 0;

    memmove(self->data + at + 1, self->data + at, (self->size - at) * sizeof (using));

    self->data[at] = element;

    self->size += 1;

    return 1;
}

u8 remove(list* self, usize at, using* element)
{
    if (at >= self->size) return 0;

    if (element) *element = self->data[at];

    memmove(self->data + at, self->data + at + 1, (self->size - at - 1) * sizeof (using));

    self->size -= 1;

    return 1;
}

u8 extend(list* self, list* other)
{
    usize available = self->capacity - self->size;

    if (available < other->size)
        if (!reserve(self, other->size - available))
            return 0;

    memcpy(self->data + self->size, other->data, other->size * sizeof (using));

    self->size += other->size;

    return 1;
}

#undef extend

#undef remove
#undef insert

#undef pop
#undef add

#undef drop
#undef copy

#undef reserve

#undef list

#undef mangle
#undef cat
