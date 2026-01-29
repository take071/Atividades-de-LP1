#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define YELLOW "\033[1;33m"
#define CYAN "\033[1;36m"
#define RESET "\033[0m"

#ifdef _WIN32
  #define CLEAR "cls"
#else
  #define CLEAR "clear"
#endif

/* Estruturas de dados */
typedef struct {
    char titulo[50];
    char resumo[200];
    int somaNotas;       /* soma das notas recebidas */
    int numAvaliadores;  /* número de avaliações */
    int numComentarios;  /* número de comentários (para controle) */
} Filme;

typedef struct {
    char nome[30];
    char senha[30];
} Usuario;

typedef struct {
    int indiceFilme;          /* índice do filme no array */
    char nomeUsuario[30];     /* quem avaliou */
    int nota;                 /* 1 a 5 */
    char comentario[81];      /* até 80 chars + '\0' */
} Avaliacao;

/* Banco em memória (limites simples) */
#define MAX_FILMES 50
#define MAX_USUARIOS 100
#define MAX_AVALIACOES 1000

Filme filmes[MAX_FILMES];
int totalFilmes = 0;

Usuario usuarios[MAX_USUARIOS];
int totalUsuarios = 0;

Avaliacao avaliacoes[MAX_AVALIACOES];
int totalAvaliacoes = 0;

/* Arquivos binários */
const char *ARQ_FILMES = "filmes.bin";
const char *ARQ_USUARIOS = "usuarios.bin";
const char *ARQ_AVALIACOES = "avaliacoes.bin";

/* Protótipos (simples e diretos) */
void carregarDados();
void salvarDados();
void inicializarFilmesCasoNecessario();
void menuPrincipal();
void menuUsuario();
void menuAdmin();

void listarFilmesPaginado();
void adicionarFilme();
void removerFilme();
void avaliarFilmePorUsuario(const char *usuario);
int cadastrarUsuario();
int loginUsuario(char *usuarioSaida);
int encontrarUsuario(const char *nome);

/* Funções de arquivo */
void carregarDados() {
    FILE *f;

    f = fopen(ARQ_FILMES, "rb");
    if (f) {
        fread(&totalFilmes, sizeof(int), 1, f);
        fread(filmes, sizeof(Filme), totalFilmes, f);
        fclose(f);
    } else {
        /* se não existe arquivo, inicializa com 20 filmes padrão */
        inicializarFilmesCasoNecessario();
        salvarDados();
    }

    f = fopen(ARQ_USUARIOS, "rb");
    if (f) {
        fread(&totalUsuarios, sizeof(int), 1, f);
        fread(usuarios, sizeof(Usuario), totalUsuarios, f);
        fclose(f);
    } else {
        totalUsuarios = 0;
        /* opcional: criar usuário admin não necessário (admin é separado) */
    }

    f = fopen(ARQ_AVALIACOES, "rb");
    if (f) {
        fread(&totalAvaliacoes, sizeof(int), 1, f);
        fread(avaliacoes, sizeof(Avaliacao), totalAvaliacoes, f);
        fclose(f);
    } else {
        totalAvaliacoes = 0;
    }
}

void salvarDados() {
    FILE *f;

    f = fopen(ARQ_FILMES, "wb");
    if (f) {
        fwrite(&totalFilmes, sizeof(int), 1, f);
        fwrite(filmes, sizeof(Filme), totalFilmes, f);
        fclose(f);
    }

    f = fopen(ARQ_USUARIOS, "wb");
    if (f) {
        fwrite(&totalUsuarios, sizeof(int), 1, f);
        fwrite(usuarios, sizeof(Usuario), totalUsuarios, f);
        fclose(f);
    }

    f = fopen(ARQ_AVALIACOES, "wb");
    if (f) {
        fwrite(&totalAvaliacoes, sizeof(int), 1, f);
        fwrite(avaliacoes, sizeof(Avaliacao), totalAvaliacoes, f);
        fclose(f);
    }
}

