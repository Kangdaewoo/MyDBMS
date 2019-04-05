#include "schema.h"
#include <iostream>


Schema::Schema(char *schemaName, FDPair fieldNamesAndTypes, int *fieldSizes): numFields(fieldNamesAndTypes.numFields) {
    schemaName = malloc(sizeof(char) * strlen(schemaName));
    strcpy(this->schemaName, schemaName);

    this->fieldNames = malloc(sizeof(char *) * numFields);
    this->fieldTypes = malloc(sizeof(char *) * numFields);
    this->fieldSizes = malloc(sizeof(int) * numFields);

    rowSize = 0;
    for (int i = 0; i < numFields; i++) {
        this->fieldNames[i] = malloc(sizeof(char) * (strlen(fieldNamesAndTypes.fields[i]) + 1));
        strcpy(this->fieldNames[i], fieldNamesAndTypes.fields[i]);

        this->fieldTypes[i] = malloc(sizeof(char) * (strlen(fieldNamesAndTypes.data[i]) + 1));
        strcpy(this->fieldTypes[i], fieldNamesAndTypes.data[i]);

        this->fieldSizes[i] = fieldSizes[i];
        rowSize += fieldSizes[i];
    }
    numRows = 0;

    file = fopen(this->schemaName, "w");
    keyList = nullptr;
}

bool Schema::isEmpty() {
    return numRows == 0;
}

int Schema::size() {
    return numRows * rowSize;
}

KeyList *Schema::containsKey(char **keys, int numKeys) {
    KeyList *temp = keyList;
    while (temp != nullptr) {
        bool found = false;
        for (int i = 0; i < numKeys; i++) {
            for (int j = 0; j < temp->numKeys; j++) {
                if (strcmp(keys[i], temp->keys[j]) != 0) {
                    found = false;
                    break;
                }
                found = true;
            }
            if (!found) {
                break;
            }
        }
        if (found) {
            return temp;
        }
        temp = temp->next;
    }
    return nullptr;
}

bool Schema::setKeys(char **keys, int numKeys) {
    if (containsKey(keys, numKeys) != nullptr) {
        return false;
    }

    KeyList *newKeyList = malloc(sizeof(KeyList));
    newKeyList->keys = malloc(sizeof(char *) * numKeys);
    for (int i = 0; i < numKeys; i++) {
        newKeyList->keys[i] = malloc(sizeof(char) * (strlen(keys[i]) + 1));
        strcpy(newKeyList->keys[i], keys[i]);
    }
    newKeyList->numKeys = numKeys;
    newKeyList->bpt = BPT() // TO DO.

    newKeyList->next = keyList;
    keyList = newKeyList;
    return true;
}

void rowToChar(FDPair data, char *buf) {
    memset(buf, '\0', sizeof(char) * (rowSize + 1));
    int cur = 0;
    for (int i = 0; i < numFields; i++) {
        int index = find(data.fields, data.numFields, fieldNames[i]);
        if (index == -1) {
            // TO DO: throw an error.
            return;
        }
        strcpy(buf + cur, data.fields[index]);
        cur += fieldSizes[i];
    }
}

void charToRow(char *buf, FDPair *data) {
    int cur = 0;
    for (int i = 0; i < numFields; i++) {
        memset(data->data[i], '\0', sizeof(char) * (fieldSizes[i] + 1));
        strncpy(data->data[i], buf + cur, fieldSizes[i]);
    }
}

int Schema::write(FDPair data) {
    // TO DO: For every write to file needs to acquire lock.
    char buf[rowSize + 1];
    rowToChar(data, buf);
    numRows++;
    return writeToFile(buf);
}

int *Schema::insert(FDPair *data, int numToInsert) {
    int result[numToInsert];
    for (int i = 0; i < numToInsert; i++) {
        bool isDuplicate = false;
        // TO DO: Check primary key
        if (!isDuplicate) {
            int offset = write(data[i]);
            KeyList *temp = keyList;
            while (temp != nullptr) {
                temp.bpt->insert(data[i], offset);
                temp = temp->next;
            }
            result[i] = offset;
        } else {
            result[i] = -1;
        }
    }
    return result;
}

int *Schema::update(FDTriplet key, FDPair data) {
    KeyList *curList = keyList;
    int *toUpdate = nullptr;
    while (curList != nullptr) {
        if (contains(key.fields, key.numFields, curList->keys[0])) {
            // TO DO: optimization, find the keyset that contains the most key
            // But is this really necessary?
            toUpdate = curList->bpt->find(key);
            break;
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