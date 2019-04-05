bool contains(char **fields, int numFields, char *field) {
    for (int i = 0; i < numFields; i++) {
        if (strcmp(field, fields[i]) == 0) {
            return true;
        }
    }
    return false;
}

int find(char **fields, int numFields, char *field) {
    for (int i = 0; i < numFields; i++) {
        if (strcmp(field, fields[i]) == 0) {
            return i;
        }
    }
    return -1;
}