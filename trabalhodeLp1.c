#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- Constantes de Cor --- */
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define YELLOW "\033[1;33m"
#define CYAN "\033[1;36m"
#define RESET "\033[0m"

/* --- Comando de Limpar Tela --- */
#ifdef _WIN32
  #define CLEAR "cls"
#else
  #define CLEAR "clear"
#endif

/* --- Tratamento de Entrada (Remove \n de fgets) --- */
void limpar_nova_linha(char *str) {
    if (str == NULL) return;
    str[strcspn(str, "\n")] = '\0';
}

/* --- Estruturas de dados --- */
typedef struct {
    char titulo[50];
    char resumo[200];
    int somaNotas;       /* soma das notas recebidas (1 a 5) */
    int numAvaliadores;  /* número de avaliações */
    /* int numComentarios; (não precisa, usar numAvaliadores) */
} Filme;

typedef struct {
    char nome[30];
    char senha[30];
} Usuario;

typedef struct {
    int indiceFilme;             /* índice do filme no array de filmes */
    char nomeUsuario[30];        /* quem avaliou */
    int nota;                    /* 1 a 5 */
    char comentario[81];         /* até 80 chars + '\0' */
} Avaliacao;

/* --- Banco em memória (limites simples) --- */
#define MAX_FILMES 50
#define MAX_USUARIOS 100
#define MAX_AVALIACOES 1000

Filme filmes[MAX_FILMES];
int totalFilmes = 0;

Usuario usuarios[MAX_USUARIOS];
int totalUsuarios = 0;

Avaliacao avaliacoes[MAX_AVALIACOES];
int totalAvaliacoes = 0;

/* --- Arquivos binários --- */
const char *ARQ_FILMES = "filmes.bin";
const char *ARQ_USUARIOS = "usuarios.bin";
const char *ARQ_AVALIACOES = "avaliacoes.bin";

/* --- Protótipos --- */
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
void limpar_buffer_stdin();

/* --- Funções Auxiliares --- */
void limpar_buffer_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


/* --- Funções de arquivo --- */
void carregarDados() {
    FILE *f;

    /* Carregar Filmes */
    f = fopen(ARQ_FILMES, "rb");
    if (f) {
        if (fread(&totalFilmes, sizeof(int), 1, f) == 1 && totalFilmes > 0) {
            fread(filmes, sizeof(Filme), totalFilmes, f);
        } else {
             /* O arquivo existe mas está vazio ou com erro. Inicializa. */
             inicializarFilmesCasoNecessario();
             salvarDados();
        }
        fclose(f);
    } else {
        /* Se o arquivo não existe, inicializa com 20 filmes padrão */
        inicializarFilmesCasoNecessario();
    }

    /* Carregar Usuários */
    f = fopen(ARQ_USUARIOS, "rb");
    if (f) {
        if (fread(&totalUsuarios, sizeof(int), 1, f) == 1) {
            fread(usuarios, sizeof(Usuario), totalUsuarios, f);
        } else {
             totalUsuarios = 0;
        }
        fclose(f);
    } else {
        totalUsuarios = 0;
    }

    /* Carregar Avaliações */
    f = fopen(ARQ_AVALIACOES, "rb");
    if (f) {
        if (fread(&totalAvaliacoes, sizeof(int), 1, f) == 1) {
            fread(avaliacoes, sizeof(Avaliacao), totalAvaliacoes, f);
        } else {
            totalAvaliacoes = 0;
        }
        fclose(f);
    } else {
        totalAvaliacoes = 0;
    }
}

void salvarDados() {
    FILE *f;

    /* Salvar Filmes */
    f = fopen(ARQ_FILMES, "wb");
    if (f) {
        fwrite(&totalFilmes, sizeof(int), 1, f);
        fwrite(filmes, sizeof(Filme), totalFilmes, f);
        fclose(f);
    } else {
        printf(RED "ERRO ao salvar filmes.\n" RESET);
    }

    /* Salvar Usuários */
    f = fopen(ARQ_USUARIOS, "wb");
    if (f) {
        fwrite(&totalUsuarios, sizeof(int), 1, f);
        fwrite(usuarios, sizeof(Usuario), totalUsuarios, f);
        fclose(f);
    } else {
        printf(RED "ERRO ao salvar usuários.\n" RESET);
    }

    /* Salvar Avaliações */
    f = fopen(ARQ_AVALIACOES, "wb");
    if (f) {
        fwrite(&totalAvaliacoes, sizeof(int), 1, f);
        fwrite(avaliacoes, sizeof(Avaliacao), totalAvaliacoes, f);
        fclose(f);
    } else {
        printf(RED "ERRO ao salvar avaliações.\n" RESET);
    }
}

