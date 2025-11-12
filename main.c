#include "skiplist.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

static void trim(char* s) {
    int n = (int)strlen(s);
    while (n>0 && (s[n-1]=='\n' || s[n-1]=='\r' || s[n-1]==' ' || s[n-1]=='\t')) s[--n] = '\0';
}

int main(void) {
    skiplist* s = skl_create();
    if (!s) { fprintf(stderr, "Failed to create skiplist\n"); return 1; }

    printf("Skiplist REPL. Commands: insert X | delete X | lookup X | print | levels | size | quit\n");

    char line[256];
    while (1) {
        printf("> ");
        if (!fgets(line, sizeof(line), stdin)) break;
        trim(line);
        if (line[0] == '\0') continue;

        char cmd[32];
        int val;
        if (sscanf(line, "%31s %d", cmd, &val) >= 1) {
            if (strcmp(cmd, "insert") == 0) {
                if (sscanf(line, "%*s %d", &val) == 1) {
                    skl_insert(s, val);
                    printf("inserted %d\n", val);
                } else {
                    printf("usage: insert X\n");
                }
            } else if (strcmp(cmd, "delete") == 0) {
                if (sscanf(line, "%*s %d", &val) == 1) {
                    if (skl_delete(s, val)) printf("deleted one occurrence of %d\n", val);
                    else printf("%d not found\n", val);
                } else {
                    printf("usage: delete X\n");
                }
            } else if (strcmp(cmd, "lookup") == 0) {
                if (sscanf(line, "%*s %d", &val) == 1) {
                    int cnt = 0;
                    if (skl_lookup(s, val, &cnt)) printf("found %d (count=%d)\n", val, cnt);
                    else printf("not found: %d\n", val);
                } else {
                    printf("usage: lookup X\n");
                }
            } else if (strcmp(cmd, "print") == 0) {
                skl_print(s);
            } else if (strcmp(cmd, "levels") == 0) {
                skl_print_levels(s);
            } else if (strcmp(cmd, "size") == 0) {
                printf("distinct=%zu\n", skl_size(s));
            } else if (strcmp(cmd, "quit") == 0 || strcmp(cmd, "exit") == 0) {
                break;
            } else {
                printf("Unknown command: %s\n", cmd);
            }
        }
    }

    skl_free(s);
    return 0;
}
