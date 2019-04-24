#include "schema.h"

using namespace std;


int Schema::write(FDPair data) {
    string row = rowToString(data);
    return fileManager->write(row);
}

int Schema::write(FDPair data, int offset) {
    string row = rowToString(data);
    return fileManager->write(row, offset);
}

Schema::Schema(string schemaName, FDPair fieldNamesAndTypes) {
    this->schemaName = schemaName.c_str();

    numFields = fieldNamesAndTypes.numFields;
    fieldNames = (const char **) malloc(sizeof(char *) * numFields);
    fieldTypes = (const char **) malloc(sizeof(char *) * numFields);
    rowSize = 0;
    for (int i = 0; i < numFields; i++) {
        fieldNames[i] = fieldNamesAndTypes.fields[i].c_str();
        fieldTypes[i] = fieldNamesAndTypes.data[i].c_str();
        rowSize += getTypeSize(fieldNamesAndTypes.data[i]);
    }
    numRows = 0;

    keyList = nullptr;

    fileManager = PagedFileManager(schemaName, rowSize * 1000, rowSize);
}

int Schema::insert(FDPair[] data, int numToInsert) {
    result = 0;
    for (int i = 0; i < numToInsert; i++) {
        // TO DO: Check for duplication.

        string buf = rowToString(data[i]);
        int offset = fileManager->write(buf);
        LinkedList *temp = keyList;
        while (temp != nullptr) {
            BPT *bpt = (BPT *) temp->ptr;
            bpt->insert(data[i], offset);
            temp = temp->next;
        }
        numRows++;
        result++;
    }
    return result;
}

void Schema::updateBPTs(FDTriplet changes, FDPair oldPair, FDPair newPair, int offset) {
    LinkedList *temp = keyList;
    while (temp != nullptr) {
        BPT *bpt = (BPT *) temp->ptr;
        if (bpt->containsKey(changes.fields, changes.numFields)) {
            bpt->remove(oldPair, offset);
            bpt->insert(newPair, offset);
        }
        temp = temp->next;
    }
}

LinkedList *Schema::findBPT(string[] fields, int numFields) {
    LinkedList *temp = keyList;
    LinkedList *toReturn = nullptr;
    int max = 0;
    while (temp != nullptr) {
        BPT *bpt = (BPT *) temp->ptr;
        int howMany = bpt->containsKey(fields, numFields);
        if (howMany > max) {
            max = howMany;
            toReturn = temp;
        }
        temp = temp->next;
    }
    return toReturn;
}

int Schema::update(FDTriplet tests, FDTriplet changes) {
    LinkedList *toUpdate = find(tests);
    while (toUpdate != nullptr) {
        FDPair oldPair = *((FDPair *) toUpdate->ptr);
        FDPair newPair = applyChanges(oldPair, changes);
        write(newPair, toUpdate->numFields);
        updateBPTs(changes, oldPair, newPair, offset);

        LinkedList *temp = toUpdate;
        toUpdate = toUpdate->next;
        free(temp->ptr);
        free(temp);

        result++;
    }
    return result;
}

FDPair Schema::read(int offset) {
    return stringToRow(fileManager->read(offset));
}

LinkedList *Schema::find(FDTriplet tests) {
    LinkedList *result = nullptr;
    LinkedList *temp = findBPT(tests.fields, tests.numFields);
    if (temp == nullptr) {
        fileManager->setIterator();
        while (fileManager->hasNext()) {
            string row;
            int offset = fileManager->next(&row);

            FDPair p = stringToRow(row);
            if (!satisfy(tests, p)) {
                continue;
            }

            temp = (LinkedList *) malloc(sizeof(LinkedList));
            FDPair *pair = (FDPair *) malloc(sizeof(FDPair));
            *pair = p;
            pair->numFields = offset;
            temp->ptr = pair;
            temp->next = result;
            result = temp;
        }
    } else {
        int[] toUpdate = ((BPT *) temp->ptr)->find(tests);
        for (int i = 0; i < toUpdate[0]; i++) {
            FDPair p = read(toUpdate[i + 1]);
            if (!satisfy(tests, p)) {
                continue;
            }

            temp = (LinkedList *) malloc(sizeof(LinkedList));
            FDPair *pair = (FDPair *) malloc(sizeof(FDPair));
            *pair = p;
            pair->numFields = toUpdate[i + 1];
            temp->ptr = pair;
            temp->next = result;
            result = temp;
        }

        for (int i = 0; i < toUpdate[0] + 1; i++) {
            free(toUpdate[i]);
        }
    }
    return result;
}

void Schema::removeFromBPTs(FDPair pair, int offset) {
    LinkedList *temp = keyList;
    while (temp != nullptr) {
        BPT *bpt = (BPT *) temp->ptr;
        bpt->remove(pair, offset);
        temp = temp->next;
    }
}

int Schema::remove(FDTriplet tests) {
    LinkedList *toRemove = find(tests);
    while (toRemove != nullptr) {
        FDPair pair = *((FDPair *) toRemove->ptr);
        fileManager->remove(toRemove->numFields);
        removeFromBPTs(pair, offset);

        LinkedList *temp = toRemove;
        toRemove = toRemove->next;
        free(temp->ptr);
        free(temp);

        numRows--;
        result++;
    }
    return result;
}

bool Schema::isEmpty() {
    return numRows == 0;
}

int Schema::size() {
    return numRows;
}

bool Schema::setKey(string[] keys, int numKeys) {
    LinkedList *temp = keyList;
    while (temp != nullptr) {
        BPT *bpt = (BPT *) temp->ptr;
        if (bpt->equals(keys, numKeys)) {
            return false;
        }
        temp = temp->next;
    }

    temp = (LinkedList *) malloc(sizeof(LinkedList));
    FDPair newKeys;
    newKeys.fields = (string *) malloc(sizeof(string) * numKeys);
    newKeys.types = (string *) malloc(sizeof(string) * numKeys);
    for (int i = 0; i < numKeys; i++) {
        int index = findField(fieldNames, keys[i], numFields);
        newKeys.fields[i] = keys[i];
        newKeys.data[i] = fieldTypes[index];
    }

    BPT *newBPT(schemaName, newKeys);
    temp->ptr = (void *) newBPT;
    temp->next = keyList;
    keyList = temp;
}

string Schema::rowToString(FDPair data) {
    char buf[rowSize + 1];
    memset(buf, '\0', rowSize + 1);
    int written = 0;
    for (int i = 0; i < numFields; i++) {
        int index = findField(data.fields, fieldNames[i], numFields);
        strcpy(&buf[written], data.fields[index]);
        written += getTypeSize(fieldTypes[i]);
    }
    return string(buf, rowSize);
}

FDPair stringToRow(string buf) {
    FDPair toReturn;
    toReturn.fields = (string *) malloc(sizeof(string) * numFields);
    toReturn.data = (string *) malloc(sizeof(string) * numFields);
    int read = 0;
    for (int i = 0; i < numFields; i++) {
        string field(fieldNames[i]);
        toReturn.fields[i] = fieldNames[i];
        int size = getTypeSize(fieldTypes[i])
        toReturn.data[i] = buf.substr(read, size);
        read += getTypeSize(fieldTypes[i]);
    }
    return toReturn;
}