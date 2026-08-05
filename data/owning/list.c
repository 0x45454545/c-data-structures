#include "../compact.c"

#define cat(a, b) a ## b
#define mangle(a, b) cat(a, b)

#define copy        mangle(copy_, using)
#define drop        mangle(drop_, using)

#define list        mangle(list_, using)

#define reserve     mangle(list, _reserve)

#define copy_list   mangle(copy_, list)
#define drop_list   mangle(drop_, list)

#define add_give    mangle(list, _add_give)
#define add_copy    mangle(list, _add_copy)

#define pop         mangle(list, _pop)
#define pop_take    mangle(list, _pop_take)

#define insert_give mangle(list, _insert_give)
#define insert_copy mangle(list, _insert_copy)

#define remove      mangle(list, _remove)
#define remove_take mangle(list, _remove_take)

#define extend_give mangle(list, _extend_give)
#define extend_copy mangle(list, _extend_copy)

u8 copy(using* dst, using* src);
u8 drop(using* elm);

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

u8 copy_list(list* dst, list* src)
{
    if (!reserve(dst, src->size)) return 0;

    for (usize it = 0; it < src->size; ++it)
        if (!copy(dst->data + it, src->data + it))
            return 0;

    return 1;
}

u8 drop_list(list* this)
{
    for (usize it = 0; it < this->size; ++it)
        drop(this->data[it]);

    free(this->data);

    *this = (list) { 0 };

    return 1;
}

u8 add_give(list* self, using* element)
{
    if (self->size == self->capacity)
        if (!reserve(self, self->capacity))
            return 0;

    self->data[self->size] = *element;

    self->size += 1;

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

    drop(self->data + self->size);

    self->size -= 1;

    return 1
}

u8 pop_take(list* self, using* element)
{
    if (self->size == 0) return 0;

    self->size -= 1;

    *element = self->data[self->size];
    
    self->data[self->size] = (using) { 0 };

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

u8 extend_give(list* self, list* other)
{
    usize available = self->capacity - self->size;

    if (available < other->size)
        if (!reserve(self, other->size - available))
            return 0;

    memcpy(self->data + self->size, other->data, other->size * sizeof (using));

    self->size += other->size;

    *other = (using) { 0 };

    return 1;
}

u8 extend_copy(list* self, list* other)
{
    usize available = self->capacity - self->size;

    if (available < other->size)
        if (!reserve(self, other->size - available))
            return 0;

    for (usize it = 0; it < other->size; ++it)
        if (!copy(self->data + self->size + it, other->data + it))
            return 0;

    self->size += other->size;

    return 1;
}

#define extend_copy
#define extend_give

#undef remove_take
#undef remove

#undef insert_copy
#undef insert_give

#undef pop_take
#undef pop

#undef add_copy
#undef add_give

#undef drop_list
#undef copy_list

#undef reserve

#undef list

#undef drop
#undef copy
