#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef double stackData_t;
#define DATAVALUE 100
const int CANVAL = 666;

typedef struct Stack {
    int cannaryIn;
    stackData_t data[DATAVALUE];
    int counter;
    int cannaryOut;
} stack_t;

void Push               (stack_t* stack, stackData_t newData);
void StackConstructor   (stack_t** stack);
void Pop                (stack_t* stack, double* outBuf);
int  StackOK            (const stack_t* stack);

int main()
{
    stack_t* newStack;
    StackConstructor(&newStack);
    printf("counter = %i\ncannaryIn = %i, cannaryOut = %i\n", newStack->counter, newStack->cannaryIn, newStack->cannaryOut);
    Push(newStack, 228);
    printf("data = %lg\n", newStack->data[newStack->counter - 1]);
    double* outData;
    Pop(newStack, outData);
    printf("Poped data = %lg\nstackOk = %i\n", *outData, StackOK(newStack));
    
    return 0;
}

void StackConstructor (stack_t** stack) {
    *stack = (stack_t*) calloc(1, sizeof(stack_t));
    //assert(StackOK((*stack)));
    ((*(stack))->cannaryIn) = CANVAL;
    ((*(stack))->cannaryOut) = CANVAL;
}

void Push (stack_t* stack, stackData_t newData) {
    assert(StackOK(stack));
    stack -> data[stack->counter++] = newData;
    assert(StackOK(stack));
}

void Pop (stack_t* stack, double* outBuf) {
    assert(StackOK(stack));
    *(outBuf) = stack->data[--(stack->counter)];    
    assert(StackOK(stack));
}

int StackOK (const stack_t* stack) {
    return stack && stack->data && stack->counter >= 0 && stack->counter < DATAVALUE
            && stack->cannaryIn == CANVAL && stack->cannaryOut == CANVAL;
}

/*void DumpStack (const stack_t* stack) {
    FILE* dumpFile = fopen("Stack_Dump.txt", "w");

}*/