/* Inicializa 20 filmes padrão (usado se não existir arquivo) */
void inicializarFilmesCasoNecessario() {
    /* Apenas preenche os primeiros 20 filmes com resumos */
    totalFilmes = 20;

    strcpy(filmes[0].titulo, "Matrix");
    strcpy(filmes[0].resumo, "Um hacker descobre a verdade sobre a realidade.");
    filmes[0].somaNotas = 0; filmes[0].numAvaliadores = 0; filmes[0].numComentarios = 0;

    strcpy(filmes[1].titulo, "Interestelar");
    strcpy(filmes[1].resumo, "Uma viagem pelo espaço e tempo em busca de um novo lar.");
    filmes[1].somaNotas = 0; filmes[1].numAvaliadores = 0; filmes[1].numComentarios = 0;

    strcpy(filmes[2].titulo, "Vingadores: Ultimato");
    strcpy(filmes[2].resumo, "Heróis se unem para derrotar Thanos e restaurar o universo.");
    filmes[2].somaNotas = 0; filmes[2].numAvaliadores = 0; filmes[2].numComentarios = 0;

    strcpy(filmes[3].titulo, "O Senhor dos Aneis: O Retorno do Rei");
    strcpy(filmes[3].resumo, "A jornada final para destruir o Um Anel e salvar a Terra Média.");
    filmes[3].somaNotas = 0; filmes[3].numAvaliadores = 0; filmes[3].numComentarios = 0;

    strcpy(filmes[4].titulo, "Coringa");
    strcpy(filmes[4].resumo, "A origem sombria de um dos maiores vilões de Gotham.");
    filmes[4].somaNotas = 0; filmes[4].numAvaliadores = 0; filmes[4].numComentarios = 0;

    strcpy(filmes[5].titulo, "Gladiador");
    strcpy(filmes[5].resumo, "Um general romano busca vingança como gladiador.");
    filmes[5].somaNotas = 0; filmes[5].numAvaliadores = 0; filmes[5].numComentarios = 0;

    strcpy(filmes[6].titulo, "O Rei Leao");
    strcpy(filmes[6].resumo, "A história de Simba, o herdeiro do reino animal.");
    filmes[6].somaNotas = 0; filmes[6].numAvaliadores = 0; filmes[6].numComentarios = 0;

    strcpy(filmes[7].titulo, "Titanic");
    strcpy(filmes[7].resumo, "Um romance trágico em meio ao naufrágio do Titanic.");
    filmes[7].somaNotas = 0; filmes[7].numAvaliadores = 0; filmes[7].numComentarios = 0;

    strcpy(filmes[8].titulo, "A Origem");
    strcpy(filmes[8].resumo, "Um ladrão invade sonhos para plantar ideias no subconsciente.");
    filmes[8].somaNotas = 0; filmes[8].numAvaliadores = 0; filmes[8].numComentarios = 0;

    strcpy(filmes[9].titulo, "Forrest Gump");
    strcpy(filmes[9].resumo, "A história de um homem simples que marcou épocas.");
    filmes[9].somaNotas = 0; filmes[9].numAvaliadores = 0; filmes[9].numComentarios = 0;

    strcpy(filmes[10].titulo, "Avatar");
    strcpy(filmes[10].resumo, "Um humano vive entre alienígenas no planeta Pandora.");
    filmes[10].somaNotas = 0; filmes[10].numAvaliadores = 0; filmes[10].numComentarios = 0;

    strcpy(filmes[11].titulo, "Os Incriveis");
    strcpy(filmes[11].resumo, "Uma família de super-heróis tenta viver uma vida normal.");
    filmes[11].somaNotas = 0; filmes[11].numAvaliadores = 0; filmes[11].numComentarios = 0;

    strcpy(filmes[12].titulo, "Homem-Aranha");
    strcpy(filmes[12].resumo, "Peter Parker ganha poderes após ser picado por uma aranha.");
    filmes[12].somaNotas = 0; filmes[12].numAvaliadores = 0; filmes[12].numComentarios = 0;

    strcpy(filmes[13].titulo, "Batman: O Cavaleiro das Trevas");
    strcpy(filmes[13].resumo, "Batman enfrenta o caos provocado pelo Coringa.");
    filmes[13].somaNotas = 0; filmes[13].numAvaliadores = 0; filmes[13].numComentarios = 0;

    strcpy(filmes[14].titulo, "Pantera Negra");
    strcpy(filmes[14].resumo, "T'Challa precisa proteger o trono de Wakanda.");
    filmes[14].somaNotas = 0; filmes[14].numAvaliadores = 0; filmes[14].numComentarios = 0;

    strcpy(filmes[15].titulo, "Duna");
    strcpy(filmes[15].resumo, "Intrigas e guerras em um deserto interplanetário.");
    filmes[15].somaNotas = 0; filmes[15].numAvaliadores = 0; filmes[15].numComentarios = 0;

    strcpy(filmes[16].titulo, "Top Gun: Maverick");
    strcpy(filmes[16].resumo, "Um piloto veterano treina novos recrutas em missões perigosas.");
    filmes[16].somaNotas = 0; filmes[16].numAvaliadores = 0; filmes[16].numComentarios = 0;

    strcpy(filmes[17].titulo, "Toy Story");
    strcpy(filmes[17].resumo, "Brinquedos ganham vida quando os humanos não estão por perto.");
    filmes[17].somaNotas = 0; filmes[17].numAvaliadores = 0; filmes[17].numComentarios = 0;

    strcpy(filmes[18].titulo, "Procurando Nemo");
    strcpy(filmes[18].resumo, "Um peixe-palhaço cruza os oceanos para encontrar seu filho.");
    filmes[18].somaNotas = 0; filmes[18].numAvaliadores = 0; filmes[18].numComentarios = 0;

    strcpy(filmes[19].titulo, "Wall-E");
    strcpy(filmes[19].resumo, "Um robô solitário descobre o amor e salva a humanidade.");
    filmes[19].somaNotas = 0; filmes[19].numAvaliadores = 0; filmes[19].numComentarios = 0;

    /* restantes vazios */
    for (int i = 20; i < MAX_FILMES; i++) {
        filmes[i].titulo[0] = '\0';
        filmes[i].resumo[0] = '\0';
        filmes[i].somaNotas = filmes[i].numAvaliadores = filmes[i].numComentarios = 0;
    }
}

