#include "BPT.h"


using namespace std;


BPT::BPT(string schemaName, FDPair keyNamesAndTypes) {
    numKeys = keyNamesAndTypes.numFields;
    keyNames = (char **) malloc(sizeof(char *) * numFields);
    keyTypes = (char **) malloc(sizeof(char *) * numFields);
    rowSize = 0;
    for (int i = 0; i < numKeys; i++) {
        keyNames[0] = keyNamesAndTypes.fields[0].c_str();
        keyTypes[0] = keyNamesAndTypes.data[0].c_str();
        rowSize += getTypeSize(fieldNamesAndTypes.data[i]);
    }

    rowSize += rowSize * TREE_DEPTH + 1 + (TREE_DEPTH + 1) * 4 + 4 + 4;
    fileManager = PagedFileManager(schemaName, rowSize * 1000, rowSize);
    root = -1;
}

string BPT::rowToString(Node data) {
    char buf[rowSize + 1];
    memset(buf, '\0', rowSize + 1);
    int written = 0;
    strncpy(&buf[written], (char *) &data.isLeaf, 1);
    for (int j = 0; j < TREE_DEPTH; j++) {
        for (int i = 0; i < numKeys; i++) {
            strncpy(&buf[written], data.keys[numKeys * j + i].c_str());
            written += getTypeSize(keyTypes[i]);
        }
    }

    for (int i = 0; i < TREE_DEPTH + 1; i++) {
        strncpy(&buf[written], (char *) &data.children[i], 4);
        written += 4;
    }
    strncpy(&buf[written], (char *) &data.leftSibling, 4);
    written += 4;
    strncpy(&buf[written], (char *) &data.rightSibling, 4);

    return string(buf);
}

Node BPT::stringToRow(string buf) {
    Node toReturn;
    toReturn.keys = (string *) malloc(sizeof(string) * TREE_DEPTH);
    toReturn.children = (int *) malloc(sizeof(int) * (TREE_DEPTH + 1));
    int read = 0;
    strncpy((char *) &toReturn.isLeaf, buf.substr(read, 1).c_str(), 1);
    read += 1;
    for (int j = 0; j < TREE_DEPTH; j++) {
        for (int i = 0; i < numKeys; i++) {
            int size = getTypeSize(keyTypes[i]);
            toReturn.keys[numKeys * j + i] = buf.substr(read, size);
            read += size;
        }
    }

    for (int i = 0; i < TREE_DEPTH + 1; i++) {
        strncpy((char *) &toReturn.children[i], &buf[read], 4);
        read += 4;
    }
    strncpy((char *) &toReturn.leftSibling, &buf[read], 4);
    written += 4;
    strncpy((char *) &toReturn.rightSibling, &buf[read], 4);

    return toReturn;
}

void BPT::insert(FDPair data, int offset) {
}

int[] BPT::find(FDTriplet tests) {

}

void BPT::remove(FDPair pair, int offset) {

}

int BPT::containsKey(string[] fields, int numFields) {

}

bool BPT::equals(string[] fields, int numFields) {

}