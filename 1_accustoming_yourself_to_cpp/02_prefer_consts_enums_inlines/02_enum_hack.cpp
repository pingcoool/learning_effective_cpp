#include "02_enum_hack.h"
const int StudentStaticConst::s_max_class_size = 10;

int main() {
    StudentMacro s1;
    printf("s1.score[0]: %d\n", s1.score_array[0]);
    StudentConst s2;
    printf("s2.score[0]: %d\n", s2.score_array[0]);
    StudentStaticConst s3;
//    printf("s3.score[0]: %d\n", s3.score_array[0]); // error
    StudentEnumHack s4;
    printf("s4.score[0]: %d\n", s4.score_array[0]);
    return 0;
}