#include<stdio.h>
#include<cs50.h>
#include<string.h>
#include<ctype.h>

bool check(string alph);

int main(int argc, string argv[]){
    if(!(argc==2)  || (check(argv[1])==false)){
        printf("Usage: ./vigenere keyword\n");
        return 1;
    }else{
        int shift[strlen(argv[1])];
        for(int i = 0; i < strlen(argv[1]); i++){
            if(islower(argv[1][i])){
                shift[i] = (int)argv[1][i] -97;
            }
            else{
                shift[i]=(int)argv[1][i] - 65;
            }
        }

    string a = get_string("plaintext: ");
    printf("ciphertext: ");
    int n = sizeof(shift)/sizeof(shift[0]);
    int wrap = 0;
    for(int i =0; i<strlen(a); i++){
        if(isalpha(a[i])){
            int index = wrap%n;
            int quer = (int)a[i]+(int)shift[index];
            if(islower(a[i]) && quer>122){
                quer = quer-26;
            }
            else if(isupper(a[i]) && quer>90){
                quer = quer-26;
            }
            printf("%c",quer);
            wrap++;
        }
        else printf("%c", a[i]);
    }
    printf("\n");

    }
}

bool check(string g){
    for(int i = 0; i < strlen(g); i++){
        if(!isalpha(g[i])){
            return false;
        }
    }
    return true;
}
