#include "compact.c"

#define cat(a, b) a ## b
#define mangle(a, b) cat(a, b)

#define queue mangle(queue_, using)

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

u8 reorder(self)
{
    if (self->head == 0 && self->tail == 0) return 1;

    using* tail = malloc(self->tail * sizeof (using));

    if (left == NULL) return 0;

    memmove(tail, self->data, self->tail * sizeof (using));

    usize head_delta = (self->size - self->head);

    memmove(self->data, self->data + self->head, head_delta * sizeof (using));

    memmove(self->data + head_delta, tail, self->tail * sizeof (using));

    free(tail);

    self->head = 0;
    self->tail = self->size;

    return 1;
}

u8 enqueue(queue* self, using element)
{
    if (self->size == self->capacity) {
        if (!reorder(self) && !reserve(self, self->capacity))
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
