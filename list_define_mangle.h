#define ___cat(prefix, suffix) prefix ## suffix

#define mangle(prefix, suffix) ___cat(prefix, suffix)

// base

#define list mangle(list_, using)

#define lst mangle(lst_, using)

#define res mangle(res_, lst)
#define fit mangle(fit_, lst)
#define end mangle(end_, lst)

#define clr mangle(lst, _clr)
#define fil mangle(lst, _fil)

#define add mangle(lst_add_, using)
#define pop mangle(lst_pop_, using)

#define get mangle(lst_get_, using)
#define set mangle(lst_set_, using)

#define ins mangle(lst_ins_, using)
#define rem mangle(lst_rem_, using)

#define ext mangle(ext_, lst)
#define cat mangle(cat_, lst)
#define cpy mangle(cpy_, lst)

// extra

#define empty mangle(lst, _empty)

#define equal mangle(lst, _equal)

#define each mangle(lst_each_, using)

#define foldl mangle(lst, _foldl)
#define foldr mangle(lst, _foldr)

#define map mangle(lst, _map)

#define any mangle(lst, _any)
#define keep mangle(lst, _keep)
#define filter mangle(lst, _filter)

#define predicate mangle(predicate_, using)
typedef u8 (*predicate)(using);

#define compare mangle(compare_, using)
typedef u8 (*compare)(using, using);

#define endo mangle(endo_, using)
typedef using (*endo)(using);

#define operator mangle(operator_, using)
typedef using (*operator)(using, using);
