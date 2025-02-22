// YK
#include "yk__lib.h"
// --forward declarations-- 
#define yy__min_int(nn__a, nn__b) ((nn__a < nn__b) ? nn__a : nn__b)
#define yy__max_int(nn__a, nn__b) ((nn__a > nn__b) ? nn__a : nn__b)
yk__sds yy__readfile(yk__sds);
bool yy__is_empty_str(yk__sds);
void yy__leakme(yk__sds, yk__sds);
int32_t yy__main();
// --structs-- 
// --functions-- 
yk__sds yy__readfile(yk__sds nn__fname) 
{
    size_t ln;
    int err;
    char *out = yk__bhalib_read_file(nn__fname, &ln, &err);
    yk__sdsfree(nn__fname); // clean up filename as it will be copied
    if (err == 0) { return yk__sdsnewlen(out, ln); }
    return yk__sdsempty();
}
bool yy__is_empty_str(yk__sds nn__s) 
{
    bool x = yk__sdslen(nn__s) == 0;
    yk__sdsfree(nn__s);
    return x;
}
void yy__leakme(yk__sds nn__a, yk__sds nn__b) { fopen(nn__a, nn__b); }
int32_t yy__main() 
{
    yk__sds t__0 = yk__sdsnewlen("// ----->>> this is my code <<<<------\n", 39);
    yk__printstr((t__0));
    yk__sds t__1 = yk__sdsnewlen("test.c", 6);
    yk__sds t__2 = yy__readfile(yk__sdsdup(t__1));
    yk__sds yy__a = yk__sdsdup((t__2));
    if (yy__is_empty_str(yk__sdsdup(yy__a)))
    {
        yk__sds t__3 = yk__sdsnewlen("test.c is empty or not found", 28);
        yk__printstr((t__3));
        yk__sdsfree(t__3);
    }
    else
    {
        yk__printstr((yy__a));
    }
    yk__sdsfree(yy__a);
    yk__sdsfree(t__2);
    yk__sdsfree(t__1);
    yk__sdsfree(t__0);
    return INT32_C(0);
}
#if defined(YK__MINIMAL_MAIN)
int main(void) { return yy__main(); }
#endif