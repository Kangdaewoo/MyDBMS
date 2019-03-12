typedef struct {
    char **fields;
    char **data;
    int numFields;
} FDPair;

int extractData(FDPair data, char *field) {
    for (int i = 0; i < data.numFields; i++) {
        if (strcmp(field, data.fields[i]) == 0) {
            return i;
        }
    }
    return -1;
}

bool containsField(FDPair data, char *field) {
    for (int i = 0; i < data.numFields; i++) {
        if (strcmp(field, data.fields[i]) == 0) {
            return true;
        }
    }
    return false;
}

typedef struct {
    char **fields;
    char **data;
    char **other;
    int numFields;
} FDTriplet;

bool containsField(FDTriplet data, char *field) {
    for (int i = 0; i < data.numFields; i++) {
        if (strcmp(field, data.fields[i]) == 0) {
            return true;
        }
    }
    return false;
}

typedef struct keyList {
    char **keys;
    int numKeys;
    BPT *bpt;
    keyList *next;
} KeyList;