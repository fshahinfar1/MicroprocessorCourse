# Prime number function
These inline assembly codes will check if the given parameter is a prime    
number or not

# Note
This code is writen for 32-bit system

# Todo
Use `sqrt(parameter) +1 ` as limit instead of `parameter`. it will be much    
more efficient.

# PrimeFunc.c
<B>Setting up:</B>
At first push EBP register to stack (memory). This will enable us to set    
base pointer back to what it was just before returning to caller function.    
Then ESP is copied to EBP. This way EBP can be used to address prameters    
and local variables defined. ESP can not be used because it will vary during    
the procedure. After setting up EBP, 12 is subtracted from ESP and ESP is    
updated. This action is researving 12 byte of memory for local variables    
used in this procedure.    
<B>Access arguments:</B>
In 32-bit systems arguments are passed in stack. Caller function push them    
to stack with order of right to left. It first pushes the right most argument    
and continues. this action means that callee function can find arguments by    
accessing stack according to passed arguments size and knowing that stack pointer   
is pointing to the return address when callee function is started.    
When execution of callee function is started stack look like the shape below.
<pre>
|             |
---------------
|             |  <== SP
---------------
| ret address |  // in 32 bit system it has a size of 4 byte
---------------
|    arg1     |
---------------
|      .      |
|      .      |
|      .      |
---------------
|    argN     |
---------------
</pre>

Now after pushing BP to stack and updating BP in last section, we can access    
the passed argument by addressing it with `BP + 8`. Four byte for value of BP    
pushed to stack and four byte four return address so eight byte before where    
BP is now, will be our parameter.



# Compilling
gcc -m32 -Wall -o [output] [src]
