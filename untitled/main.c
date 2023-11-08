#include <stdio.h>
#include <stdlib.h>

void printInfinite(){
    int m = rand() % 2;
    printf("Coucou %d\n", m);
    if(m == 1){
        printInfinite();
    }
}

int main() {
    printf("Hello, World!\n");


    int m = rand() % 2;

    for (printf("Start boucle\n"); m == 1; printf("On continue\n")){
        printf("Coucou %d\n", m);
        m = rand() % 2;
    }
    return 0;
}
