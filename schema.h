using namespace std;

class Schema {
    private: 
        const char *schemaName;
        
        const char *fieldNames[];
        const char *fieldTypes[];
        int numFields;

        int rowSize;
        int numRows;

        LinkedList *keyList;

        PagedFileManager *fileManager;
        int write(FDPair data);
        int write(FDPair data, int offset);
        FDPair read(int offset);

        string rowToString(FDPair data);
        FDPair stringToRow(string buf);

        void updateBPTs(FDTriplet changes, FDPair oldPair, FDPair newPair, int offset);
        void removeFromBPTs(FDPair pair, int offset);
        LinkedList *findBPT(string[] fields, int numFields);
    
    public:
        Schema(string schemaName, FDPair fieldNamesAndTypes);

        int insert(FDPair[] data, int numToInsert);
        int update(FDTriplet tests, FDTriplet changes);
        LinkedList *find(FDTriplet tests);
        int remove(FDTriplet tests);

        bool isEmpty();
        int size();

        bool setKey(string[] keys, int numKeys);
};