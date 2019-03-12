#include <cstdio>


// mmap

class BPT {
    private:
    int end;
    FILE *file;
    int (*comparator) (void *d1, void *d2);
    
    public:
    BPT(FILE *indexFile, int (*f) (void *d1, void *d2)) {
        file = indexFile;
        comparator = f;
        end = 0;
    }

    int insert(void *d) {

    }

    int remove(void *d) {

    }

    void *findOne(void *d) {

    }

    void *findMany(void *small, void *large) {

    }
};


int main(int argc, char *argv[]) {
    
}