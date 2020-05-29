#include "type.c"
#include "lib.c"
pair_list l;
int main()
{
    int a = 0, b = 0;
    list_show(&l);
    list_add(&l, &a, &b);
    a = 1, b = 1;
    list_add(&l, &a, &b);
    a = 1, b = 3;
    list_add(&l, &a, &b);
    a = 2, b = 1;
    list_add(&l, &a, &b);
    list_show(&l);
    list_pop(&l, &a, &b);
    list_show(&l);
    list_pop(&l, &a, &b);
    list_show(&l);
    list_pop(&l, &a, &b);
    list_show(&l);
    list_pop(&l, &a, &b);
    list_show(&l);
    list_pop(&l, &a, &b);
    list_show(&l);
    a = 1, b = 1;
    list_add(&l, &a, &b);
    a = 1, b = 3;
    list_add(&l, &a, &b);
    a = 2, b = 1;
    list_add(&l, &a, &b);
    list_show(&l);
    return 0;
}