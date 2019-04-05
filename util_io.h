#define PAGE_SIZE 


void writeToFile(FILE *file, char *data, int size);

class PagedFileManager {
    private:
        FILE *file;
        int rowSize;
        int pageSize;
        // TO DO: lock.
    
    public:
        PagedFileManager(char *name, int pageSize, int rowSize);
        void write(char *data);
        bool setIterator();
        bool hasNext();
        void next(char *buf);
};