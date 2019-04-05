PagedFileManager::PagedFileManager(char *name, int pageSize, int rowSize): rowSize(rowSize), pageSize(pageSize) {
    // TO DO: file consists of directory pages and data pages.
    file = fopen(name, "w+");

}

void PagedFileManager::write(char *data) {
    // TO DO: Find an empty slot
    fwrite(data, sizeof(char), rowSize, file);
}

bool PagedFileManager::setIterator() {

}

bool PagedFileManager::hasNext() {

}

void PagedFileManager::next(char *buf) {
    
}