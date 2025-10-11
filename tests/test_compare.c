#include "main.h"
#include <stdio.h>
int main(void){
    int a=_printf("%+d%+d\n",1024,1024);
    int b= printf("%+d%+d\n",1024,1024);
    printf("ret _printf=%d  ret printf=%d\n",a,b);
    return 0;
}
