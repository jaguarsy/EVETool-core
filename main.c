#include "libs/api.h"
#include "model/dic.h"

int main(int argc, char *argv[])
{
    // char *typeIds[3];
    // typeIds[0] = "17715";
    // typeIds[1] = "34";
    // typeIds[2] = "35";
    // load_price(typeIds, 3);

    load_good_file();

    Good *result = find_by_name("vexor");
    Good *p = result;
    while(p->next!=NULL) {
        p = p-> next;
        printf("%s %lu\n", p->n, p->v);
    }

    return 0;
}