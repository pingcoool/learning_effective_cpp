#include <iostream>

#define CALL_WITH_MAX(a, b) print_max((a)>(b)?(a):(b))

void print_max(int num) {
    std::cout << "max num is: " << num << std::endl;
}
// GCC does not inline any functions when not optimizing unless you specify the ‘always_inline’ attribute for the function.
template<class T>
inline void callWithMax(const T &a, const T &b) __attribute__((always_inline));
template<class T>
inline void callWithMax(const T &a, const T &b) {
    print_max(a > b ? a : b);
}


int using_macro() {
    int a = 5;
    int b = 0;
    CALL_WITH_MAX(++a, b); // a 被累加两次
    printf("a = %d\n", a); // a = 7
    CALL_WITH_MAX(++a, b + 10); // a 被累加一次
    printf("a = %d\n", a); // a = 8
    return 0;
}

int using_template_inline(){
    int a = 5;
    int b = 0;
    callWithMax(++a, b); // a 被累加一次
    printf("a = %d\n", a); // a = 6
    callWithMax(++a, b + 10); // a 被累加一次
    printf("a = %d\n", a); // a = 7
    return 0;
}

int main(){
    using_macro();
    using_template_inline();
    return 0;
}

/*
disadvantages_of_macros`using_macro:
    0x105550120 <+0>:   pushq  %rbp
    0x105550121 <+1>:   movq   %rsp, %rbp
    0x105550124 <+4>:   subq   $0x10, %rsp
->  0x105550128 <+8>:   movl   $0x5, -0x4(%rbp)
    0x10555012f <+15>:  movl   $0x0, -0x8(%rbp)
    0x105550136 <+22>:  movl   -0x4(%rbp), %eax
    0x105550139 <+25>:  addl   $0x1, %eax
    0x10555013c <+28>:  movl   %eax, -0x4(%rbp)
    0x10555013f <+31>:  cmpl   -0x8(%rbp), %eax
    0x105550142 <+34>:  jle    0x105550159               ; <+57> at 01_disadvantages_of_macros.cpp:18:5
    0x105550148 <+40>:  movl   -0x4(%rbp), %eax
    0x10555014b <+43>:  addl   $0x1, %eax
    0x10555014e <+46>:  movl   %eax, -0x4(%rbp)
    0x105550151 <+49>:  movl   %eax, -0xc(%rbp)
    0x105550154 <+52>:  jmp    0x10555015f               ; <+63> at 01_disadvantages_of_macros.cpp
    0x105550159 <+57>:  movl   -0x8(%rbp), %eax
    0x10555015c <+60>:  movl   %eax, -0xc(%rbp)
    0x10555015f <+63>:  movl   -0xc(%rbp), %edi
    0x105550162 <+66>:  callq  0x105550030               ; print_max at 01_disadvantages_of_macros.cpp:5
    0x105550167 <+71>:  movl   -0x4(%rbp), %esi
    0x10555016a <+74>:  leaq   0xde0(%rip), %rdi         ; "a = %d\n"
    0x105550171 <+81>:  movb   $0x0, %al
    0x105550173 <+83>:  callq  0x105550e08               ; symbol stub for: printf
    0x105550178 <+88>:  movl   -0x4(%rbp), %eax
    0x10555017b <+91>:  addl   $0x1, %eax
    0x10555017e <+94>:  movl   %eax, -0x4(%rbp)
    0x105550181 <+97>:  movl   -0x8(%rbp), %ecx
    0x105550184 <+100>: addl   $0xa, %ecx
    0x105550187 <+103>: cmpl   %ecx, %eax
    0x105550189 <+105>: jle    0x1055501a0               ; <+128> at 01_disadvantages_of_macros.cpp:20:5
    0x10555018f <+111>: movl   -0x4(%rbp), %eax
    0x105550192 <+114>: addl   $0x1, %eax
    0x105550195 <+117>: movl   %eax, -0x4(%rbp)
    0x105550198 <+120>: movl   %eax, -0x10(%rbp)
    0x10555019b <+123>: jmp    0x1055501a9               ; <+137> at 01_disadvantages_of_macros.cpp
    0x1055501a0 <+128>: movl   -0x8(%rbp), %eax
    0x1055501a3 <+131>: addl   $0xa, %eax
    0x1055501a6 <+134>: movl   %eax, -0x10(%rbp)
    0x1055501a9 <+137>: movl   -0x10(%rbp), %edi
    0x1055501ac <+140>: callq  0x105550030               ; print_max at 01_disadvantages_of_macros.cpp:5
    0x1055501b1 <+145>: movl   -0x4(%rbp), %esi
    0x1055501b4 <+148>: leaq   0xd96(%rip), %rdi         ; "a = %d\n"
    0x1055501bb <+155>: movb   $0x0, %al
    0x1055501bd <+157>: callq  0x105550e08               ; symbol stub for: printf
    0x1055501c2 <+162>: xorl   %eax, %eax
    0x1055501c4 <+164>: addq   $0x10, %rsp
    0x1055501c8 <+168>: popq   %rbp
    0x1055501c9 <+169>: retq
    0x1055501ca <+170>: nopw   (%rax,%rax)
 */

