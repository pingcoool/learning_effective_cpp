条款 02: 尽量以 const, enum, inline 替换 #define
=========
在 c++ 中使用宏有以下缺点:
# 1. 宏在预处理阶段会被直接替换, 编译器不可见
预编译语句仅仅是简单的值代替，缺乏类型的检测机制。这样预处理语句就不能享受C++严格的类型检查的好处，从而可能成为引发一系列错误的隐患.
# 2. 一些使用宏定义的函数, 即便加上小括号, 仍然可能出现非预期的行为
```c++
#define CALL_WITH_MAX(a, b) print_max_num((a)>(b)?((a):(b)))
int using_macro() {
    int a = 5;
    int b = 0;
    CALL_WITH_MAX(++a, b); // a 被累加两次
    printf("a = %d\n", a); // a = 7
    CALL_WITH_MAX(++a, b + 10); // a 被累加一次
    printf("a = %d\n", a); // a = 8
    return 0;
}
``` 
上这段代码中, a 的递增次数竟然取决于比较结果. 这就是使用宏带来的风险.
<br>
一个非常有效的方法是将比较大小的逻辑使用 inline 函数实现:
```c++
// GCC does not inline any functions when not optimizing unless you specify the ‘always_inline’ attribute for the function.
template<class T>
inline void callWithMax(const T &a, const T &b) __attribute__((always_inline));

template<class T>
inline void callWithMax(const T &a, const T &b) {
    print_max(a > b ? a : b);
}
```
这个 template inline 能够起到与宏完全相同的功能, 却又没有宏那样的缺点: 它可以在编译阶段直接生成对应类型的代码, 而不是进行函数调用; 同时, 传入的参数不会向宏那样被简单地替换.
<br>
> 注意, 是否将函数进行内联取决于编译器的实现, inline 关键字只是提出内联建议.
> <br>
> 事实上, 通过查看汇编代码, 如果没有在函数声明时添加 `__attribute__((always_inline))`, 编译器仍然会把这个函数编译成非内联函数.

 # 3. const 的正确使用   
在使用 const 关键字替换字符串宏时, 需要在常量指针前后都加上 const:
```c++
#define MY_STR "my_string"
const char* const my_str = "my_string";
const std::string my_std_str("my_string");
```
上面三种常量字符串的定义是等价的, 推荐使用第三种方式.
 # 4. enum hack
当对象的成员变量中存在数组时, 数组在初始化时必须明确大小, 而其大小又需要以常量的形式进行表示时, 有以下可选办法:
## 1. 使用宏
```c++
 // using macro
struct StudentMacro {
int score_array[MAX_CLASS_SIZE]{};
};
```
## 2. 使用 const 变量
```c++
// using const
const int max_class_size = 10;
struct StudentConst {
int score_array[max_class_size]{};
};
```
## 3. 使用类内 static const
```c++
// using static const
struct StudentStaticConst {
    static const int s_max_class_size = 10; // 常量声明式, 部分旧编译器可能不支持这么做
    int score_array[s_max_class_size]{}; 
};
```
上面这个方式可能部分旧编译器并不支持, 它们可能只支持下面这种静态变量的声明和定义方式:
```c++
// 02_enum_hack.h
// using static const
struct StudentStaticConst {
    static const int s_max_class_size; // 部分旧编译器不支持常量声明式, 只能在 .cpp 文件中的定义式中赋初值
    int score_array[s_max_class_size]{}; // error: Fields must have a constant size:
                                           // 'variable length array in structure' extension will never be supported
};

// 02_enum_hack.cpp
const int StudentStaticConst::s_max_class_size = 10;

```
此时, 编译器将无法在编译期间获取到数组的大小.
<br>
可以采用下面这种 enum hack 方式保存数组大小:
```c++
// using enum hack
struct StudentEnumHack {
    enum { e_max_class_size = 10 };     // 在使用旧编译器不支持 static 成员在声明式上获得初值时, 可以使用枚举类型保存初值.
    int score_array[e_max_class_size]{};
};
```
enum hack 和宏的行为比较相像: 取一个 enum 或者宏的地址均不合法, 但是取一个 const 变量的地址是合法的; 当你想要约束用户不对这个 size 变量去地址时, 或许 enum hack 会是一个好办法.

请记住
========
## 对于单纯的常量, 最好以 const 对象或者 enum 替换 #define
## 对于形似函数的宏, 最好改用 inline 函数替换 #define