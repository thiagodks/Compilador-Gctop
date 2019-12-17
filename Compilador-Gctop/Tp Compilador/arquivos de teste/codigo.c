#include <stdio.h>
#include <stdlib.h>

#define MAX 100
int main(){

    //isso é um comentário de uma linha

    while(1){
        printf("TESTANDO..\n");
    }

    char t = 't';

    /* isso é um comentario de
    n linhas hahaha
    teste de erros: 
    @@@@@@@@@@@@@@@ áááá´eééééé úúúúíííí 01a 0023b*/

    int x = 0, y = 0, @ = 0, a = 0;
    int x = 0, y = 0, @ = 0, a = 0;
    int z = 0, 01w = 0;

    printf("Digite dois numeros: ");
    scanf("%d %d", &x, &y);

    z = x + y;

    char nóme[256] = "Thiago Adriano";
    for(int i = 0; i < z + MAX; i++){
        x += i;
        y -= i;
    }

    if(x == z || 09x == y){
    	return -1;
    }

    príntf(" != x: %d, y: %d, z: %d\n", x, y, z);
    return 0;
}