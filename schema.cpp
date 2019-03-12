#include "schema.h"
#include <iostream>


Schema::Schema(char *schemaName, FDPair fieldNamesAndTypes, int *fieldSizes): numFields(fieldNamesAndTypes.numFields), lastOffset(0) {
    schemaName = malloc(sizeof(char) * strlen(schemaName));
    strcpy(this->schemaName, schemaName);

    this->fieldNames = malloc(sizeof(char *) * numFields);
    this->fieldTypes = malloc(sizeof(char *) * numFields);
    this->fieldSizes = malloc(sizeof(int) * numFields);

    totalSize = 0;
    for (int i = 0; i < numFields; i++) {
        this->fieldNames[i] = malloc(sizeof(char) * strlen(fieldNamesAndTypes.fields[i]));
        strcpy_s(this->fieldNames[i], fieldNamesAndTypes.fields[i]);

        this->fieldTypes[i] = malloc(sizeof(char) * strlen(fieldNamesAndTypes.data[i]));
        strcpy_s(this->fieldTypes[i], fieldNamesAndTypes.data[i]);

        this->fieldSizes[i] = fieldSizes[i];
        totalSize += fieldSizes[i];
    }

    file = fopen(this->schemaName, "w");
    keyList = nullptr;
}

bool Schema::isEmpty() {
    return lastOffset == 0;
}

bool setKeys(char **keys, int numKeys) {
    KeyList *temp = keyList;
    while (temp != nullptr) {
        bool found = false;
        for (int i = 0; i < numKeys; i++) {
            for (int j = 0; j < temp->numKeys; j++) {
                if (strcmp(keys[i], temp->keys[j]) == 0) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                break;
            }
            found = false;
        }
        if (found) {
            return false;
        }
        temp = temp->next;
    }

    KeyList *newKeyList = malloc(sizeof(KeyList));
    newKeyList->keys = malloc(sizeof(char *) * numKeys);
    for (int i = 0; i < numKeys; i++) {
        newKeyList->keys[i] = malloc(sizeof(char) * (strlen(keys[i]) + 1));
        strcpy_s(newKeyList->keys[i], keys[i]);
    }
    newKeyList->numKeys = numKeys;
    newKeyList->bpt = BPT() // TO DO.

    newKeyList->next = keyList;
    keyList = newKeyList;
    return true;
}

void Schema::writeToFile(FDPair data) {
    fseek(file, lastOffset, SEEK_SET);
    for (int i = 0; i < numFields; i++) {
        char toWrite[fieldSizes[i] + 1];
        memset(toWrite, '\0', fieldSizes[i] + 1);
        strcpy(toWrite, extractData(data, fieldNames[i]));
        fwrite(toWrite, sizeof(char), fieldSizes[i], file);
    }
    lastOffset += totalSize;
}

int *Schema::insert(FDPair *data, int numToInsert) {
    int result[numToInsert];
    for (int i = 0; i < numToInsert; i++) {
        int offset = bpt.insert(data[i], lastOffset);
        if (offset >= 0) {
            writeToFile(data[i]);
        }
        result[i] = offset;
    }
    return result;
}

int *Schema::update(FDTriplet key, FDPair data) {
    KeyList *curList = keyList;
    int *toUpdate = nullptr;
    while (curList != nullptr) {
        if (containsField(key, curList->keys[0])) {
            toUpdate = curList->bpt.find(key);
        }
        curList = curList->next;
    }

    int *result;
    if (toUpdate == nullptr) {
        // Brute force search.
        // Update file.
        // Remove from every bpt.
        // Add to file.
        // Add to every bpt.
        // Update result.
    } else {
        result = malloc(sizeof(int) * (1 + toUpdate[0]));
        result[0] = toUpdate[0];
        for (int i = 0; i < toUpdate[0]; i++) {
            // Update file.
            // Remove from every bpt.
            // Add to file.
            // Add to every bpt.
            // Update result.
        }
    }

    return result;
}

int main() {
    
    return 0;
}

// class Schema {
//     private: 
//         char *schemaName;

//         char **fieldNames;
//         char **fieldTypes;
//         int *fieldSizes;
//         int numFields;

//         FILE *file;
//         int lastOffset;
    
//         BPT *bpt;
    
//     public:
//         Schema(char *schemaName, char **fieldNames, char **fieldTypes, int *fieldSizes, int numFields);

//         int insert(FDPair *data);
//         int *update(FDPair *key, FDPair *data);
//         int *find(FDPair keys);
//         int *remove(FDPair *key);
// };