//
// strings are just lists of chars.
//

#include "compact.c"

#define using chr
#include "list.c"

#define rope list_chr

#define rope_empty empty_chr

#define rope_eql eql_chr

#define extend ext_chr
#define concat cat_chr
