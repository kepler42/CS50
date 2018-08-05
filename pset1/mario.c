#include<stdio.h>
#include<cs50.h>
int main(void){
   int n = get_int("Height: ");
    while(n>23||n<0){
     n = get_int("Height: ");
    }
    for(int i = 0;i<n; i++){
        for(int  q=1;q<n-i;q++){
            printf(" ");
        }

        for(int q =-1;q<i;q++){
            printf("#");
        }
        printf("  ");

        for(int q =-1;q<i;q++){
            printf("#");
        }
        printf("\n");


    }

}