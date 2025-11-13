#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define YELLOW "\033[1;33m"
#define CYAN  "\033[1;36m"
#define RESET "\033[0m"

#ifdef _WIN32
  #define CLEAR "cls"
#else
  #define CLEAR "clear"
#endif

// Struct para armazenar filmes
typedef struct {
    char nome[50];
    float nota;
    char resumo[200]; 
} Filme;

// Banco de dados dos filmes
Filme filmes[50];
int totalFilmes = 0;

// Declaração antecipada das funções
//void inicializarFilmes();
void menuPrincipal();
//void menuUsuario();
//void menuAdmin();
void listarFilmes();
//void avaliarFilme();
//void adicionarFilme();
//float calcularMedia(Filme f);

int main() {
    
    system(CLEAR);
    printf(GREEN "=== SISTEMA DE AVALIACAO DE FILMES ===\n" RESET);
    menuPrincipal();
   
    return 0;
}

void menuPrincipal() {
    int opcao;

    do {
        // Limpa o terminal antes de mostrar o menu
        system(CLEAR);

        // Mostra o título
        printf(GREEN "\n=== SISTEMA DE AVALIAÇÃO DE FILMES ===\n" RESET);

        // Mostra as opções principais
        printf(BLUE "\nMenu Principal:\n" RESET);
        printf("1 - Entrar como Usuário\n");
        printf("2 - Entrar como Administrador\n");
        printf("3 - Ver catálogo de filmes\n");
        printf("0 - Sair\n");
        printf("\nEscolha uma opção: ");

        scanf("%d", &opcao);
        getchar(); // limpa o ENTER deixado pelo scanf

        // Limpa a tela após escolher
        system(CLEAR);

        // Trata a escolha do usuário
        switch(opcao) {
            case 1:
                printf(GREEN "Bem-vindo, Usuário!\n" RESET);
                //avaliarFilme(); // exemplo de função
                break;

            case 2:
                //menuAdmin();
                break;

            case 3:
               listarFilmes();
                break;

            case 0:
                printf(YELLOW "Encerrando o programa... Até logo!\n" RESET);
                break;

            default:
                printf(RED "Opção inválida! Tente novamente.\n" RESET);
                break;
        }

        // pausa para o usuário ver a mensagem antes de limpar de novo
        if (opcao != 0) {
            printf("\nPressione ENTER para continuar...");
            getchar();
        }

    } while (opcao != 0);
}

void listarFilmes(){

printf(RED"maconha preta a revolta"RESET);

}
