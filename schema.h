#include "BPT.h"
#include <stdio.h>
#include <map>

class Schema {
    private: 
        char *schemaName;

        char **fieldNames;
        char **fieldTypes;
        int *fieldSizes;
        int numFields;

        KeyList *keyList;

        int totalSize;

        FILE *file;
        int lastOffset;

        void writeToFile(FDPair data);
    
    public:
        Schema(char *schemaName, FDPair fieldNamesAndTypes, int *fieldSizes);

        int insert(FDPair *data);
        int *update(FDTriplet key, FDPair data);
        int *find(FDPair keys);
        int *remove(FDPair *key);
        
        bool isEmpty();
        bool setKeys(char **keys, int numKeys);
};