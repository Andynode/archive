#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>

#define I0 1e-12
#define E 2.718281828
#define UT 25.8563e-3

#define UP 1
#define RIGHT 2
#define DOWN 4
#define LEFT 8

void print(char c){
    for(int i = 3; i >= 0; i--)
        printf((c>>i & 1)? "1":"0");
    printf("\n");
}

int main(){
    /*
    int a[2][2] = {
        {1, 0},
        {0, 1}
    };
    printf("%d\n", a[0][0]);
    for(int i = 1; i < 9; i *= 2){
        printf("%d\t", (int)log2(i));
    }
    printf("\n");
    printf("%d\n", 5 % 4);
    */
    char c = '0';
    print(c);
    c |= 1;
    print(c);
    //c |= 2;
    print(c);
    //c |= 4;
    print(c);
    c |= 8;
    print(c);


    return 0;
}

