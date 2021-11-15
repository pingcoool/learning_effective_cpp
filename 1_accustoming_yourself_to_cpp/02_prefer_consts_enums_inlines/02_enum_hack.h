#include <iostream>
#define MAX_CLASS_SIZE 10

// using macro
struct StudentMacro {
    int score_array[MAX_CLASS_SIZE]{};
};

// using const
const int max_class_size = 10;
struct StudentConst {
    int score_array[max_class_size]{};
};

// using static const
struct StudentStaticConst {
//    static const int s_max_class_size = 10; // 常量声明式, 部分旧编译器可能不支持这么做
    static const int s_max_class_size; // 部分旧编译器不支持常量声明式, 只能在 .cpp 文件中的定义式中赋初值
//    int score_array[s_max_class_size]{}; // error: Fields must have a constant size:
                                           // 'variable length array in structure' extension will never be supported
};

// using enum hack
struct StudentEnumHack {
    enum { e_max_class_size = 10 };     // 在使用旧编译器不支持 static 成员在声明式上获得初值时, 可以使用枚举类型保存初值.
    int score_array[e_max_class_size]{};
};