/* Main e menus */
int main() {
    carregarDados();
    menuPrincipal();
    salvarDados();
    return 0;
}

void menuPrincipal() {
    int opcao;
    do {
        system(CLEAR);
        printf(GREEN "=== SISTEMA DE AVALIAÇÃO DE FILMES ===\n" RESET);
        printf("1 - Entrar como Usuário\n");
        printf("2 - Entrar como Administrador\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        if (scanf("%d", &opcao) != 1) { while(getchar()!='\n'); opcao = -1; }
        getchar();

        switch(opcao) {
            case 1:
                menuUsuario();
                break;
            case 2:
                menuAdmin();
                break;
            case 0:
                printf(YELLOW "Saindo... Obrigado!\n" RESET);
                break;
            default:
                printf(RED "Opção inválida.\n" RESET);
        }
        if (opcao != 0) {
            printf("\nPressione ENTER para continuar...");
            getchar();
        }
    } while (opcao != 0);
}

/* Menu para usuário: login/cadastro, listar e avaliar */
void menuUsuario() {
    int opc;
    char usuarioLogado[30] = {0};
    int logado = 0;

    do {
        system(CLEAR);
        printf(CYAN "=== MENU USUÁRIO ===\n" RESET);
        if (!logado) {
            printf("1 - Login\n");
            printf("2 - Cadastrar novo usuário\n");
            printf("0 - Voltar\n");
            printf("Escolha: ");
            if (scanf("%d", &opc) != 1) { while(getchar()!='\n'); opc = -1; }
            getchar();

            if (opc == 1) {
                if (loginUsuario(usuarioLogado)) logado = 1;
                else { printf(RED "Login falhou.\n" RESET); }
            } else if (opc == 2) {
                if (cadastrarUsuario()) printf(GREEN "Usuário cadastrado.\n" RESET);
                else printf(RED "Cadastro cancelado ou falhou.\n" RESET);
            } else if (opc == 0) {
                break;
            } else {
                printf(RED "Opção inválida.\n" RESET);
            }
        } else {
            /* após login, opções do usuário */
            printf("Usuário: %s\n", usuarioLogado);
            printf("1 - Listar filmes\n");
            printf("2 - Avaliar um filme\n");
            printf("3 - Ver minhas avaliações\n");
            printf("0 - Logout\n");
            printf("Escolha: ");
            if (scanf("%d", &opc) != 1) { while(getchar()!='\n'); opc = -1; }
            getchar();

            if (opc == 1) {
                listarFilmesPaginado();
            } else if (opc == 2) {
                avaliarFilmePorUsuario(usuarioLogado);
            } else if (opc == 3) {
                /* listar avaliações do usuário */
                system(CLEAR);
                printf(BLUE "=== Minhas Avaliações ===\n" RESET);
                int encontrou = 0;
                for (int i = 0; i < totalAvaliacoes; i++) {
                    if (strcmp(avaliacoes[i].nomeUsuario, usuarioLogado) == 0) {
                        encontrou = 1;
                        printf(YELLOW "\nFilme: %s\n" RESET, filmes[avaliacoes[i].indiceFilme].titulo);
                        printf("Nota: %d\n", avaliacoes[i].nota);
                        if (avaliacoes[i].comentario[0]) printf("Coment: %s\n", avaliacoes[i].comentario);
                    }
                }
                if (!encontrou) printf("Nenhuma avaliação feita por você ainda.\n");
            } else if (opc == 0) {
                logado = 0;
                usuarioLogado[0] = '\0';
                printf(YELLOW "Logout realizado.\n" RESET);
            } else {
                printf(RED "Opção inválida.\n" RESET);
            }
        }
        printf("\nPressione ENTER para continuar...");
        getchar();
    } while (1);
}

/* Admin: usuário 'admin' e senha '1234' (fixos) */
void menuAdmin() {
    char login[30], senha[30];
    system(CLEAR);
    printf(BLUE "=== LOGIN ADMIN ===\n" RESET);
    printf("Usuário: ");
    fgets(login, sizeof(login), stdin);
    login[strcspn(login, "\n")] = '\0';
    printf("Senha: ");
    fgets(senha, sizeof(senha), stdin);
    senha[strcspn(senha, "\n")] = '\0';

    if (strcmp(login, "admin") == 0 && strcmp(senha, "1234") == 0) {
        int opc;
        do {
            system(CLEAR);
            printf(GREEN "=== MENU ADMIN ===\n" RESET);
            printf("1 - Adicionar filme\n");
            printf("2 - Listar filmes\n");
            printf("3 - Remover filme\n");
            printf("0 - Voltar\n");
            printf("Escolha: ");
            if (scanf("%d", &opc) != 1) { while(getchar()!='\n'); opc = -1; }
            getchar();

            if (opc == 1) {
                adicionarFilme();
            } else if (opc == 2) {
                listarFilmesPaginado();
            } else if (opc == 3) {
                removerFilme();
            } else if (opc == 0) {
                break;
            } else {
                printf(RED "Opção inválida.\n" RESET);
            }
            printf("\nPressione ENTER para continuar...");
            getchar();
        } while (1);
    } else {
        printf(RED "Acesso negado.\n" RESET);
    }
}

/* Lista filmes em páginas de 10 */
void listarFilmesPaginado() {
    int pagina = 0;
    int totalPaginas = (totalFilmes + 9) / 10;
    if (totalPaginas == 0) totalPaginas = 1;

    while (1) {
        system(CLEAR);
        printf(BLUE "=== Catálogo de Filmes (página %d/%d) ===\n" RESET, pagina + 1, totalPaginas);
        int inicio = pagina * 10;
        int fim = inicio + 10;
        if (fim > totalFilmes) fim = totalFilmes;
        for (int i = inicio; i < fim; i++) {
            float media = 0.0f;
            if (filmes[i].numAvaliadores > 0) media = (float)filmes[i].somaNotas / filmes[i].numAvaliadores;
            printf(YELLOW "\n%d) %s\n" RESET, i + 1, filmes[i].titulo);
            printf("   Média: %.2f (%d avaliações)\n", media, filmes[i].numAvaliadores);
            printf("   Resumo: %.150s\n", filmes[i].resumo);
        }
        printf("\nN - próxima página | P - anterior | 0 - sair\nEscolha: ");
        char op[10];
        fgets(op, sizeof(op), stdin);
        if (op[0] == 'N' || op[0] == 'n') {
            if (pagina < totalPaginas - 1) pagina++;
        } else if (op[0] == 'P' || op[0] == 'p') {
            if (pagina > 0) pagina--;
        } else {
            break;
        }
    }
}

/* Adicionar filme (admin) */
void adicionarFilme() {
    if (totalFilmes >= MAX_FILMES) {
        printf(RED "Limite de filmes atingido.\n" RESET);
        return;
    }
    system(CLEAR);
    printf(BLUE "=== Adicionar Filme ===\n" RESET);
    printf("Título: ");
    fgets(filmes[totalFilmes].titulo, sizeof(filmes[totalFilmes].titulo), stdin);
    filmes[totalFilmes].titulo[strcspn(filmes[totalFilmes].titulo, "\n")] = '\0';
    printf("Resumo (máx 199 chars): ");
    fgets(filmes[totalFilmes].resumo, sizeof(filmes[totalFilmes].resumo), stdin);
    filmes[totalFilmes].resumo[strcspn(filmes[totalFilmes].resumo, "\n")] = '\0';
    filmes[totalFilmes].somaNotas = 0;
    filmes[totalFilmes].numAvaliadores = 0;
    filmes[totalFilmes].numComentarios = 0;
    totalFilmes++;
    salvarDados();
    printf(GREEN "Filme adicionado com sucesso.\n" RESET);
}

/* Remover filme (admin) */
void removerFilme() {
    listarFilmesPaginado();
    printf("\nDigite o número do filme a remover (0 para cancelar): ");
    int id;
    if (scanf("%d", &id) != 1) { while(getchar()!='\n'); printf(RED "Entrada inválida.\n" RESET); return; }
    getchar();
    if (id == 0) return;
    if (id < 1 || id > totalFilmes) { printf(RED "ID inválido.\n" RESET); return; }
    id--;
    for (int i = id; i < totalFilmes - 1; i++) filmes[i] = filmes[i + 1];
    totalFilmes--;
    /* também remover avaliações ligadas ao filme */
    int dst = 0;
    for (int i = 0; i < totalAvaliacoes; i++) {
        if (avaliacoes[i].indiceFilme == id) continue; /* pula avaliações do filme removido */
        avaliacoes[dst++] = avaliacoes[i];
        /* corrige índices maiores */
        if (avaliacoes[dst-1].indiceFilme > id) avaliacoes[dst-1].indiceFilme--;
    }
    totalAvaliacoes = dst;
    salvarDados();
    printf(GREEN "Filme removido.\n" RESET);
}

/* Avaliar filme por usuário logado */
void avaliarFilmePorUsuario(const char *usuario) {
    if (totalFilmes == 0) { printf(RED "Nenhum filme cadastrado.\n" RESET); return; }
    listarFilmesPaginado();
    printf("\nDigite o número do filme que deseja avaliar (0 para cancelar): ");
    int id;
    if (scanf("%d", &id) != 1) { while(getchar()!='\n'); printf(RED "Entrada inválida.\n" RESET); return; }
    getchar();
    if (id == 0) return;
    if (id < 1 || id > totalFilmes) { printf(RED "ID inválido.\n" RESET); return; }
    id--;

    int nota;
    do {
        printf("Digite a nota (1 a 5): ");
        if (scanf("%d", &nota) != 1) { while(getchar()!='\n'); nota = -1; }
        getchar();
        if (nota < 1 || nota > 5) printf(RED "Nota inválida. Tente novamente.\n" RESET);
    } while (nota < 1 || nota > 5);

    char comentario[81] = {0};
    printf("Comentário (opcional, até 80 chars). Deixe em branco para pular:\n");
    fgets(comentario, sizeof(comentario), stdin);
    comentario[strcspn(comentario, "\n")] = '\0';

    /* registra avaliação */
    if (totalAvaliacoes < MAX_AVALIACOES) {
        avaliacoes[totalAvaliacoes].indiceFilme = id;
        strncpy(avaliacoes[totalAvaliacoes].nomeUsuario, usuario, sizeof(avaliacoes[totalAvaliacoes].nomeUsuario)-1);
        avaliacoes[totalAvaliacoes].nota = nota;
        strncpy(avaliacoes[totalAvaliacoes].comentario, comentario, sizeof(avaliacoes[totalAvaliacoes].comentario)-1);
        totalAvaliacoes++;
    } else {
        printf(RED "Limite de avaliações atingido, avaliação não registrada.\n" RESET);
        return;
    }

    filmes[id].somaNotas += nota;
    filmes[id].numAvaliadores += 1;
    if (comentario[0] != '\0') filmes[id].numComentarios += 1;

    salvarDados();
    printf(GREEN "Avaliação registrada. Obrigado!\n" RESET);
}

/* Cadastro e login simples */
int cadastrarUsuario() {
    if (totalUsuarios >= MAX_USUARIOS) { printf(RED "Limite de usuários atingido.\n" RESET); return 0; }
    char nome[30], senha[30];
    printf("Nome (sem espaços): ");
    if (!fgets(nome, sizeof(nome), stdin)) return 0;
    nome[strcspn(nome, "\n")] = '\0';
    if (strlen(nome) == 0) return 0;
    if (encontrarUsuario(nome) != -1) { printf(RED "Usuário já existe.\n" RESET); return 0; }
    printf("Senha: ");
    if (!fgets(senha, sizeof(senha), stdin)) return 0;
    senha[strcspn(senha, "\n")] = '\0';
    if (strlen(senha) == 0) return 0;
    strncpy(usuarios[totalUsuarios].nome, nome, sizeof(usuarios[totalUsuarios].nome)-1);
    strncpy(usuarios[totalUsuarios].senha, senha, sizeof(usuarios[totalUsuarios].senha)-1);
    totalUsuarios++;
    salvarDados();
    return 1;
}

int loginUsuario(char *usuarioSaida) {
    char nome[30], senha[30];
    printf("Nome: ");
    if (!fgets(nome, sizeof(nome), stdin)) return 0;
    nome[strcspn(nome, "\n")] = '\0';
    printf("Senha: ");
    if (!fgets(senha, sizeof(senha), stdin)) return 0;
    senha[strcspn(senha, "\n")] = '\0';
    int idx = encontrarUsuario(nome);
    if (idx == -1) return 0;
    if (strcmp(usuarios[idx].senha, senha) == 0) {
        strncpy(usuarioSaida, nome, 29);
        usuarioSaida[29] = '\0';
        return 1;
    }
    return 0;
}

int encontrarUsuario(const char *nome) {
    for (int i = 0; i < totalUsuarios; i++) if (strcmp(usuarios[i].nome, nome) == 0) return i;
    return -1;
}
