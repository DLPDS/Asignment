#include "types.h"
#include "stat.h"
#include "user.h"

#define NUM_STRUCTS 10

struct mystruct {
    int value1;
    int value2;
    char name[16];
};

int main(void) {

    struct mystruct struct_array[NUM_STRUCTS];
    struct mystruct *p;

    int return_val = mycall( NUM_STRUCTS*sizeof(struct mystruct), &struct_array);
    p = &struct_array[0];
//value 1 will be pid and value2 will be size
    while(p != &struct_array[NUM_STRUCTS-1]){
        printf(1,"%d  %d %s\n",p->value1,p->value2,p->name);
        p++;
    }

    printf(1, "Program %d \n", return_val);
    exit();
}