/* Inicializa 20 filmes padrão (usado se não existir arquivo ou se estiver vazio) */
void inicializarFilmesCasoNecessario() {
    /* Apenas preenche os primeiros 20 filmes com resumos */
    totalFilmes = 20;

    /* Apenas os títulos e resumos serão preenchidos. Notas/avaliadores começam em 0. */
    #define INIT_FILME(i, titulo_str, resumo_str) \
        strncpy(filmes[i].titulo, titulo_str, 49); filmes[i].titulo[49] = '\0'; \
        strncpy(filmes[i].resumo, resumo_str, 199); filmes[i].resumo[199] = '\0'; \
        filmes[i].somaNotas = 0; filmes[i].numAvaliadores = 0;

    INIT_FILME(0, "Matrix", "Um hacker descobre a verdade sobre a realidade.");
    INIT_FILME(1, "Interestelar", "Uma viagem pelo espaço e tempo em busca de um novo lar.");
    INIT_FILME(2, "Vingadores: Ultimato", "Heróis se unem para derrotar Thanos e restaurar o universo.");
    INIT_FILME(3, "O Senhor dos Aneis: O Retorno do Rei", "A jornada final para destruir o Um Anel e salvar a Terra Média.");
    INIT_FILME(4, "Coringa", "A origem sombria de um dos maiores vilões de Gotham.");
    INIT_FILME(5, "Gladiador", "Um general romano busca vingança como gladiador.");
    INIT_FILME(6, "O Rei Leao", "A história de Simba, o herdeiro do reino animal.");
    INIT_FILME(7, "Titanic", "Um romance trágico em meio ao naufrágio do Titanic.");
    INIT_FILME(8, "A Origem", "Um ladrão invade sonhos para plantar ideias no subconsciente.");
    INIT_FILME(9, "Forrest Gump", "A história de um homem simples que marcou épocas.");
    INIT_FILME(10, "Avatar", "Um humano vive entre alienígenas no planeta Pandora.");
    INIT_FILME(11, "Os Incriveis", "Uma família de super-heróis tenta viver uma vida normal.");
    INIT_FILME(12, "Homem-Aranha", "Peter Parker ganha poderes após ser picado por uma aranha.");
    INIT_FILME(13, "Batman: O Cavaleiro das Trevas", "Batman enfrenta o caos provocado pelo Coringa.");
    INIT_FILME(14, "Pantera Negra", "T'Challa precisa proteger o trono de Wakanda.");
    INIT_FILME(15, "Duna", "Intrigas e guerras em um deserto interplanetário.");
    INIT_FILME(16, "Top Gun: Maverick", "Um piloto veterano treina novos recrutas em missões perigosas.");
    INIT_FILME(17, "Toy Story", "Brinquedos ganham vida quando os humanos não estão por perto.");
    INIT_FILME(18, "Procurando Nemo", "Um peixe-palhaço cruza os oceanos para encontrar seu filho.");
    INIT_FILME(19, "Wall-E", "Um robô solitário descobre o amor e salva a humanidade.");

    /* restante vazio */
    for (int i = 20; i < MAX_FILMES; i++) {
        filmes[i].titulo[0] = '\0';
        filmes[i].resumo[0] = '\0';
        filmes[i].somaNotas = filmes[i].numAvaliadores = 0;
    }
}


