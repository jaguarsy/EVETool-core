#include <stdio.h> 

#define GOOD_LEN sizeof(struct Good)  

typedef struct Good {
    char *n;
    unsigned long v;
    struct Good *next;
} Good;

Good *init_good_list() {  
    Good *head;
    head = (Good *)malloc(GOOD_LEN);
    if(head == NULL) {
        printf("申请内存空间失败/n");        
    }
    head->n = NULL;
    head->v = -1;
    head->next = NULL;
    return head;
}

Good *good_list_add(Good *head, char *name, unsigned long id) {
    Good *p = (Good *)malloc(GOOD_LEN);
    p->n = name;
    p->v = id;
    p->next = head->next;
    head->next = p;
    return p;
}

Good *good_list_add_by_name(Good *head, char *name, unsigned long id) {
    Good *p = (Good *)malloc(GOOD_LEN);
    p->n = name;
    p->v = id;
    int nameLen = strlen(name);
    Good *t = head;
    if (t->next == NULL) {
        t->next = p;
        p->next = NULL;
        return p;
    }

    while(t->next != NULL && strlen(t->next->n) < nameLen) {
        t = t->next;
    }
    p->next = t->next;
    t->next = p;
    return p;
}