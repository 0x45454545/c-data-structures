#include "compact.c"

#define cat(a, b) a ## b
#define mangle(a, b) cat(a, b)

#define queue mangle(queue_, using)

#define reserve mangle(queue, _reserve)

#define copy mangle(copy_, queue)
#define drop mangle(drop_, queue)

#define wraps mangle(queue, _wraps)

#define reorder mangle(queue, _reorder)

#define enqueue mangle(queue, _enqueue)
#define dequeue mangle(queue, _dequeue)

typedef struct {
    using* data;
    usize  size;
    usize  capacity;
    usize  head;
    usize  tail;
} queue;

u8 reserve(queue* self, usize n)
{
    using* data = realloc(self->data, (self->capacity + n) * sizeof (using));

    if (data == NULL) return 0;

    self->data = data;
    self->capacity += n;

    return 1;
}

u8 copy(queue* dst, queue* src)
{
    if (!reserve(dst, src->capacity)) return 0; // TODO: reserve only src->size.

    memcpy(dst->data, src->data, src->capacity * sizeof (using));

    dst->size = src->size;
    dst->head = src->head;
    dst->tail = src->tail;

    return 1;
}

u8 drop(queue* this)
{
    free(this->data);

    *this = (queue) { 0 };

    return 1;
}

u8 wraps(queue* this) { return this->size && this->tail <= this->head; }

u8 reorder(queue* self)
{
    if (!wraps(self)) return 1;

    usize left_size = self->tail * sizeof (using);

    using* left = malloc(left_size);

    if (left == NULL) return 0;

    memmove(left, self->data, left_size);

    usize delta = self->size - self->head;

    memmove(self->data, self->data + self->head, delta * sizeof (using));

    memmove(self->data + delta, left, left_size);

    free(left);

    self->head = 0;
    self->tail = self->size;

    return 1;
}

u8 enqueue(queue* self, using element)
{
    if (self->size == self->capacity) {
        if (!reserve(self, self->capacity) || !reorder(self))
            return 0;
    }

    self->data[self->tail] = element;

    self->size += 1;
    self->tail = (self->tail + 1) % self->capacity;

    return 1;
}

u8 dequeue(queue* self, using* element)
{
    if (self->size == 0) return 0;

    *element = self->data[self->head];

    self->size -= 1;
    self->head = (self->head + 1) % self->capacity;

    return 1;
}

#undef dequeue
#undef enqueue

#undef reorder

#undef wraps

#undef drop
#undef copy

#undef reserve

#undef queue

#undef mangle
#undef cat
