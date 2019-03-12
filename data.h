#if !defined(SHORT_LENGTH) || !defined(LONG_LENGTH)
#define SHORT_LENGTH 20
#define LONG_LENGTH 200
#endif

typedef struct {
    char firstName[SHORT_LENGTH];
    char lastName[SHORT_LENGTH];
    char dob[6];
    char studentNum[10];
} Student;

typedef struct {
    char courseName[SHORT_LENGTH];
    char description[LONG_LENGTH];
} Course;