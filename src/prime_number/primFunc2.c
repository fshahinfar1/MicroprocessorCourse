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
       
        "pushfl                         \n\t"
        "pushl %ebx                     \n\t"
        "pushl %ecx                     \n\t"
        "pushl %ebp                     \n\t"
        "movl %esp, %ebp                \n\t"
        "movl 20(%ebp), %ebx            \n\t"

        "cmpl $3, %ebx                  \n\t"
        "jl false                       \n\t"  // if less than or equal to three then false
        
        "subl $4, %esp                  \n\t"  // reserve space for variable on stack (4 byte)
        "movl %ebx, -4(%ebp)            \n\t"  // move limit to reserve space (todo: use, sqrt.find a way to calculate sqrt.)
       
        "movl $2, %ecx                  \n\t"  // initialize a counter (ECX)

        "movl %ebx, %eax                \n\t"  // set numerator
        "cdq                            \n\t"
        "divl %ecx                      \n\t"  // divide parameter by two
        "test %edx, %edx                \n\t"  // check if remainder is zero
        "jz false                       \n\t"  // if zero then false
        "inc %ecx                       \n\t"
        "cmp %ecx, -4(%ebp)             \n\t"  // check if counter has passed the limit
        "jle true                       \n\t"  // then it is a prime number
        
        "check_module:                  \n\t"
        "movl %ebx, %eax                \n\t"  // set numerator
        "cdq                            \n\t"
        "divl %ecx                      \n\t"  // divide parameter by two
        "test %edx, %edx                \n\t"  // check if remainder is zero
        "jz false                       \n\t"  // if zero then false
        "inc %ecx                       \n\t"
        "inc %ecx                       \n\t"
        "cmp %ecx, -4(%ebp)             \n\t"  // check if counter has passed the limit
        "jle true                       \n\t"  // then it is a prime number
        "jmp check_module               \n\t"
        
        "false:                         \n\t"  // not prime
        "movl $0, %eax                  \n\t"
        "movl %ebp, %esp                \n\t"
        "popl %ebp                      \n\t"
        "popl %ecx                      \n\t"
        "popl %ebx                      \n\t"
        "popfl                          \n\t"
        "ret                            \n\t"

        "true:                          \n\t"
        "movl $1, %eax                  \n\t"
        "movl %ebp, %esp                \n\t"
        "popl %ebp                      \n\t"
        "popl %ecx                      \n\t"
        "popl %ebx                      \n\t"
        "popfl                          \n\t"
        "ret                            \n\t");
