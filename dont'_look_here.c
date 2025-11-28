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
//float calcularMedia(Filme f);
void removerFilme();

int main() {
    
    printf(GREEN "=== SISTEMA DE AVALIACAO DE FILMES ===\n" RESET);
    inicializarFilmes();
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
                menuUsuario(); 
                break;

            case 2:
                menuAdmin();
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

void menuUsuario() {
    int opcao;

    do {
        system(CLEAR);

        printf(CYAN "=== MENU DO USUÁRIO ===\n" RESET);
        printf("1 - Listar filmes\n");
        printf("2 - Avaliar um filme\n");
        printf("0 - Voltar ao menu principal\n");
        printf("\nEscolha: ");

        scanf("%d", &opcao);
        getchar(); // limpar ENTER

        system(CLEAR);

        switch(opcao) {
            case 1:
                listarFilmes();
                break;

            case 2:
                avaliarFilme();
                break;

            case 0:
                printf(YELLOW "Voltando ao menu principal...\n" RESET);
                break;

            default:
                printf(RED "Opção inválida!\n" RESET);
        }

        if (opcao != 0) {
            printf("\nPressione ENTER para continuar...");
            getchar();
        }

    } while(opcao != 0);
}
void avaliarFilme() {
    if (totalFilmes == 0) {
        printf(RED "Nenhum filme cadastrado para avaliar!\n" RESET);
        return;
    }

    listarFilmes();

    int id;
    printf("\nDigite o número do filme que deseja avaliar: ");
    scanf("%d", &id);
    getchar(); // limpar ENTER

    if (id < 1 || id > totalFilmes) {
        printf(RED "ID inválido!\n" RESET);
        return;
    }

    id--; // converter para índice

    float novaNota;
    printf("Digite sua nota para '%s' (0 a 5): ", filmes[id].nome);
    scanf("%f", &novaNota);
    getchar();

    if (novaNota < 0 || novaNota > 5) {
        printf(RED "Nota inválida! A nota deve estar entre 0 e 5.\n" RESET);
        return;
    }

    // Atualiza a média de forma simples (substitui a nota antiga)
    filmes[id].nota = novaNota;

    printf(GREEN "\nAvaliação registrada com sucesso!\n" RESET);
}

void menuAdmin() {
    char login[20], senha[20];

    printf(BLUE "=== LOGIN DO ADMIN ===\n" RESET);

    printf("Usuário: ");
    fgets(login, sizeof(login), stdin);

    printf("Senha: ");
    fgets(senha, sizeof(senha), stdin);

    if (strcmp(login, "admin\n") == 0 && strcmp(senha, "1234\n") == 0) {
        int opcao;
        do {
            system(CLEAR);
            printf(GREEN "=== MENU ADMINISTRADOR ===\n" RESET);
            printf("1 - Adicionar filme\n");
            printf("2 - Listar filmes\n");
            printf("3 - Remover filme\n");
            printf("0 - Sair\n");
            printf("Escolha: ");

            scanf("%d", &opcao);
            getchar();

            switch (opcao) {
                case 1:
                    adicionarFilme();
                    break;
                case 2:
                    listarFilmes();
                    break;
                case 3:
                    removerFilme();
                    break;
                case 0:
                    printf(YELLOW "Saindo do menu admin...\n" RESET);
                    break;
                default:

                printf(RED "Opção inválida!\n" RESET);
            }

            if (opcao != 0) {
                printf("\nPressione ENTER para continuar...");
                getchar();
            }

        } while (opcao != 0);

    } else {
        printf(RED "Acesso negado!\n" RESET);
    }
}

void adicionarFilme() {
    if (totalFilmes >= 50) {
        printf(RED "Limite máximo de filmes atingido!\n" RESET);
        return;
    }

    printf(BLUE "=== Adicionar Filme ===\n" RESET);

    printf("Nome do filme: ");
    fgets(filmes[totalFilmes].nome, sizeof(filmes[totalFilmes].nome), stdin);

    printf("Resumo: ");
    fgets(filmes[totalFilmes].resumo, sizeof(filmes[totalFilmes].resumo), stdin);

    printf("Nota inicial do filme: ");
    scanf("%f", &filmes[totalFilmes].nota);
    getchar();

    totalFilmes++;

    printf(GREEN "Filme cadastrado com sucesso!\n" RESET);
}
void removerFilme() {
    listarFilmes();

    int id;
    printf("\nDigite o número do filme que deseja remover: ");
    scanf("%d", &id);
    getchar();

    if (id < 1 || id > totalFilmes) {
        printf(RED "ID inválido!\n" RESET);
        return;
    }

    id--; // convertendo para índice do array

    // desloca os filmes para a esquerda
    for (int i = id; i < totalFilmes - 1; i++) {
        filmes[i] = filmes[i + 1];
    }

    totalFilmes--;

    printf(GREEN "Filme removido com sucesso!\n" RESET);
}

void listarFilmes() {
    printf(BLUE "=== Lista de Filmes (%d cadastrados) ===\n" RESET, totalFilmes);

    for (int i = 0; i < totalFilmes; i++) {
        printf(YELLOW "\nFilme %d:\n" RESET, i + 1);
        printf("Nome: %s", filmes[i].nome);
        printf("Nota: %.1f\n", filmes[i].nota);
        printf("Resumo: %s\n", filmes[i].resumo);
    }
}

void inicializarFilmes() {
    strcpy(filmes[0].nome, "Matrix");
    filmes[0].nota = 4.8;
    strcpy(filmes[0].resumo, "Um hacker descobre a verdade sobre a realidade.");

    strcpy(filmes[1].nome, "Interestelar");
    filmes[1].nota = 4.9;
    strcpy(filmes[1].resumo, "Uma viagem pelo espaco e tempo em busca de um novo lar para a humanidade.");

    strcpy(filmes[2].nome, "Vingadores: Ultimato");
    filmes[2].nota = 4.7;
    strcpy(filmes[2].resumo, "Os herois se unem para derrotar Thanos e restaurar o universo.");

    strcpy(filmes[3].nome, "O Senhor dos Aneis: O Retorno do Rei");
    filmes[3].nota = 4.9;
    strcpy(filmes[3].resumo, "A jornada final para destruir o Um Anel e salvar a Terra Media.");

    strcpy(filmes[4].nome, "Coringa");
    filmes[4].nota = 4.6;
    strcpy(filmes[4].resumo, "A origem sombria de um dos maiores vilões de Gotham.");

    strcpy(filmes[5].nome, "Gladiador");
    filmes[5].nota = 4.8;
    strcpy(filmes[5].resumo, "Um general romano busca vingança como gladiador.");

    strcpy(filmes[6].nome, "O Rei Leao");
    filmes[6].nota = 4.7;
    strcpy(filmes[6].resumo, "A historia de Simba, o herdeiro do reino animal.");

    strcpy(filmes[7].nome, "Titanic");
    filmes[7].nota = 4.5;
    strcpy(filmes[7].resumo, "Um romance tragico em meio ao naufragio do Titanic.");

    strcpy(filmes[8].nome, "A Origem");
    filmes[8].nota = 4.8;
    strcpy(filmes[8].resumo, "Um ladrão invade sonhos para plantar ideias no subconsciente.");

    strcpy(filmes[9].nome, "Forrest Gump");
    filmes[9].nota = 4.9;
    strcpy(filmes[9].resumo, "A historia de um homem simples que marcou epocas.");

    strcpy(filmes[10].nome, "Avatar");
    filmes[10].nota = 4.6;
    strcpy(filmes[10].resumo, "Um humano vive entre alienigenas no planeta Pandora.");

    strcpy(filmes[11].nome, "Os Incriveis");
    filmes[11].nota = 4.5;
    strcpy(filmes[11].resumo, "Uma familia de super-herois tenta viver uma vida normal.");

    strcpy(filmes[12].nome, "Homem-Aranha");
    filmes[12].nota = 4.7;
    strcpy(filmes[12].resumo, "Peter Parker ganha poderes apos ser picado por uma aranha.");

    strcpy(filmes[13].nome, "Batman: O Cavaleiro das Trevas");
    filmes[13].nota = 4.9;
    strcpy(filmes[13].resumo, "Batman enfrenta o caos provocado pelo Coringa.");

    strcpy(filmes[14].nome, "Pantera Negra");
    filmes[14].nota = 4.6;
    strcpy(filmes[14].resumo, "T'Challa precisa proteger o trono de Wakanda.");

    strcpy(filmes[15].nome, "Duna");
    filmes[15].nota = 4.5;
    strcpy(filmes[15].resumo, "Um jovem enfrenta intrigas e guerras em um deserto interplanetario.");

    strcpy(filmes[16].nome, "Top Gun: Maverick");
    filmes[16].nota = 4.7;
    strcpy(filmes[16].resumo, "Um piloto veterano treina novos recrutas em missões perigosas.");

    strcpy(filmes[17].nome, "Toy Story");
    filmes[17].nota = 4.8;
    strcpy(filmes[17].resumo, "Brinquedos ganham vida quando os humanos nao estao por perto.");

    strcpy(filmes[18].nome, "Procurando Nemo");
    filmes[18].nota = 4.7;
    strcpy(filmes[18].resumo, "Um peixe-palhaco cruza os oceanos para encontrar seu filho.");

    strcpy(filmes[19].nome, "Wall-E");
    filmes[19].nota = 4.8;
    strcpy(filmes[19].resumo, "Um robo solitario descobre o amor e salva a humanidade.");

    totalFilmes = 20;
}