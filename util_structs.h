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

typedef linkedList {
    void *ptr;
    linkedList *next;
} LinkedList;


int findField(char *fieldNames[], char field[], int numFields);