/* --- Main e Menus --- */
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
        printf(GREEN "               === SISTEMA DE AVALIAÇÃO DE FILMES ===\n" RESET);
        printf("                   1 - Entrar como Usuário\n");
        printf("                   2 - Entrar como Administrador\n");
        printf("                   0 - Sair\n");
        printf("                         Escolha: ");
        if (scanf("%d", &opcao) != 1) {
            limpar_buffer_stdin();
            opcao = -1;
        } else {
            limpar_buffer_stdin();
        }

        switch(opcao) {
            case 1:
                menuUsuario();
                system(CLEAR); // Adicionado: Limpa após sair do menu do Usuário
                break;
            case 2:
                menuAdmin();
                system(CLEAR); // Adicionado: Limpa após sair do menu do Admin
                break;
            case 0:
                printf(YELLOW "                  Saindo... Obrigado!\n" RESET);
                break;
            default:
                printf(RED "                    Opção inválida.\n" RESET);
        }
        if (opcao != 0) {
            printf("                   \nPressione ENTER para continuar...");
            getchar();
        }
    } while (opcao != 0);
}

void menuUsuario() {
    int opc;
    char usuarioLogado[30] = {0};
    int logado = 0;

    do {
        system(CLEAR);
        printf(CYAN "               === MENU USUÁRIO ===\n" RESET);
        if (!logado) {
            printf("                 1 - Login\n");
            printf("                 2 - Cadastrar novo usuário\n");
            printf("                 0 - Voltar\n");
            printf("                 Escolha: ");
            if (scanf("%d", &opc) != 1) { limpar_buffer_stdin(); opc = -1; }
            else limpar_buffer_stdin();

            if (opc == 1) {
                if (loginUsuario(usuarioLogado)) {
                    logado = 1;
                    printf(GREEN "                Login bem-sucedido. Bem-vindo(a), %s!\n" RESET, usuarioLogado);
                }
                else { printf(RED "                 Login falhou: Nome de usuário ou senha incorretos.\n" RESET); }
            } else if (opc == 2) {
                if (cadastrarUsuario()) printf(GREEN "              Usuário cadastrado. Faça o login para continuar.\n" RESET);
                else printf(RED "                 Cadastro cancelado ou falhou.\n" RESET);
            } else if (opc == 0) {
                break;
            } else {
                printf(RED "              Opção inválida.\n" RESET);
            }
        } else {
            /* após login, opções do usuário */
            printf("                 Usuário: %s\n", usuarioLogado);
            printf("                 1 - Listar filmes\n");
            printf("                 2 - Avaliar um filme\n");
            printf("                 3 - Ver minhas avaliações\n");
            printf("                 0 - Logout\n");
            printf("                 Escolha: ");
            if (scanf("%d", &opc) != 1) { limpar_buffer_stdin(); opc = -1; }
            else limpar_buffer_stdin();

            if (opc == 1) {
                listarFilmesPaginado();
                system(CLEAR); // Adicionado: Limpa após a paginação
            } else if (opc == 2) {
                avaliarFilmePorUsuario(usuarioLogado);
                system(CLEAR); // Adicionado: Limpa após a avaliação
            } else if (opc == 3) {
                /* listar avaliações do usuário */
                system(CLEAR);
                printf(BLUE "               === Minhas Avaliações (%s) ===\n" RESET, usuarioLogado);
                int encontrou = 0;
                for (int i = 0; i < totalAvaliacoes; i++) {
                    if (strcmp(avaliacoes[i].nomeUsuario, usuarioLogado) == 0) {
                        encontrou = 1;
                        printf(YELLOW "\nFilme: %s\n" RESET, filmes[avaliacoes[i].indiceFilme].titulo);
                        printf("                      Nota: %d / 5\n", avaliacoes[i].nota);
                        if (avaliacoes[i].comentario[0]) printf("                 Comentário: %s\n", avaliacoes[i].comentario);
                    }
                }
                if (!encontrou) printf("                   Nenhuma avaliação feita por você ainda.\n");
            } else if (opc == 0) {
                logado = 0;
                usuarioLogado[0] = '\0';
                printf(YELLOW "                    Logout realizado.\n" RESET);
                break;
            } else {
                printf(RED "                    Opção inválida.\n" RESET);
            }
        }

        /* Só pausa se não for a opção de sair do submenu ou falha no scanf na tela de login */
        if (opc != 0) {
            printf("               \nPressione ENTER para continuar...");
            getchar();
        }

    } while (1);
}

