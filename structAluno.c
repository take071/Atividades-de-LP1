#include <stdio.h>
#include <string.h>
#include <stdlib.h>

 typedef struct {  
 
    char nome[50];
    char matricula [10];
    int idade;
    int sala;
    float notas[3];
    float media;
    float provafinal;
 } aluno;
 

int main () {
 
    aluno teste1;
    float media;

    printf("qual é a matricula do aluno?: %s ", &teste1.matricula);
    scanf("%S", teste1.matricula);
    getchar();
     printf("Qual é seu nome? ");
    fgets(teste1.nome, sizeof(teste1.nome), stdin);
    teste1.nome[strcspn(teste1.nome, "\n")] = 0;

     printf("Qual é sua idade? ");
    scanf("%f", &teste1.idade);
    getchar();
    //teste1.idade = 19;
    
    printf("qual é sua sala?: ");
    scanf("%d", &teste1.sala);
    getchar();
    //teste1.sala = 7;

    for (int i = 0; i < 3; i++) {
    printf("Digite a nota %d: ", i + 1);
    scanf("%f", &teste1.notas[i]);
     }
 
     system("clear");

 printf("======= Informações do Aluno =======\n");
 printf("sua matricula é: %s\n", teste1.matricula);
 printf("nome do aluno é: %d \n", teste1.nome);
 printf("idade do aluno é: %f\n", teste1.idade);
 printf("sua sala é: %d\n", teste1.sala);
 printf("nota do aluno é: %f, %f, %f\n", teste1.notas[0], teste1.notas[1], teste1.notas[2]);

  media = (teste1.notas[0] + teste1.notas[1] + teste1.notas[2]) / 3.0;

 if (media >= 7.0) {
 printf("aluno: %s passou!\n", teste1.nome);
 } else{printf("aluno: %s reprovou!\n", teste1.nome);
}

     return 0;
}
