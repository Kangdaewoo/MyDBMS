#include <stdio.h>
#include "utils.h"

#if !defined(TREE_DEPTH)
#define TREE_DEPTH 5
#endif

using namespace std;

class BPT {
    private:
        PagedFileManager fileManager;
        const char *keyNames[];
        const char *keyTypes[];
        int numKeys;
        int root;
        int rowSize;

        // Helpers.
        string rowToString(FDPair data);
        FDPair stringToRow(string buf);

    public:
        BPT(string schemaName, FDPair keyNamesAndTypes);

        void insert(FDPair data, int offset);
        int[] find(FDTriplet tests);
        void remove(FDPair pair, int offset);
        // Update is done by removing the old and inserting the new.

        int containsKey(string[] fields, int numFields);
        bool equals(string[] fields, int numFields);
};

typedef struct {
    char *keys[];
    bool isLeaf;
    int leftSibling;
    int rightSibling;
    int children[];
} Node;