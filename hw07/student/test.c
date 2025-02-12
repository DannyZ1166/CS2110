#include <stdio.h>

int capitalized(char* str1, char* str2){

int len = strlen(str2);
    for (int i = 0; i < len; i++) {
    char ch = (*(str2 + i));
      if ((ch >= 'a') && (ch <= 'z')) {
        *(str2 + i) = ch - 'a' + 'A';
        }
    }
    return cmpstrn(str1, str2);
}

int main(void) {
int a = 6;
int b = 10;
int *pa = &a;
int *pb = &b;
int **ppa = &pa;
int **ppb = &pb;
pb = 0;
    printf("%d\n", **ppb);
}