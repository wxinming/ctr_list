#ifndef __CTR_LIST__
#define __CTR_LIST__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CTR_LIST_NPOS (-1UL)

void* ctr_list_new();
void ctr_list_free(void** ctx);
void ctr_list_clear(void* ctx);
void ctr_list_push_front(void* ctx, const void* data, size_t size);
void ctr_list_push_back(void* ctx, const void* data, size_t size);
void ctr_list_pop_front(void* ctx);
void ctr_list_pop_back(void* ctx);
void* ctr_list_front(void* ctx);
void* ctr_list_back(void* ctx);
size_t ctr_list_size(void* ctx);
size_t ctr_list_find(void* ctx, int(*findcb)(const void* data, void* args), void* args);
void ctr_list_insert(void* ctx, size_t index, const void* data, size_t size);
void ctr_list_foreach(void* ctx, void(*foreachcb)(const void* data, void* args), void* args);
void ctr_list_erase_condition(void* ctx, int(*erasecb)(const void* data, void* args), void* args);
void ctr_list_erase_index(void* ctx, size_t index);
void* ctr_list_at(void* ctx, size_t index);
int ctr_list_empty(void* ctx);

#endif
