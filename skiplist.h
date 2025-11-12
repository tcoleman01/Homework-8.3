#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <stdbool.h>
#include <stddef.h>

#define SKL_MAX_LEVEL 32
#define SKL_P 0.5

typedef struct skl_node {
    int key;
    int count; 
    int level; 
    struct skl_node* next[SKL_MAX_LEVEL];
} skl_node;

typedef struct {
    skl_node* head;    
    int level;         
    size_t distinct;   
} skiplist;

skiplist* skl_create(void);
void      skl_free(skiplist* s);

void  skl_insert(skiplist* s, int key);     
bool  skl_delete(skiplist* s, int key);     
bool  skl_lookup(skiplist* s, int key, int* out_count);
size_t skl_size(skiplist* s);               

void  skl_print(skiplist* s);               
void  skl_print_levels(skiplist* s);        

#endif
