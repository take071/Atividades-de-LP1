#include <stdio.h>
#include <string.h>
#include <stdlib.h>

 void troca(char *a, int *b){
    strcpy(a,"nome aleatorio");
    *b = *b + 100;
 }



int main()
{
   char a[50];
   int salario;
   
   printf("digite seu nome\n");
   fgets(a,50,stdin);
   
   printf("digite seu salario: ");
   scanf("%d", &salario);
   
   printf("%s",a);
   printf("%d\n", salario);
   
   system("clear");
   
   troca(a, &salario);
   
   printf("%s\n",a);
   printf("%d\n", salario);
    return 0;
}
