#include <stdio.h>
#include "utils.h"

#if !defined(TREE_DEPTH)
#define TREE_DEPTH 5
#endif


class BPT {
    private:
        FILE *file;
        int lastOffset;

        char *keys[];
        int numKeys;

    public:
        BPT(char *schemaName, char *keys[], int numKeys);

        int insert(FDPair data, int offset);
        int *find(FDTriplet keys);
        int *remove(int *offsets);
};

typedef struct {
    void *key;
    int parent;
    int siblings[TREE_DEPTH];
    int children[TREE_DEPTH + 1];
} Node;

typedef struct {
    void *data;
    int leftSibling;
    int rightSibling;
} Leaf;