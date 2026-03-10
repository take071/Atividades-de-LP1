#include <stdio.h>
#include <stdlib.h>

int main(){
   
   int x,d,c;
   char op;
   int *p = &x;
 do{
  printf("PONTEIROS  EM 'C'");
   getchar();
   system("clear");

 printf("escolha se você quer o programa seja de forma automática ou manual...\n");
 printf("- 1 para automático\n");
 printf("- 2 para manual\n");
 scanf("%d",&d);
getchar();
system("clear");

switch(d) {
 case 1:
do {
 printf("digite o valor de 'x', sem ser o número 19: \n");
 scanf("%d",&x);
 getchar(); system("clear");
 if(x == 19) {
 printf("19 não pode ser usado por ti\n");}
 } while (x == 19);

 printf("o valor de 'x' é: %d\n",x);
 printf("o endereço de 'x' é: %d\n",&x);
 
 *p =19;
 printf("agora 'x' vale %d, por causa do '*p'\n",*p);

break;

 case 2:
  printf("escolha um valor para ser posto no ponteiro: \n");
  scanf("%d", &c);
  getchar(); system("clear");
  
   printf("digite o valor de 'x': \n");
   scanf("%d",&x);
   getchar(); system("clear");
  
  int *p = &c;

  printf("o valor dentro do ponteiro é %d\n",*p);
  printf("o valor de 'x' é: %d\n",x);
  printf("o endereço de 'x' é: %d\n",&x);
  break;
  default:
   printf(" escolha a opção 1 ou 2\n");
   getchar(); system("clear");
 break;
} 
  printf("deseja esolher outra opção?(s/n)\n");
  scanf("%c",&op);getchar();system("clear");
} while(op == 's' || op == 'S');

printf("cabou fih, tmj. Deusabençoe\n");
   return 0;
}
