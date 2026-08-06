#include "compact.c"

typedef struct {

    str   data;
    usize size;
    usize capacity;

} rope;

u8 rope_reserve(rope* self, usize n)
{
    str data = realloc(self->data, self->capacity + n);

    if (data == NULL) return 0;

    self->data = data;
    self->capacity += n;

    return 1;
}

inline u0 rope_clear(rope* self) { self->size = 0; }

u8 copy_rope(rope* dst, rope* src)
{
    if (dst->capacity < src->size)
        if (!rope_reserve(dst, src->size - dst->capacity)) return 0;

    memcpy(dst->data, src->data, src->size);

    return 1;
}

u8 drop_rope(rope* this)
{
    free(this->data);

    *this = (rope) { 0 };

    return 1;
}

u8 rope_append_str(rope* self, str text)
{
    usize size = str_size(text);
    usize available = self->capacity - self->size;

    if (available < size)
        if (!rope_reserve(self, size - available))
            return 0;

    memcpy(self->data + self->size, text, size);

    self->size += size;

    return 1;
}

u8 rope_append_rope(rope* self, rope* other)
{
    usize available = self->capacity - self->size;

    if (available < other->size)
        if (!rope_reserve(self, other->size - available))
            return 0;

    memcpy(self->data + self->size, other->data, other->size);

    self->size += other->size;

    return 1;
}
