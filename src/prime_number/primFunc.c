#include <stdio.h>
#include <stdlib.h>



int is_prime(int);

int main () {
    
    int val;
    printf("Input: ");
    scanf("%d", &val);   
    printf("Value: %d\n",is_prime(val)); 
    if (is_prime(val)) {
        printf("It's a prime number\n");
    } else {
        printf("It isn't a prime number\n");
    }

    return 0;
}


/*
 * is_prime(int)
 * 32 bit inline assembly function for prime number
 * in 32 bit assembly parameters are passed on 
 * stack (on 64 bit it is not always true)
 *
 * on 32-bit systems address are 4 bytes
 *
 * note: stack is growing by decreasing ESP
 */
asm (
        ".text                          \n\t"
        ".global is_prime               \n\t"
        ".type is_prime, @function      \n\t"
        "is_prime:                      \n\t"

        "pushl %ebp                     \n\t"
        "movl %esp, %ebp                \n\t"

        "subl $12, %esp                  \n\t"  // local variable reservance

        "movl 8(%ebp), %eax             \n\t"  // move parameter to EAX
        "movl %eax, -4(%ebp)            \n\t"  // move parameter to local var
        "movl %eax, -8(%ebp)            \n\t"  // move limit to local var (todo: use sqrt)

        "cmpl $3, %eax                  \n\t"
        "jl false                       \n\t"

        "movl $2, -12(%ebp)             \n\t"  // initialize counter
        
        "movl $0, %edx                  \n\t"  // sign extend EDX:EAX
        "divl -12(%ebp)                 \n\t"
        "test %edx, %edx                \n\t"  // check if EDX is zero
        "jz false                       \n\t"
        "incl -12(%ebp)                 \n\t"
        "mov -8(%ebp), %eax             \n\t"
        "cmpl -12(%ebp), %eax           \n\t"
        "jle true                       \n\t"
        "movl -4(%ebp), %eax            \n\t"

        "check_module:                  \n\t"
        "movl $0, %edx                  \n\t"  // sign extend EDX:EAX
        "divl -12(%ebp)                 \n\t"
        "test %edx, %edx                \n\t"  // check if EDX is zero
        "jz false                       \n\t"
        "incl -12(%ebp)                 \n\t"
        "incl -12(%ebp)                 \n\t"
        "mov -8(%ebp), %eax             \n\t"
        "cmpl -12(%ebp), %eax           \n\t"
        "jle true                       \n\t"
        "movl -4(%ebp), %eax            \n\t"
        "jmp check_module               \n\t"

        "false:                         \n\t"
        "movl $0, %eax                  \n\t"
        "leave                          \n\t"
        "ret                            \n\t"
        
        "true:                          \n\t"
        "movl $1, %eax                  \n\t"
        "leave                          \n\t"
        "ret                            \n\t");
