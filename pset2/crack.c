#include<cs50.h>
#include<stdio.h>
#include<string.h>
#include <unistd.h>
#define _XOPEN_SOURCE


//Assume that each password is no longer than five (5) characters. Gasp!

//Assume that each password is composed entirely of alphabetical characters (uppercase and/or lowercase).

int main(int argc, string argv[]){
     if(!(argc==2)) {
         printf("entre proper input moron\n");
         return 1;
     }
    char one = argv[1][0];
    char two = argv[1][1];
    char salt[2];
    salt[0] = one;
    salt[1] = two;
    for(int i = 0; i<strlen(argv[1]); i++){
        int j = 65+i;
        char a[1];
        a[0] = j;
        char *hash = crypt(a,salt);
    }
}