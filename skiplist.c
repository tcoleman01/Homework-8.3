#include "skiplist.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

static int rand_level(void) {
    int lvl = 0;
    while ((double)rand() / RAND_MAX < SKL_P && lvl < SKL_MAX_LEVEL-1) {
        lvl++;
    }
    return lvl;
}

skiplist* skl_create(void) {
    skiplist* s = (skiplist*)calloc(1, sizeof(skiplist));
    if (!s) return NULL;
    s->head = (skl_node*)calloc(1, sizeof(skl_node));
    if (!s->head) { free(s); return NULL; }
    s->head->level = SKL_MAX_LEVEL-1;
    for (int i = 0; i < SKL_MAX_LEVEL; ++i) s->head->next[i] = NULL;
    s->level = 0;
    s->distinct = 0;
    srand((unsigned)time(NULL));
    return s;
}

static void free_chain(skl_node* n) {
    while (n) {
        skl_node* nx = n->next[0];
        free(n);
        n = nx;
    }
}

void skl_free(skiplist* s) {
    if (!s) return;
    free_chain(s->head);
    free(s);
}

bool skl_lookup(skiplist* s, int key, int* out_count) {
    skl_node* x = s->head;
    for (int i = s->level; i >= 0; --i) {
        while (x->next[i] && x->next[i]->key < key) x = x->next[i];
    }
    x = x->next[0];
    if (x && x->key == key) {
        if (out_count) *out_count = x->count;
        return true;
    }
    return false;
}

void skl_insert(skiplist* s, int key) {
    skl_node* update[SKL_MAX_LEVEL];
    skl_node* x = s->head;

    for (int i = s->level; i >= 0; --i) {
        while (x->next[i] && x->next[i]->key < key) x = x->next[i];
        update[i] = x;
    }
    x = x->next[0];

    if (x && x->key == key) {
        x->count += 1;
        return;
    }

    int lvl = rand_level();
    if (lvl > s->level) {
        for (int i = s->level + 1; i <= lvl; ++i) update[i] = s->head;
        s->level = lvl;
    }

    skl_node* n = (skl_node*)calloc(1, sizeof(skl_node));
    n->key = key;
    n->count = 1;
    n->level = lvl;
    for (int i = 0; i <= lvl; ++i) {
        n->next[i] = update[i]->next[i];
        update[i]->next[i] = n;
    }
    s->distinct += 1;
}

bool skl_delete(skiplist* s, int key) {
    skl_node* update[SKL_MAX_LEVEL];
    skl_node* x = s->head;

    for (int i = s->level; i >= 0; --i) {
        while (x->next[i] && x->next[i]->key < key) x = x->next[i];
        update[i] = x;
    }
    x = x->next[0];

    if (!x || x->key != key) return false;

    if (x->count > 1) {
        x->count -= 1;
        return true;
    }

    for (int i = 0; i <= s->level; ++i) {
        if (update[i]->next[i] == x) update[i]->next[i] = x->next[i];
    }
    free(x);

    while (s->level > 0 && s->head->next[s->level] == NULL) s->level--;

    s->distinct -= 1;
    return true;
}

size_t skl_size(skiplist* s) {
    return s ? s->distinct : 0;
}

void skl_print(skiplist* s) {
    skl_node* x = s->head->next[0];
    printf("[");
    int first = 1;
    while (x) {
        if (!first) printf(", ");
        printf("%d(x%d)", x->key, x->count);
        first = 0;
        x = x->next[0];
    }
    printf("]\n");
}

void skl_print_levels(skiplist* s) {
    int level_used[SKL_MAX_LEVEL] = {0};
    skl_node* x = s->head->next[0];
    while (x) {
        level_used[x->level] += 1;
        x = x->next[0];
    }
    printf("max_level=%d, distinct=%zu\n", s->level, s->distinct);
    printf("level histogram (#nodes having exactly that top-level):\n");
    for (int i = s->level; i >= 0; --i) {
        printf("  L%-2d : %d\n", i, level_used[i]);
    }
}
