// YK
#include "yk__lib.h"
// --forward declarations-- 
struct yy__Student;
struct yy__Spaceship;
int32_t yy__main();
// --structs-- 
struct yy__Student {
    yk__sds yy__name;
    int32_t yy__age;
};
struct yy__Spaceship {
    yk__sds yy__name;
    yk__sds yy__class_;
};
// --functions-- 
int32_t yy__main() 
{
    int32_t yy__a = INT32_C(10);
    while (1) {
        if (!((yy__a != INT32_C(0)))) { break; } // Check
        // Body of while loop

        {
            yk__printint(((intmax_t)yy__a));
            yk__sds t__0 = yk__sdsnewlen("\n", 1);
            yk__printstr((t__0));
            yy__a = (yy__a - INT32_C(1));
            yk__sdsfree(t__0);
        }
    }
    return INT32_C(0);
}
#if defined(YK__MINIMAL_MAIN)
int main(void) { return yy__main(); }
#endif