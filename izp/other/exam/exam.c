#include <stdio.h>

void is_float_int(){
    float f = 0.0f;
    for(int i=0; i<10; i++)
        f = f + 0.1;
    if(f == 1.0f)
       printf("vysledek je %6.4f\n", f);
    else
       printf("nope, %6.4f\n", f);
}

void test(int a, int b){
    if(a=b)
        printf("a je rovno b\n");
    else
        printf("a neni rovno b\n");
}

void printMainSideDiag(int n, int arr[n][n]){
    for(int i = 0; i < n; i++)
        if(i==n-i-1)
            printf("%d \n", arr[i][i]);
        else
            printf("%d %d \n", arr[i][i<n/2 ? i : n-i-1],
                               arr[i][i<n/2 ? n-1-i : i]);
}

void myPrint(int n){
    printf ("%d", n/2);
    if(n>0)
        myPrint(n-1);
    printf("%d", n);
}

void myOtherPrint(int n){
    printf("%d", n % 2);
    if(n > 0)
        myPrint(n-1);
    printf("%d", n/2);
}

void switchTest(){
    int sum = 1;
    for(int i = 0; i < 10; i++){
        switch(i){
            case 1: case 2: case 7: sum++;
            default: continue;
            case 3: break;
        }
        break;
    }
    printf("%d", sum);
}

unsigned long vypocet (unsigned long a, unsigned long b){
    if (a*b == 0) return 0;
    if (a < b){
        a ^= b;
        b ^= a;
        a ^= b;
    }
    return (a%b > 0)? vypocet(b, a%b) : b;
}

void zobrazeni(int cislo, int bitu){
    if(bitu > 2)
        zobrazeni(cislo >> 2, bitu - 2);
    printf("%d%d ", cislo & 1, (cislo>>1) & 1);
}

int main(){
    /*
    is_float_int();

    test(1,1);
    test(654,213);
    test(0,1);
    test(1,0);

    int n = 5;
    int m[5][5] = {{0}};
    for(int i = 0; i < n*n; i++){
        m[i/n][i%n] = i + 1;
    }
    printMainSideDiag(n, m);

    myPrint(4);
    myOtherPrint(4);

    unsigned a = 1, b = 2, c;
    c = a & b|| a && b;
    printf("%d\n", c);

    switchTest();

    printf("Vysledek %lu\n", vypocet(114, 204));

    zobrazeni(121, 8);
    */
    return 0;
}
