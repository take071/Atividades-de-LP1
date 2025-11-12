#include <stdio.h>
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
void inicializarFilmes();
void menuPrincipal();
void menuUsuario();
void menuAdmin();
void listarFilmes();
void avaliarFilme();
void adicionarFilme();
float calcularMedia(Filme f);

int main() {
    inicializarFilmes();
    system(CLEAR);
    printf(GREEN "=== SISTEMA DE AVALIACAO DE FILMES ===\n" RESET);

    menuPrincipal(); 
    return 0;
}

