// YK
#include "yk__lib.h"
// --forward declarations-- 
#define yy__numbers_cstr2i atoi
#define yy__c_VoidPtr void*
#define yy__c_Size size_t
#define yy__c_CStr char*
#define yy__c_CDouble double
#define yy__c_CBool bool
#define yy__c_CInt int
#define yy__c_CLong long
#define yy__c_CShort short
#define yy__c_CUInt unsigned int
#define yy__c_CULong unsigned long
#define yy__c_CUShort unsigned short
#define yy__c_free free
#define yy__c_calloc calloc
#define yy__c_realloc realloc
#define yy__c_malloc malloc
#define yy__c_system system
#define yy__c_acos acos
#define yy__c_asin asin
#define yy__c_atan atan
#define yy__c_atan2 atan2
#define yy__c_cos cos
#define yy__c_cosh cosh
#define yy__c_sin sin
#define yy__c_sinh sinh
#define yy__c_tan tan
#define yy__c_tanh tanh
#define yy__c_log log
#define yy__c_log10 log10
#define yy__c_log2 log2
#define yy__c_pow pow
#define yy__c_sqrt sqrt
#define yy__c_ceil ceil
#define yy__c_fabs fabs
#define yy__c_floor floor
#define yy__c_fmod pow
#define yy__c_memcpy memcpy
#define yy__c_memmove memmove
#define yy__c_memset memset
#define yy__c_memcmp memcmp
#define yy__mpc_Ast mpc_ast_t*
#define yy__mpc_Mpc mpc_parser_t*
#define yy__mpc_Result mpc_result_t*
#define yy__mpc_new mpc_new
#define yy__mpc_mpca_lang mpca_lang
#define yy__mpc_cleanup mpc_cleanup
#define yy__mpc_parse mpc_parse
int32_t yy__strings_get(yy__c_CStr, int32_t);
bool yy__strings_is_empty_str(yk__sds);
bool yy__strings_startswith(yk__sds, yk__sds);
yk__sds yy__strings_from_cstr(yy__c_CStr);
yy__c_CStr yy__strings_to_cstr(yk__sds);
void yy__strings_del_cstr(yy__c_CStr);
bool yy__c_has_command_processor();
yy__mpc_Result yy__mpc_new_result();
void yy__mpc_ast_print(yy__mpc_Result);
void yy__mpc_err_print(yy__mpc_Result);
void yy__mpc_ast_del(yy__mpc_Result);
void yy__mpc_err_del(yy__mpc_Result);
yk__sds yy__io_readfile(yk__sds);
yk__sds yy__io_readline(yk__sds);
int32_t yy__eval_op(int32_t, yk__sds, int32_t);
int32_t yy__eval(yy__mpc_Ast);
int32_t yy__main();
// --structs-- 
// --functions-- 
int32_t yy__strings_get(yy__c_CStr nn__s, int32_t nn__pos) 
{
    return (int32_t)nn__s[nn__pos];
}
bool yy__strings_is_empty_str(yk__sds nn__s) 
{
    bool x = yk__sdslen(nn__s) == 0;
    yk__sdsfree(nn__s);
    return x;
}
bool yy__strings_startswith(yk__sds nn__a, yk__sds nn__b) 
{
    bool x = (strstr(nn__a, nn__b) != NULL);
    yk__sdsfree(nn__a);
    yk__sdsfree(nn__b);
    return x;
}
yk__sds yy__strings_from_cstr(yy__c_CStr nn__a) 
{
    return yk__sdsnewlen(nn__a, strlen(nn__a));
}
yy__c_CStr yy__strings_to_cstr(yk__sds nn__a) 
{
    return nn__a;
}
void yy__strings_del_cstr(yy__c_CStr nn__a) 
{
    free(nn__a);
}
bool yy__c_has_command_processor() 
{
    return (!!system(NULL));
}
yy__mpc_Result yy__mpc_new_result() 
{
    return calloc(1, sizeof(mpc_result_t));
}
void yy__mpc_ast_print(yy__mpc_Result nn__r) 
{
    mpc_ast_print(nn__r->output);
}
void yy__mpc_err_print(yy__mpc_Result nn__r) 
{
    mpc_err_print(nn__r->output);
}
void yy__mpc_ast_del(yy__mpc_Result nn__r) 
{
    mpc_ast_delete(nn__r->output);
}
void yy__mpc_err_del(yy__mpc_Result nn__r) 
{
    mpc_err_delete(nn__r->output);
}
yk__sds yy__io_readfile(yk__sds nn__fname) 
{
    size_t ln;
    int err;
    char *out = yk__bhalib_read_file(nn__fname, &ln, &err);
    yk__sdsfree(nn__fname); // clean up filename as it will be copied
    if (err == 0) { return yk__sdsnewlen(out, ln); }
    return yk__sdsempty();
}
yk__sds yy__io_readline(yk__sds nn__prompt) 
{
    char* input = yk__readline(nn__prompt);
    yk__sds result = yk__sdsnew(input);
    free(input);
    yk__sdsfree(nn__prompt);
    return result;
}
int32_t yy__eval_op(int32_t yy__x, yk__sds yy__op, int32_t yy__y) 
{
    yk__sds t__0 = yk__sdsnewlen("+", 1);
    if ((yk__sdscmp(yy__op , t__0) == 0))
    {
        int32_t t__1 = (yy__x + yy__y);
        yk__sdsfree(t__0);
        yk__sdsfree(yy__op);
        return t__1;
    }
    yk__sds t__2 = yk__sdsnewlen("-", 1);
    if ((yk__sdscmp(yy__op , t__2) == 0))
    {
        int32_t t__3 = (yy__x - yy__y);
        yk__sdsfree(t__2);
        yk__sdsfree(t__0);
        yk__sdsfree(yy__op);
        return t__3;
    }
    yk__sds t__4 = yk__sdsnewlen("*", 1);
    if ((yk__sdscmp(yy__op , t__4) == 0))
    {
        int32_t t__5 = (yy__x * yy__y);
        yk__sdsfree(t__4);
        yk__sdsfree(t__2);
        yk__sdsfree(t__0);
        yk__sdsfree(yy__op);
        return t__5;
    }
    yk__sds t__6 = yk__sdsnewlen("/", 1);
    if ((yk__sdscmp(yy__op , t__6) == 0))
    {
        int32_t t__7 = (yy__x / yy__y);
        yk__sdsfree(t__6);
        yk__sdsfree(t__4);
        yk__sdsfree(t__2);
        yk__sdsfree(t__0);
        yk__sdsfree(yy__op);
        return t__7;
    }
    yk__sdsfree(t__6);
    yk__sdsfree(t__4);
    yk__sdsfree(t__2);
    yk__sdsfree(t__0);
    yk__sdsfree(yy__op);
    return INT32_C(0);
}
int32_t yy__eval(yy__mpc_Ast yy__t) 
{
    yk__sds t__8 = yy__strings_from_cstr(yy__t->tag);
    yk__sds t__9 = yk__sdsnewlen("number", 6);
    if (yy__strings_startswith(yk__sdsdup((t__8)), yk__sdsdup(t__9)))
    {
        int32_t t__10 = yy__numbers_cstr2i(yy__t->contents);
        yk__sdsfree(t__9);
        yk__sdsfree(t__8);
        return t__10;
    }
    yk__sds t__11 = yy__strings_from_cstr(yy__t->children[INT32_C(1)]->contents);
    yk__sds yy__op = yk__sdsdup((t__11));
    int32_t yy__x = yy__eval(yy__t->children[INT32_C(2)]);
    int32_t yy__i = INT32_C(3);
    while (1) {
        if (!(true)) { break; } // Check
        // Body of while loop

        {
            yk__sds t__12 = yy__strings_from_cstr(yy__t->children[yy__i]->tag);
            yk__sds t__13 = yk__sdsnewlen("expr", 4);
            if (yy__strings_startswith(yk__sdsdup((t__12)), yk__sdsdup(t__13)))
            {
                yy__x = yy__eval_op(yy__x, yk__sdsdup(yy__op), yy__eval(yy__t->children[yy__i]));
                yy__i = (yy__i + INT32_C(1));
            }
            else
            {
                yk__sdsfree(t__13);
                yk__sdsfree(t__12);
                break;
            }
            yk__sdsfree(t__13);
            yk__sdsfree(t__12);
        }
    }
    int32_t t__14 = yy__x;
    yk__sdsfree(yy__op);
    yk__sdsfree(t__11);
    yk__sdsfree(t__9);
    yk__sdsfree(t__8);
    return t__14;
}
int32_t yy__main() 
{
    yk__sds t__15 = yk__sdsnewlen("number", 6);
    yy__mpc_Mpc yy__number = yy__mpc_new(yk__sdsdup(t__15));
    yk__sds t__16 = yk__sdsnewlen("operator", 8);
    yy__mpc_Mpc yy__operator = yy__mpc_new(yk__sdsdup(t__16));
    yk__sds t__17 = yk__sdsnewlen("expr", 4);
    yy__mpc_Mpc yy__expr = yy__mpc_new(yk__sdsdup(t__17));
    yk__sds t__18 = yk__sdsnewlen("lispy", 5);
    yy__mpc_Mpc yy__lispy = yy__mpc_new(yk__sdsdup(t__18));
    yk__sds t__19 = yk__sdsnewlen("\n      number   : /-\?[0-9]+/ ;\n      operator : \'+\' | \'-\' | \'*\' | \'/\' ;\n      expr     : <number> | \'(\' <operator> <expr>+ \')\' ;\n      lispy    : /^/ <operator> <expr>+ /$/ ;\n    ", 179);
    yk__sds yy__lang = yk__sdsdup(t__19);
    yy__mpc_mpca_lang(INT32_C(0), yk__sdsdup(yy__lang), yy__number, yy__operator, yy__expr, yy__lispy);
    yk__sds t__20 = yk__sdsnewlen("Lispy Version 0.0.0.0.2\n", 24);
    yk__printstr((t__20));
    yk__sds t__21 = yk__sdsnewlen("Type just \'q\' to exit.\n\n", 24);
    yk__printstr((t__21));
    yk__sds t__22 = yk__sdsnewlen("<stdin>", 7);
    yk__sds yy__filename = yk__sdsdup(t__22);
    yy__mpc_Result yy__r = yy__mpc_new_result();
    while (1) {
        if (!(true)) { break; } // Check
        // Body of while loop

        {
            yk__sds t__23 = yk__sdsnewlen("lispy> ", 7);
            yk__sds t__24 = yy__io_readline(yk__sdsdup(t__23));
            yk__sds yy__inp = yk__sdsdup((t__24));
            yk__sds t__25 = yk__sdsnewlen("q", 1);
            if ((yk__sdscmp(yy__inp , t__25) == 0))
            {
                free(yy__r);
                free(yy__lispy);
                free(yy__expr);
                free(yy__operator);
                free(yy__number);
                yk__sdsfree(t__25);
                yk__sdsfree(yy__inp);
                yk__sdsfree(t__24);
                yk__sdsfree(t__23);
                yk__sdsfree(yy__filename);
                yk__sdsfree(t__22);
                yk__sdsfree(t__21);
                yk__sdsfree(t__20);
                yk__sdsfree(yy__lang);
                yk__sdsfree(t__19);
                yk__sdsfree(t__18);
                yk__sdsfree(t__17);
                yk__sdsfree(t__16);
                yk__sdsfree(t__15);
                return INT32_C(0);
            }
            if (yy__mpc_parse(yk__sdsdup(yy__filename), yk__sdsdup(yy__inp), yy__lispy, yy__r))
            {
                yk__printlnint(((intmax_t)yy__eval(yy__r->output)));
                yy__mpc_ast_del(yy__r);
            }
            else
            {
                yy__mpc_err_print(yy__r);
                yy__mpc_err_del(yy__r);
            }
            yk__sdsfree(t__25);
            yk__sdsfree(yy__inp);
            yk__sdsfree(t__24);
            yk__sdsfree(t__23);
        }
    }
    yy__mpc_cleanup(INT32_C(4), yy__number, yy__operator, yy__expr, yy__lispy);
    yk__sds t__26 = yk__sdsnewlen(">>>bye!\n", 8);
    yk__printstr((t__26));
    free(yy__r);
    free(yy__lispy);
    free(yy__expr);
    free(yy__operator);
    free(yy__number);
    yk__sdsfree(t__26);
    yk__sdsfree(yy__filename);
    yk__sdsfree(t__22);
    yk__sdsfree(t__21);
    yk__sdsfree(t__20);
    yk__sdsfree(yy__lang);
    yk__sdsfree(t__19);
    yk__sdsfree(t__18);
    yk__sdsfree(t__17);
    yk__sdsfree(t__16);
    yk__sdsfree(t__15);
    return INT32_C(0);
}
#if defined(YK__MINIMAL_MAIN)
int main(void) { return yy__main(); }
#endif