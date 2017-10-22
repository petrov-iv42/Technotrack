#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#define NDEBUG
#include <assert.h>

typedef double stackData_t;
const int CANVAL = 0xFD;

typedef struct Stack_ {
    int cannaryIn;
    int dataSize;
    stackData_t* data;
    int counter;
    int cannaryOut;
} stack_t;

void StackConstructor   (stack_t** Stack, int dataSize);
void StackDestructor    (stack_t** Stack);
void Push               (stack_t* Stack, stackData_t newData);
void Pop                (stack_t* Stack, double* outBuf);
int  StackOK            (const stack_t* Stack);
void _DumpStack         (const stack_t* Stack, const char* stackName);

#define StackDump(a) _DumpStack(a, #a)

#define Overthrow(a) {                                                  \
    if (!a##OK(a)) {                                                    \
        a##Dump(a);                                                     \
        fprintf(stderr,                                                 \
                "Error in %s structure. Dump was done\n", (char*)a);    \
        abort();                                                        \
    }                                                                   \
}

int main()
{
    stack_t* newStack;
    StackConstructor(&newStack, 100);
    printf("counter = %i\ncannaryIn = %i, cannaryOut = %i\n", newStack->counter, newStack->cannaryIn, newStack->cannaryOut);
    
    newStack->cannaryIn = 0;
    Push(newStack, 228);
    printf("data = %lg\n", newStack->data[newStack->counter - 1]);
    //StackDump(newStack);
    double* outData;
    Pop(newStack, outData);
    printf("Poped data = %lg\nstackOk = %i\n", *outData, StackOK(newStack));
    
    StackDestructor(&newStack);
    printf("After destruction StackOK = %i\n", StackOK(newStack));
    
    return 0;
}

void StackConstructor (stack_t** Stack_, int dataSize) {
    *Stack_ = (stack_t*) calloc(1, sizeof(stack_t));
    (*Stack_)->dataSize = dataSize;
    (*Stack_)->data = (stackData_t*) calloc(dataSize, sizeof(stackData_t));
    ((*Stack_)->cannaryIn) = CANVAL;
    ((*Stack_)->cannaryOut) = CANVAL;
    stack_t *Stack = *Stack_;
    Overthrow(Stack);
}

void StackDestructor (stack_t** Stack) {
    free((*Stack)->data);
    (*Stack)->data = NULL;
    (*Stack)->cannaryIn = INT_MAX;
    (*Stack)->cannaryOut = INT_MAX;
    (*Stack)->dataSize = INT_MAX;
    (*Stack)->counter = INT_MAX;
    free((*Stack));
    (*Stack) = NULL;
}

void Push (stack_t* Stack, stackData_t newData) {
    assert(StackOK(Stack));
    Overthrow(Stack);
    Stack -> data[Stack->counter++] = newData;
    assert(StackOK(Stack));
    Overthrow(Stack);
}

void Pop (stack_t* Stack, double* outBuf) {
    assert(StackOK(Stack));
    Overthrow(Stack);
    *(outBuf) = Stack->data[--(Stack->counter)];    
    assert(StackOK(Stack));
    Overthrow(Stack);
}

int StackOK (const stack_t* Stack) {
    return Stack && Stack->data && Stack->counter >= 0 && Stack->dataSize < INT_MAX 
            && Stack->dataSize > 0 && Stack->counter < Stack->dataSize
            && Stack->cannaryIn == CANVAL && Stack->cannaryOut == CANVAL;
}

void _DumpStack (const stack_t* Stack, const char* stackName) {
    FILE* dumpFile = fopen("Stack_Dump.txt", "a");
    time_t curTime;
    time(&curTime);
    fprintf(dumpFile, ">>>>>>>>>>Dump time: %s\n", ctime(&curTime));
    if (!StackOK(Stack)) {
        fprintf(dumpFile, "%s [%p] (ERROR!)\n", stackName, Stack);
    } else {
        fprintf(dumpFile, "%s [%p] (ok)\n{\n", stackName, Stack);
    }

    fprintf(dumpFile, "\tcannaryIn = %i;", Stack->cannaryIn);
    if (Stack->cannaryIn != CANVAL) {
        fprintf(dumpFile, " //WTF?");
    } 
    fprintf(dumpFile, "\n");

    fprintf(dumpFile, "\tdataSize = %i;", Stack->dataSize);
    if (Stack->dataSize <= 0 || Stack->dataSize >= INT_MAX) {
        fprintf(dumpFile, " //Uncommon value");
    }
    fprintf(dumpFile, "\n");
    
    fprintf(dumpFile, "\tcounter = %i;", Stack->counter);
    if (Stack->counter < 0 || Stack->counter >= INT_MAX) {
        fprintf(dumpFile, " //Uncommon value");
    }
    fprintf(dumpFile, "\n");
    
    fprintf(dumpFile, "\tdata [%i] [%p] = {\n", Stack->dataSize, Stack->data);
    for (int i = 0; i < Stack->dataSize; i++) {
        fprintf(dumpFile, "\t\t[%*i] = %lg;\n", 4, i, Stack->data[i]);
    }
    fprintf(dumpFile, "\t}\n");
    
    fprintf(dumpFile, "\tcannaryOut = %i;", Stack->cannaryOut);
    if (Stack->cannaryOut != CANVAL) {
        fprintf(dumpFile, " //WTF?");
    } 
    fprintf(dumpFile, "\n}\n<<<<<<<<<<\n");
}
