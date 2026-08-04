#define cat(a, b) a ## b

#define mangle(a, b) cat(a, b)

#define copy mangle(copy_, using)
#define drop mangle(drop_, using)

#define list mangle(list_, using)

#define with_capacity mangle(list, _with_capacity)
#define reserve mangle(list, _reserve)

#define add_give mangle(list, _add_give)
#define add_copy mangle(list, _add_copy)

#define pop mangle(list, _pop)
#define pop_take mangle(list, _pop_take)

#define insert_give mangle(list, _insert_give)
#define insert_copy mangle(list, _insert_copy)

#define remove mangle(list, _remove)
#define remove_take mangle(list, _remove_take)