void menuAdmin() {
    char login[30], senha[30];
    system(CLEAR);
    printf(BLUE "              === LOGIN ADMIN ===\n" RESET);
    printf("                Usuário: ");
    fgets(login, sizeof(login), stdin);
    limpar_nova_linha(login);
    printf("              Senha: ");
    fgets(senha, sizeof(senha), stdin);
    limpar_nova_linha(senha);

    if (strcmp(login, "admin") == 0 && strcmp(senha, "1234") == 0) {
        int opc;
        do {
            system(CLEAR);
            printf(GREEN "              === MENU ADMIN ===\n" RESET);
            printf("                 1 - Adicionar filme\n");
            printf("                 2 - Listar filmes\n");
            printf("                 3 - Remover filme\n");
            printf("                 0 - Voltar\n");
            printf("                 Escolha: ");
            if (scanf("%d", &opc) != 1) { limpar_buffer_stdin(); opc = -1; }
            else limpar_buffer_stdin();

            if (opc == 1) {
                adicionarFilme();
                system(CLEAR); // Adicionado: Limpa após adicionar filme
            } else if (opc == 2) {
                listarFilmesPaginado();
                system(CLEAR); // Adicionado: Limpa após listar filmes
            } else if (opc == 3) {
                removerFilme();
                system(CLEAR); // Adicionado: Limpa após remover filme
            } else if (opc == 0) {
                break;
            } else {
                printf(RED "              Opção inválida.\n" RESET);
            }

            if (opc != 0) {
                printf("              \nPressione ENTER para continuar...");
                getchar();
            }
        } while (1);
    } else {
        printf(RED "                Acesso negado: Usuário ou senha do admin incorretos.\n" RESET);
    }
}

/* Lista filmes em páginas de 10 */
void listarFilmesPaginado() {
    if (totalFilmes == 0) {
        printf(YELLOW "                Nenhum filme cadastrado.\n" RESET);
        return;
    }

    int pagina = 0;
    const int FILMES_POR_PAGINA = 10;
    int totalPaginas = (totalFilmes + FILMES_POR_PAGINA - 1) / FILMES_POR_PAGINA;
    if (totalPaginas == 0) totalPaginas = 1;

    while (1) {
        system(CLEAR);
        printf(BLUE "                === Catálogo de Filmes (página %d/%d) ===\n" RESET, pagina + 1, totalPaginas);
        int inicio = pagina * FILMES_POR_PAGINA;
        int fim = inicio + FILMES_POR_PAGINA;
        if (fim > totalFilmes) fim = totalFilmes;

        for (int i = inicio; i < fim; i++) {
            float media = 0.0f;
            if (filmes[i].numAvaliadores > 0) media = (float)filmes[i].somaNotas / filmes[i].numAvaliadores;
            printf(YELLOW "\n%d) %s\n" RESET, i + 1, filmes[i].titulo);
            printf("                Média: %.2f / 5.00 (%d avaliações)\n", media, filmes[i].numAvaliadores);
            printf("                 Resumo: %.150s\n", filmes[i].resumo);
        }

        printf("               \nN - próxima página | P - anterior | 0 - sair\nEscolha: ");
        char op[10];
        if (!fgets(op, sizeof(op), stdin)) break;

        if (op[0] == 'N' || op[0] == 'n') {
            if (pagina < totalPaginas - 1) pagina++;
            else printf(RED "  Última página atingida.\n" RESET);
        } else if (op[0] == 'P' || op[0] == 'p') {
            if (pagina > 0) pagina--;
            else printf(RED "  Primeira página atingida.\n" RESET);
        } else if (op[0] == '0') {
            break;
        } else {
            printf(RED "  Opção desconhecida.\n" RESET);
        }

        if (op[0] != '0') {
            printf("               \nPressione ENTER para continuar...");
            getchar(); /* Pausa para ver a mensagem de erro/limite */
        }
    }
}

/* Adicionar filme (admin) */
void adicionarFilme() {
    if (totalFilmes >= MAX_FILMES) {
        printf(RED "             Limite de filmes (%d) atingido.\n" RESET, MAX_FILMES);
        return;
    }
    system(CLEAR);
    printf(BLUE "              === Adicionar Filme ===\n" RESET);
    printf("                Título (máx 49 chars): ");
    fgets(filmes[totalFilmes].titulo, sizeof(filmes[totalFilmes].titulo), stdin);
    limpar_nova_linha(filmes[totalFilmes].titulo);

    printf("                Resumo (máx 199 chars): ");
    fgets(filmes[totalFilmes].resumo, sizeof(filmes[totalFilmes].resumo), stdin);
    limpar_nova_linha(filmes[totalFilmes].resumo);

    filmes[totalFilmes].somaNotas = 0;
    filmes[totalFilmes].numAvaliadores = 0;
    totalFilmes++;
    salvarDados();
    printf(GREEN "             Filme adicionado com sucesso: %s\n" RESET, filmes[totalFilmes - 1].titulo);
}

