#include <stdio.h>
#include "goodList.h"

Good *goodListHead;

void load_good_file() {
    goodListHead = init_good_list();

    int c;
    FILE *file;
    file = fopen("goods", "r");
    if (file) {
        int idx = 0;
        char cache[128];
        char *name;
        char *idStr;
        unsigned long id;
        int len;
        while ((c = getc(file)) != EOF) {
            if (c != '=' && c != ';') {
                cache[idx] = c;
                idx++;
            } else if (c == '=') {
                len = idx + 1;
                name = malloc(len);
                memcpy(name, &cache[0], len);
                name[len - 1] = '\0';
                idx = 0;
            } else if (c == ';') {
                len = idx + 1;
                idStr = malloc(len);
                memcpy(idStr, &cache[0], len);
                idStr[len - 1] = '\0';
                id = strtoul(idStr, NULL, 10);
                good_list_add(goodListHead, name, id);

                idx = 0;
            }
        }
        fclose(file);
    }
}

Good *find_by_name(char *keyword) {
    Good *p = goodListHead;
    Good *result = (Good *)malloc(GOOD_LEN);
    result->n = NULL;
    result->v = -1;
    while(p->next != NULL) {
        p = p->next;
        if (strstr(p->n, keyword) != NULL) {
            good_list_add_by_name(result, p->n, p->v);
        }
    }
    return result;
}