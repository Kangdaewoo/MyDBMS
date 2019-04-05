typedef struct {
    char **fields;
    char **data;
    int numFields;
} FDPair;

typedef struct {
    char **fields;
    char **data;
    char **other;
    int numFields;
} FDTriplet;

typedef struct keyList {
    char **keys;
    int numKeys;
    BPT *bpt;
    keyList *next;
} KeyList;


bool contains(char **fields, int numFields, char *field);

int find(char **fields, int numFields, char *field);