/*
disadvantages_of_macros`using_template_inline: (without __attribute__((always_inline)))
    0x1035541d0 <+0>:   pushq  %rbp
    0x1035541d1 <+1>:   movq   %rsp, %rbp
    0x1035541d4 <+4>:   subq   $0x10, %rsp
->  0x1035541d8 <+8>:   movl   $0x5, -0x4(%rbp)
    0x1035541df <+15>:  movl   $0x0, -0x8(%rbp)
    0x1035541e6 <+22>:  movl   -0x4(%rbp), %eax
    0x1035541e9 <+25>:  addl   $0x1, %eax
    0x1035541ec <+28>:  movl   %eax, -0x4(%rbp)
    0x1035541ef <+31>:  leaq   -0x4(%rbp), %rdi
    0x1035541f3 <+35>:  leaq   -0x8(%rbp), %rsi
    0x1035541f7 <+39>:  callq  0x103554d8a               ; symbol stub for: void callWithMax<int>(int const&, int const&)
    0x1035541fc <+44>:  movl   -0x4(%rbp), %esi
    0x1035541ff <+47>:  leaq   0xd4b(%rip), %rdi         ; "a = %d\n"
    0x103554206 <+54>:  movb   $0x0, %al
    0x103554208 <+56>:  callq  0x103554e08               ; symbol stub for: printf
    0x10355420d <+61>:  movl   -0x4(%rbp), %eax
    0x103554210 <+64>:  addl   $0x1, %eax
    0x103554213 <+67>:  movl   %eax, -0x4(%rbp)
    0x103554216 <+70>:  movl   -0x8(%rbp), %eax
    0x103554219 <+73>:  addl   $0xa, %eax
    0x10355421c <+76>:  movl   %eax, -0xc(%rbp)
    0x10355421f <+79>:  leaq   -0x4(%rbp), %rdi
    0x103554223 <+83>:  leaq   -0xc(%rbp), %rsi
    0x103554227 <+87>:  callq  0x103554d8a               ; symbol stub for: void callWithMax<int>(int const&, int const&)
    0x10355422c <+92>:  movl   -0x4(%rbp), %esi
    0x10355422f <+95>:  leaq   0xd1b(%rip), %rdi         ; "a = %d\n"
    0x103554236 <+102>: movb   $0x0, %al
    0x103554238 <+104>: callq  0x103554e08               ; symbol stub for: printf
    0x10355423d <+109>: xorl   %eax, %eax
    0x10355423f <+111>: addq   $0x10, %rsp
    0x103554243 <+115>: popq   %rbp
    0x103554244 <+116>: retq
    0x103554245 <+117>: nopw   %cs:(%rax,%rax)
    0x10355424f <+127>: nop
 */

 /*
 disadvantages_of_macros`using_template_inline: (with __attribute__((always_inline)))
     0x10d0fa1c0 <+0>:   pushq  %rbp
     0x10d0fa1c1 <+1>:   movq   %rsp, %rbp
     0x10d0fa1c4 <+4>:   subq   $0x40, %rsp
 ->  0x10d0fa1c8 <+8>:   movl   $0x5, -0x24(%rbp)
     0x10d0fa1cf <+15>:  movl   $0x0, -0x28(%rbp)
     0x10d0fa1d6 <+22>:  movl   -0x24(%rbp), %eax
     0x10d0fa1d9 <+25>:  addl   $0x1, %eax
     0x10d0fa1dc <+28>:  movl   %eax, -0x24(%rbp)
     0x10d0fa1df <+31>:  leaq   -0x24(%rbp), %rax
     0x10d0fa1e3 <+35>:  movq   %rax, -0x18(%rbp)
     0x10d0fa1e7 <+39>:  leaq   -0x28(%rbp), %rax
     0x10d0fa1eb <+43>:  movq   %rax, -0x20(%rbp)
     0x10d0fa1ef <+47>:  movq   -0x18(%rbp), %rax
     0x10d0fa1f3 <+51>:  movl   (%rax), %eax
     0x10d0fa1f5 <+53>:  movq   -0x20(%rbp), %rcx
     0x10d0fa1f9 <+57>:  cmpl   (%rcx), %eax
     0x10d0fa1fb <+59>:  jle    0x10d0fa20f               ; <+79> [inlined] void callWithMax<int>(int const&, int const&) + 32 at 01_disadvantages_of_macros.cpp:30
     0x10d0fa201 <+65>:  movq   -0x18(%rbp), %rax
     0x10d0fa205 <+69>:  movl   (%rax), %eax
     0x10d0fa207 <+71>:  movl   %eax, -0x30(%rbp)
     0x10d0fa20a <+74>:  jmp    0x10d0fa218               ; <+88> [inlined] void callWithMax<int>(int const&, int const&) + 41 at 01_disadvantages_of_macros.cpp:30
     0x10d0fa20f <+79>:  movq   -0x20(%rbp), %rax
     0x10d0fa213 <+83>:  movl   (%rax), %eax
     0x10d0fa215 <+85>:  movl   %eax, -0x30(%rbp)
     0x10d0fa218 <+88>:  movl   -0x30(%rbp), %edi
     0x10d0fa21b <+91>:  callq  0x10d0fa020               ; print_max at 01_disadvantages_of_macros.cpp:5
     0x10d0fa220 <+96>:  movl   -0x24(%rbp), %esi
     0x10d0fa223 <+99>:  leaq   0xd23(%rip), %rdi         ; "a = %d\n"
     0x10d0fa22a <+106>: movb   $0x0, %al
     0x10d0fa22c <+108>: callq  0x10d0fae02               ; symbol stub for: printf
     0x10d0fa231 <+113>: movl   -0x24(%rbp), %eax
     0x10d0fa234 <+116>: addl   $0x1, %eax
     0x10d0fa237 <+119>: movl   %eax, -0x24(%rbp)
     0x10d0fa23a <+122>: movl   -0x28(%rbp), %eax
     0x10d0fa23d <+125>: addl   $0xa, %eax
     0x10d0fa240 <+128>: movl   %eax, -0x2c(%rbp)
     0x10d0fa243 <+131>: leaq   -0x24(%rbp), %rax
     0x10d0fa247 <+135>: movq   %rax, -0x8(%rbp)
     0x10d0fa24b <+139>: leaq   -0x2c(%rbp), %rax
     0x10d0fa24f <+143>: movq   %rax, -0x10(%rbp)
     0x10d0fa253 <+147>: movq   -0x8(%rbp), %rax
     0x10d0fa257 <+151>: movl   (%rax), %eax
     0x10d0fa259 <+153>: movq   -0x10(%rbp), %rcx
     0x10d0fa25d <+157>: cmpl   (%rcx), %eax
     0x10d0fa25f <+159>: jle    0x10d0fa273               ; <+179> [inlined] void callWithMax<int>(int const&, int const&) + 32 at 01_disadvantages_of_macros.cpp:32
     0x10d0fa265 <+165>: movq   -0x8(%rbp), %rax
     0x10d0fa269 <+169>: movl   (%rax), %eax
     0x10d0fa26b <+171>: movl   %eax, -0x34(%rbp)
     0x10d0fa26e <+174>: jmp    0x10d0fa27c               ; <+188> [inlined] void callWithMax<int>(int const&, int const&) + 41 at 01_disadvantages_of_macros.cpp:32
     0x10d0fa273 <+179>: movq   -0x10(%rbp), %rax
     0x10d0fa277 <+183>: movl   (%rax), %eax
     0x10d0fa279 <+185>: movl   %eax, -0x34(%rbp)
     0x10d0fa27c <+188>: movl   -0x34(%rbp), %edi
     0x10d0fa27f <+191>: callq  0x10d0fa020               ; print_max at 01_disadvantages_of_macros.cpp:5
     0x10d0fa284 <+196>: movl   -0x24(%rbp), %esi
     0x10d0fa287 <+199>: leaq   0xcbf(%rip), %rdi         ; "a = %d\n"
     0x10d0fa28e <+206>: movb   $0x0, %al
     0x10d0fa290 <+208>: callq  0x10d0fae02               ; symbol stub for: printf
     0x10d0fa295 <+213>: xorl   %eax, %eax
     0x10d0fa297 <+215>: addq   $0x40, %rsp
     0x10d0fa29b <+219>: popq   %rbp
     0x10d0fa29c <+220>: retq

 */