/* Remover filme (admin) */
void removerFilme() {
    if (totalFilmes == 0) { printf(YELLOW "              Nenhum filme para remover.\n" RESET); return; }
    listarFilmesPaginado();
    printf("                  \nDigite o número do filme a remover (0 para cancelar): ");
    int id;
    if (scanf("%d", &id) != 1) { limpar_buffer_stdin(); printf(RED "Entrada inválida.\n" RESET); return; }
    limpar_buffer_stdin();

    if (id == 0) { printf(YELLOW "              Operação cancelada.\n" RESET); return; }
    if (id < 1 || id > totalFilmes) { printf(RED "              ID inválido.\n" RESET); return; }
    id--; /* ID é 1-based, índice é 0-based */

    printf(YELLOW "              Confirma a remoção do filme '%s'? (S/n): " RESET, filmes[id].titulo);
    char confirmacao[10];
    if (!fgets(confirmacao, sizeof(confirmacao), stdin) || (confirmacao[0] != 'S' && confirmacao[0] != 's')) {
          printf(YELLOW "              Remoção cancelada.\n" RESET); return;
    }

    /* 1. Remove o filme */
    for (int i = id; i < totalFilmes - 1; i++) filmes[i] = filmes[i + 1];
    totalFilmes--;

    /* 2. Remove e corrige as avaliações */
    int dst = 0;
    for (int i = 0; i < totalAvaliacoes; i++) {
        if (avaliacoes[i].indiceFilme == id) continue; /* pula avaliações do filme removido */
        if (avaliacoes[i].indiceFilme > id) {
            avaliacoes[i].indiceFilme--; /* corrige o índice */
        }
        avaliacoes[dst++] = avaliacoes[i];
    }
    totalAvaliacoes = dst;

    salvarDados();
    printf(GREEN "              Filme removido com sucesso.\n" RESET);
}

/* Avaliar filme por usuário logado */
void avaliarFilmePorUsuario(const char *usuario) {
    if (totalFilmes == 0) { printf(RED "              Nenhum filme cadastrado para avaliar.\n" RESET); return; }
    listarFilmesPaginado();
    printf("             \nDigite o número do filme que deseja avaliar (0 para cancelar): ");
    int id;
    if (scanf("%d", &id) != 1) { limpar_buffer_stdin(); printf(RED "              Entrada inválida.\n" RESET); return; }
    limpar_buffer_stdin();

    if (id == 0) { printf(YELLOW "              Operação cancelada.\n" RESET); return; }
    if (id < 1 || id > totalFilmes) { printf(RED "ID inválido.\n" RESET); return; }
    id--; /* ID é 1-based, índice é 0-based */

    int nota;
    do {
        printf("              Digite a nota para '%s' (1 a 5): ", filmes[id].titulo);
        if (scanf("%d", &nota) != 1) { limpar_buffer_stdin(); nota = -1; }
        else limpar_buffer_stdin();

        if (nota < 1 || nota > 5) printf(RED "              Nota inválida. Tente novamente.\n" RESET);
    } while (nota < 1 || nota > 5);

    char comentario[81] = {0};
    printf("                  Comentário (opcional, até 80 chars). Deixe em branco para pular:\n");
    fgets(comentario, sizeof(comentario), stdin);
    limpar_nova_linha(comentario);

    /* Verifica se o usuário já avaliou este filme */
    int avaliacao_existente_idx = -1;
    for (int i = 0; i < totalAvaliacoes; i++) {
        if (avaliacoes[i].indiceFilme == id && strcmp(avaliacoes[i].nomeUsuario, usuario) == 0) {
            avaliacao_existente_idx = i;
            break;
        }
    }

    if (avaliacao_existente_idx != -1) {
        /* Atualiza a avaliação existente */
        int nota_antiga = avaliacoes[avaliacao_existente_idx].nota;
        filmes[id].somaNotas = filmes[id].somaNotas - nota_antiga + nota;
        filmes[id].somaNotas = (filmes[id].somaNotas < 0) ? 0 : filmes[id].somaNotas; // Garantir não negativo
        avaliacoes[avaliacao_existente_idx].nota = nota;
        strncpy(avaliacoes[avaliacao_existente_idx].comentario, comentario, sizeof(avaliacoes[avaliacao_existente_idx].comentario)-1);
        printf(GREEN "               Avaliação atualizada. Obrigado!\n" RESET);

    } else {
        /* Cria nova avaliação */
        if (totalAvaliacoes < MAX_AVALIACOES) {
            avaliacoes[totalAvaliacoes].indiceFilme = id;
            strncpy(avaliacoes[totalAvaliacoes].nomeUsuario, usuario, sizeof(avaliacoes[totalAvaliacoes].nomeUsuario)-1);
            avaliacoes[totalAvaliacoes].nomeUsuario[sizeof(avaliacoes[totalAvaliacoes].nomeUsuario)-1] = '\0';
            avaliacoes[totalAvaliacoes].nota = nota;
            strncpy(avaliacoes[totalAvaliacoes].comentario, comentario, sizeof(avaliacoes[totalAvaliacoes].comentario)-1);
            avaliacoes[totalAvaliacoes].comentario[sizeof(avaliacoes[totalAvaliacoes].comentario)-1] = '\0';

            filmes[id].somaNotas += nota;
            filmes[id].numAvaliadores += 1;
            totalAvaliacoes++;
            printf(GREEN "              Nova avaliação registrada. Obrigado!\n" RESET);
        } else {
            printf(RED             "Limite de avaliações atingido, avaliação não registrada.\n" RESET);
            return;
        }
    }

    salvarDados();
}

