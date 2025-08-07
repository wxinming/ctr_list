#include "list.h"

#define CTR_CAST_CTX(ctx) ((ctr_list_ctx_t*)ctx)
#define CTR_CAST_NODE(node) ((ctr_list_node_t*)node)

typedef struct ctr_list_node {
    void* data;
    struct ctr_list_node* next;
} ctr_list_node_t;

typedef struct ctr_list_ctx {
    size_t size;
    ctr_list_node_t* node;
    //struct ctr_list_node* head;
    //struct ctr_list_node* tail;
} ctr_list_ctx_t;

static void* ctr_list_create_node(const void* data, size_t size)
{
    ctr_list_node_t* node = malloc(sizeof(ctr_list_node_t));
    if (node == NULL) {
        return NULL;
    }
    node->data = malloc(size);
    memset(node->data, 0, size);
    memcpy(node->data, data, size);
    node->next = NULL;
    return node;
}

void* ctr_list_new()
{
    ctr_list_ctx_t* ctx = malloc(sizeof(ctr_list_ctx_t));
    ctx->node = malloc(sizeof(ctr_list_node_t));
    ctx->node->data = NULL;
    ctx->node->next = NULL;
    ctx->size = 0;
    //ctx->head = NULL;
    //ctx->tail = NULL;
    return ctx;
}

void ctr_list_free(void** ctx)
{
    ctr_list_ctx_t* curr = *ctx;
    while(curr->node->next) {
        ctr_list_node_t* node = curr->node->next;
        curr->node->next = node->next;
        free(node->data);
        free(node);
    }
    free(curr->node);
    free(curr);
    *ctx = NULL;
}

void ctr_list_clear(void* ctx)
{
    ctr_list_ctx_t* curr = ctx;
    while (curr->node->next) {
        ctr_list_node_t* node = curr->node->next;
        curr->node->next = node->next;
        free(node->data);
        free(node);
    }
    curr->size = 0;
}

void ctr_list_push_front(void* ctx, const void* data, size_t size) 
{
    void* node = ctr_list_create_node(data, size);
    if (node == NULL) {
        return;
    }
    ctr_list_node_t* curr = CTR_CAST_CTX(ctx)->node->next;
    CTR_CAST_CTX(ctx)->node->next = node;
    CTR_CAST_NODE(node)->next = curr;
    ++CTR_CAST_CTX(ctx)->size;
}

void ctr_list_push_back(void* ctx, const void* data, size_t size)
{
    void* node = ctr_list_create_node(data, size);
    if (node == NULL) {
        return;
    }
    ctr_list_node_t* curr = CTR_CAST_CTX(ctx)->node->next;
    if (curr == NULL) {
        CTR_CAST_CTX(ctx)->node->next = node;
    }
    else {
        while (curr->next) {
            curr = curr->next;
        }
        curr->next = node;
    }
    ++CTR_CAST_CTX(ctx)->size;
}

void ctr_list_pop_front(void* ctx)
{
    ctr_list_node_t* node = CTR_CAST_CTX(ctx)->node->next;
    CTR_CAST_CTX(ctx)->node->next = node->next;
    free(node->data);
    free(node);
    --CTR_CAST_CTX(ctx)->size;
}

void ctr_list_pop_back(void* ctx)
{
    ctr_list_node_t* curr = CTR_CAST_CTX(ctx)->node->next;
    if (curr == NULL) {
        return;
    }

    ctr_list_node_t* prev = CTR_CAST_CTX(ctx)->node->next;
    while (curr->next) {
        prev = curr;
        curr = curr->next;
    }

    prev->next = NULL;
    free(curr->data);
    free(curr);
    --CTR_CAST_CTX(ctx)->size;
}

void* ctr_list_front(void* ctx)
{
    return ctr_list_at(ctx, 0);
}

void* ctr_list_back(void* ctx)
{
    return ctr_list_at(ctx, ctr_list_size(ctx) - 1);
}

size_t ctr_list_size(void* ctx)
{
    return CTR_CAST_CTX(ctx)->size;
}

size_t ctr_list_find(void* ctx, int(*findcb)(const void* data, void* args), void* args)
{
    size_t result = -1, index = 0;
    ctr_list_node_t* node = CTR_CAST_CTX(ctx)->node->next;
    while (node) {
        if (findcb(node->data, args)) {
            result = index;
            break;
        }
        node = node->next;
        ++index;
    }
    return result;
}

void ctr_list_insert(void* ctx, size_t index, const void* data, size_t size)
{
    size_t list_size = ctr_list_size(ctx);
    if (index == 0) {
        ctr_list_push_front(ctx, data, size);
    }
    else if (index >= list_size) {
        ctr_list_push_back(ctx, data, size);
    }
    else {
        ctr_list_node_t* prev = NULL;
        size_t i = 0;
        ctr_list_node_t* node = CTR_CAST_CTX(ctx)->node->next;
        while (node) {
            if (i == index) {
                void* new_node = ctr_list_create_node(data, size);
                if (new_node == NULL) {
                    break;
                }
                prev->next = new_node;
                CTR_CAST_NODE(new_node)->next = node;
                ++CTR_CAST_CTX(ctx)->size;
                break;
            }
            ++i;
            prev = node;
            node = node->next;
        }
    }
}

void ctr_list_foreach(void* ctx, void(*foreachcb)(const void* data, void* args), void* args)
{
    ctr_list_node_t* node = CTR_CAST_CTX(ctx)->node->next;
    while (node) {
        foreachcb(node->data, args);
        node = node->next;
    }
}

void ctr_list_erase_condition(void* ctx, int(*erasecb)(const void* data, void* args), void* args)
{
    ctr_list_node_t* node = CTR_CAST_CTX(ctx)->node->next, *prev = CTR_CAST_CTX(ctx)->node;
    while(node) {
        if (erasecb(node->data, args)) {
            prev->next = node->next;
            free(node->data);
            free(node);
            --CTR_CAST_CTX(ctx)->size;
            break;
        }
        prev = node;
        node = node->next;
    }
}

void ctr_list_erase_index(void* ctx, size_t index)
{
    ctr_list_node_t* node = CTR_CAST_CTX(ctx)->node->next, *prev = CTR_CAST_CTX(ctx)->node;
    size_t i = 0;
    while (node) {
        if (i == index) {
            prev->next = node->next;
            free(node->data);
            free(node);
            --CTR_CAST_CTX(ctx)->size;
            break;
        }
        ++i;
        prev = node;
        node = node->next;
    }
}

void* ctr_list_at(void* ctx, size_t index)
{
    ctr_list_node_t* node = CTR_CAST_CTX(ctx)->node->next;
    size_t i = 0;
    while (node) {
        if (i == index) {
            return node->data;
        }
        ++i;
        node = node->next;
    }
    return NULL;
}

int ctr_list_empty(void* ctx)
{
    return ctr_list_size(ctx) == 0;
}

