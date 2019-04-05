#include "BPT.h"
#include <stdio.h>

typedef struct keyList {
    char **keys;
    int numKeys;
    BPT *bpt;
    keyList *next;
} KeyList;

class Schema {
    private: 
        char *schemaName;

        char **fieldNames;
        char **fieldTypes;
        int *fieldSizes;
        int numFields;

        KeyList *keyList;

        int rowSize;
        int numRows;

        FILE *file;

        int write(FDPair data);
    
    public:
        Schema(char *schemaName, FDPair fieldNamesAndTypes, int *fieldSizes);

        int insert(FDPair *data);
        int *update(FDTriplet key, FDPair data);
        int *find(FDPair keys);
        int *remove(FDPair *key);
        
        bool isEmpty();
        int size();
        KeyList *containsKey(char **keys, int numKeys);
        bool setKeys(char **keys, int numKeys);

        void rowToChar(FDPair data, char *buf);
        void charToRow(char *buf, FDPair *data);
};