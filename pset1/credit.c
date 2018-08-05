#include<cs50.h>
#include<stdio.h>

int main (void){
    long long i;
    do
    {
        i = get_long_long("Number: ");

    }
    while(i<0);
    int sum, first, second;
    int count = 1;
    while(i!=0){
        int d = i%10;
        i =i/10;

        if(count%2==0){
            first = d;
            d= d*2;
            if(d>9){d=d/10+d%10;}
            sum = sum+d;
            count++;
        }
        else{
        sum = sum+d;
        count++;
        second = d;
        }
    }
    count = count-1;

    if(!(count%2==0)){
        int n = first;
        first = second;
        second =n;
    }

    if(sum%10==0){
        if((count ==15 && first == 3) && (second==4||second==7)){
            printf("AMEX\n");
        }

        else if((count ==16 || count==13) && first==4){
            printf("VISA\n");
        }

        else if((first==5 && count==16) && (second<6 && second!=0)){
            printf("MASTERCARD\n");
         }

         else{
             printf("INVALID\n");
        }


    }
    else{
        printf("INVALID\n");

    }

}