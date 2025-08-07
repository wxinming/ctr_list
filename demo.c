#include <stdio.h>
#include "list.h"

void ctr_foreach_cb(const void* data, void* args)
{
    printf("%d\n", *(int*)data);
}

int ctr_erase_cb(const void* data, void* args)
{
    if (*(int*)data == 3) {
        return 1;
    }
    return 0;
}

int ctr_find_cb(const void* data, void* args)
{
    if (*(int*)data == 1) {
        return 1;
    }
    return 0;
}

int main()
{
    void* ctx = ctr_list_new();
    for (int i = 0; i < 10; ++i) {
        ctr_list_push_back(ctx, &i, sizeof(i));
    }

    for (int i = 0; i < 5; ++i) {
        ctr_list_pop_back(ctx);
    }

    ctr_list_erase_condition(ctx, ctr_erase_cb, NULL);
    ctr_list_erase_index(ctx, 0);
    //ctr_list_clear(ctx);
    for (int i = 0; i < 10; ++i) {
        ctr_list_push_back(ctx, &i, sizeof(i));
    }
    int data = 1024;
    ctr_list_insert(ctx, 10, &data, sizeof(data));
    ctr_list_foreach(ctx, ctr_foreach_cb, NULL);
    printf("find %lu\n", ctr_list_find(ctx, ctr_find_cb, NULL));
    printf("size %lu, empty %d\n", ctr_list_size(ctx), ctr_list_empty(ctx));
    ctr_list_free(&ctx);
    printf("ctx %p\n", ctx);
    return 0;
}