/* Cadastro e login simples */
int cadastrarUsuario() {
    if (totalUsuarios >= MAX_USUARIOS) { printf(RED "               Limite de usuários (%d) atingido.\n" RESET, MAX_USUARIOS); return 0; }
    char nome[30], senha[30];
    printf("                Nome (sem espaços, máx 29 chars): ");
    if (!fgets(nome, sizeof(nome), stdin)) return 0;
    limpar_nova_linha(nome);

    if (strlen(nome) == 0) { printf(RED "              Nome não pode ser vazio.\n" RESET); return 0; }
    if (encontrarUsuario(nome) != -1) { printf(RED "Usuário '%s' já existe.\n" RESET, nome); return 0; }

    printf("                 Senha (máx 29 chars): ");
    if (!fgets(senha, sizeof(senha), stdin)) return 0;
    limpar_nova_linha(senha);

    if (strlen(senha) == 0) { printf(RED "                Senha não pode ser vazia.\n" RESET); return 0; }

    strncpy(usuarios[totalUsuarios].nome, nome, sizeof(usuarios[totalUsuarios].nome)-1);
    usuarios[totalUsuarios].nome[sizeof(usuarios[totalUsuarios].nome)-1] = '\0';
    strncpy(usuarios[totalUsuarios].senha, senha, sizeof(usuarios[totalUsuarios].senha)-1);
    usuarios[totalUsuarios].senha[sizeof(usuarios[totalUsuarios].senha)-1] = '\0';
    totalUsuarios++;
    salvarDados();
    return 1;
}

int loginUsuario(char *usuarioSaida) {
    char nome[30], senha[30];
    printf("                Nome: ");
    if (!fgets(nome, sizeof(nome), stdin)) return 0;
    limpar_nova_linha(nome);

    printf("                  Senha: ");
    if (!fgets(senha, sizeof(senha), stdin)) return 0;
    limpar_nova_linha(senha);

    int idx = encontrarUsuario(nome);

    if (idx == -1) return 0; /* Usuário não encontrado */

    if (strcmp(usuarios[idx].senha, senha) == 0) {
        strncpy(usuarioSaida, nome, 29);
        usuarioSaida[29] = '\0';
        return 1;
    }
    return 0; /* Senha incorreta */
}

int encontrarUsuario(const char *nome) {
    for (int i = 0; i < totalUsuarios; i++) {
        if (strcmp(usuarios[i].nome, nome) == 0) return i;
    }
    return -1;
}