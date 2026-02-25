#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ========================================================================== */
/* CONFIGURAÇÕES E CONSTANTES                                                */
/* ========================================================================== */

#ifdef _WIN32
#include <windows.h>
#define CLEAR_CMD "cls"
#else
#include <sys/ioctl.h>
#include <unistd.h>
#define CLEAR_CMD "clear"
#endif

// Cores ANSI
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define YELLOW "\033[1;33m"
#define CYAN "\033[1;36m"
#define MAGENTA "\033[1;35m"
#define WHITE "\033[1;37m"
#define RESET "\033[0m"

#define MAX_FILMES 50
#define MAX_USUARIOS 100
#define MAX_AVALIACOES 1000

const char *ARQ_FILMES = "filmes.bin";
const char *ARQ_USUARIOS = "usuarios.bin";
const char *ARQ_AVALIACOES = "avaliacoes.bin";

/* ========================================================================== */
/* ASCII ART                                                                 */
/* ========================================================================== */

const char *ART_MAIN[] = {"   _____       _                            ",
                          "  / ____|     (_)                           ",
                          " | |      ____ _ _ __   ___ _ __ ___   __ _ ",
                          " | |     |____| | '_ \\ / _ \\ '_ ` _ \\ / _` |",
                          " | |____      | | | | |  __/ | | | | | (_| |",
                          "  \\_____|     |_|_| |_|\\___|_| |_| |_|\\__,_|",
                          "                                            "};

const char *ART_USER[] = {
    "  _   _               ",   " | | | |              ",
    " | | | |___  ___ _ __ ",   " | | | / __|/ _ \\ '__|",
    " | |_| \\__ \\  __/ |   ", "  \\___/|___/\\___|_|   ",
    "                      "};

// NOVO ASCII ART "ADMIN"
const char *ART_ADMIN[] = {
    "           _taff      ",    "     /\\   | |         ",
    "    /  \\  | |__  ___  ",   "   / /\\ \\ | '_ \\/ __| ",
    "  / ____ \\| |_) \\__ \\ ", " /_/    \\_\\_.__/|___/ ",
    "                      "};

const char *ART_CATALOGO[] = {
    "   _____      _        _                  ",
    "  / ____|    | |      | |                 ",
    " | |     __ _| |_ __ _| | ___   __ _  ___ ",
    " | |    / _` | __/ _` | |/ _ \\ / _` |/ _ \\",
    " | |___| (_| | || (_| | | (_) | (_| | (_) |",
    "  \\_____\\__,_|\\__\\__,_|_|\\___/ \\__, |\\___/",
    "                                __/ |     ",
    "                               |___/      "};

const char *ART_LOGIN[] = {
    "  _                _       ",   " | |    ___   __ _(_)_ __  ",
    " | |   / _ \\ / _` | | '_ \\ ", " | |__| (_) | (_| | | | | |",
    " |_____\\___/ \\__, |_|_| |_|", "             |___/         "};

/* ========================================================================== */
/* ESTRUTURAS DE DADOS                                    */
/* ========================================================================== */

typedef struct {
  char titulo[50];
  char resumo[200];
  int somaNotas;
  int numAvaliadores;
  int numComentarios;
} Filme;

typedef struct {
  char nome[30];
  char senha[30];
} Usuario;

typedef struct {
  int idFilme;
  char nomeUsuario[30];
  int nota;
  char comentario[81];
} Avaliacao;

typedef struct {
  Filme filmes[MAX_FILMES];
  int qtdFilmes;
  Usuario usuarios[MAX_USUARIOS];
  int qtdUsuarios;
  Avaliacao avaliacoes[MAX_AVALIACOES];
  int qtdAvaliacoes;
} Sistema;

/* ========================================================================== */
/* PROTÓTIPOS                                                                */
/* ========================================================================== */

// Layout e Utils
void getTerminalSize(int *rows, int *cols);
void limparTela(void);
void centralizarVerticalmente(int alturaConteudo);
void centerText(const char *text, const char *color);
void printSeparator(const char *color);
void printHeader(const char *title, const char *color);
void imprimirAsciiCentralizado(const char *art[], int linhas,
                               const char *color);
void imprimirBoxMenu(const char *opcoes[], int qtd);
int exibirMenuComAscii(const char *art[], int linhasArt, const char *corArt,
                       const char *opcoes[], int qtdOpcoes);
void pausar(void);
void lerString(char *buffer, int tamanho, const char *msg);
int lerInteiro(const char *msg);

// Lógica Core
void seedFilmes(Sistema *sys);
void carregarSistema(Sistema *sys);
void salvarSistema(Sistema *sys);
int buscarUsuario(Sistema *sys, const char *nome);
int autenticarUsuario(Sistema *sys, const char *nome, const char *senha);
int cadastrarUsuarioLogic(Sistema *sys, const char *nome, const char *senha);
void adicionarAvaliacaoLogic(Sistema *sys, int idFilme, const char *usuario,
                             int nota, const char *comentario);

// Funcionalidades Requisitadas
void listarFilmesPaginado(Sistema *sys);
void exibirTopFilmes(Sistema *sys, int invertido);
void verAvaliacoesPorFilme(Sistema *sys);
void menuPrincipal(Sistema *sys);
void menuUsuario(Sistema *sys);
void menuAdmin(Sistema *sys);

/* ========================================================================== */
/* MAIN                                                                      */
/* ========================================================================== */

int main(void) {
  Sistema sys = {0};
  carregarSistema(&sys);
  menuPrincipal(&sys);
  salvarSistema(&sys);
  return 0;
}

/* ========================================================================== */
/* LAYOUT E INPUT                                                            */
/* ========================================================================== */

void getTerminalSize(int *rows, int *cols) {
#ifdef _WIN32
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
  *cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
  *rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#else
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  *rows = w.ws_row;
  *cols = w.ws_col;
#endif
  if (*cols <= 0)
    *cols = 80;
  if (*rows <= 0)
    *rows = 24;
}

void limparTela(void) { system(CLEAR_CMD); }

void centralizarVerticalmente(int alturaConteudo) {
  int rows, cols;
  getTerminalSize(&rows, &cols);
  int padding = (rows - alturaConteudo) / 2;
  if (padding < 0)
    padding = 0;
  for (int i = 0; i < padding; i++)
    printf("\n");
}

void centerText(const char *text, const char *color) {
  int rows, cols;
  getTerminalSize(&rows, &cols);
  int len = (int)strlen(text);
  int padding = (cols - len) / 2;
  if (padding < 0)
    padding = 0;
  printf("%*s%s%s%s\n", padding, "", color, text, RESET);
}

void printSeparator(const char *color) {
  int rows, cols;
  getTerminalSize(&rows, &cols);
  printf("%s", color);
  for (int i = 0; i < cols; i++)
    printf("=");
  printf("%s\n", RESET);
}

void printHeader(const char *title, const char *color) {
  printSeparator(color);
  centerText(title, color);
  printSeparator(color);
}

void imprimirAsciiCentralizado(const char *art[], int linhas,
                               const char *color) {
  int rows, cols;
  getTerminalSize(&rows, &cols);
  int maxLen = 0;
  for (int i = 0; i < linhas; i++) {
    int len = (int)strlen(art[i]);
    if (len > maxLen)
      maxLen = len;
  }
  int padding = (cols - maxLen) / 2;
  if (padding < 0)
    padding = 0;
  printf("%s", color);
  for (int i = 0; i < linhas; i++)
    printf("%*s%s\n", padding, "", art[i]);
  printf("%s", RESET);
}

void imprimirBoxMenu(const char *opcoes[], int qtd) {
  int rows, cols;
  getTerminalSize(&rows, &cols);
  int maxLen = 0;
  for (int i = 0; i < qtd; i++) {
    int len = (int)strlen(opcoes[i]);
    if (len > maxLen)
      maxLen = len;
  }
  int width = maxLen + 6;
  int pad = (cols - width) / 2;
  if (pad < 0)
    pad = 0;
  printf("%*s╔", pad, "");
  for (int i = 0; i < width - 2; i++)
    printf("═");
  printf("╗\n");
  for (int i = 0; i < qtd; i++)
    printf("%*s║  %-*s║\n", pad, "", width - 4, opcoes[i]);
  printf("%*s╚", pad, "");
  for (int i = 0; i < width - 2; i++)
    printf("═");
  printf("╝\n");
}

int exibirMenuComAscii(const char *art[], int linhasArt, const char *corArt,
                       const char *opcoes[], int qtdOpcoes) {
  limparTela();
  int alturaTotal = linhasArt + 1 + qtdOpcoes + 2 + 2;
  centralizarVerticalmente(alturaTotal);
  imprimirAsciiCentralizado(art, linhasArt, corArt);
  printf("\n");
  imprimirBoxMenu(opcoes, qtdOpcoes);
  printf("\n");
  return lerInteiro("Digite sua opcao: ");
}

void pausar(void) {
  printf("\n");
  centerText("Pressione ENTER para continuar...", CYAN);
  getchar();
}

void lerString(char *buffer, int tamanho, const char *msg) {
  int rows, cols;
  getTerminalSize(&rows, &cols);
  if (msg) {
    int pad = (cols - (int)strlen(msg) - 20) / 2;
    if (pad < 0)
      pad = 0;
    printf("%*s%s", pad, "", msg);
  }
  if (fgets(buffer, tamanho, stdin) != NULL) {
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n')
      buffer[len - 1] = '\0';
  } else
    buffer[0] = '\0';
}

int lerInteiro(const char *msg) {
  int rows, cols;
  getTerminalSize(&rows, &cols);
  char buffer[64];
  if (msg) {
    int pad = (cols - (int)strlen(msg) - 5) / 2;
    if (pad < 0)
      pad = 0;
    printf("%*s%s", pad, "", msg);
  }
  if (fgets(buffer, sizeof(buffer), stdin) != NULL)
    return (int)strtol(buffer, NULL, 10);
  return -1;
}

/* ========================================================================== */
/* LÓGICA CORE & PERSISTÊNCIA                                                */
/* ========================================================================== */

void seedFilmes(Sistema *sys) {
  const char *titulos[] = {
      "Matrix",       "Interestelar",  "O Poderoso Chefao",
      "Pulp Fiction", "Clube da Luta", "Vingadores",
      "Titanic",      "O Rei Leao",    "Coringa",
      "Batman",       "Star Wars",     "Senhor dos Aneis",
      "Forrest Gump", "A Origem",      "Cidade de Deus",
      "Parasita",     "Gladiador",     "De Volta para o Futuro",
      "Toy Story",    "Shrek"};
  sys->qtdFilmes = 20;
  for (int i = 0; i < sys->qtdFilmes; i++) {
    snprintf(sys->filmes[i].titulo, 50, "%s", titulos[i]);
    snprintf(sys->filmes[i].resumo, 200,
             "Resumo gerado automaticamente para %s.", titulos[i]);
    sys->filmes[i].somaNotas = 0;
    sys->filmes[i].numAvaliadores = 0;
    sys->filmes[i].numComentarios = 0;
  }
}

void carregarTabela(const char *nomeArq, void *buffer, int tamanhoItem,
                    int *qtd) {
  FILE *f = fopen(nomeArq, "rb");
  if (f) {
    fread(qtd, sizeof(int), 1, f);
    fread(buffer, tamanhoItem, *qtd, f);
    fclose(f);
  } else
    *qtd = 0;
}
void salvarTabela(const char *nomeArq, void *buffer, int tamanhoItem, int qtd) {
  FILE *f = fopen(nomeArq, "wb");
  if (f) {
    fwrite(&qtd, sizeof(int), 1, f);
    fwrite(buffer, tamanhoItem, qtd, f);
    fclose(f);
  }
}
void carregarSistema(Sistema *sys) {
  carregarTabela(ARQ_FILMES, sys->filmes, sizeof(Filme), &sys->qtdFilmes);
  if (sys->qtdFilmes < 20)
    seedFilmes(sys);
  carregarTabela(ARQ_USUARIOS, sys->usuarios, sizeof(Usuario),
                 &sys->qtdUsuarios);
  carregarTabela(ARQ_AVALIACOES, sys->avaliacoes, sizeof(Avaliacao),
                 &sys->qtdAvaliacoes);
}
void salvarSistema(Sistema *sys) {
  salvarTabela(ARQ_FILMES, sys->filmes, sizeof(Filme), sys->qtdFilmes);
  salvarTabela(ARQ_USUARIOS, sys->usuarios, sizeof(Usuario), sys->qtdUsuarios);
  salvarTabela(ARQ_AVALIACOES, sys->avaliacoes, sizeof(Avaliacao),
               sys->qtdAvaliacoes);
}

int buscarUsuario(Sistema *sys, const char *nome) {
  for (int i = 0; i < sys->qtdUsuarios; i++)
    if (strcmp(sys->usuarios[i].nome, nome) == 0)
      return i;
  return -1;
}
int autenticarUsuario(Sistema *sys, const char *nome, const char *senha) {
  int idx = buscarUsuario(sys, nome);
  if (idx != -1 && strcmp(sys->usuarios[idx].senha, senha) == 0)
    return 1;
  return 0;
}
int cadastrarUsuarioLogic(Sistema *sys, const char *nome, const char *senha) {
  if (sys->qtdUsuarios >= MAX_USUARIOS)
    return -1;
  if (buscarUsuario(sys, nome) != -1)
    return 0;
  Usuario *u = &sys->usuarios[sys->qtdUsuarios++];
  strncpy(u->nome, nome, 29);
  strncpy(u->senha, senha, 29);
  salvarSistema(sys);
  return 1;
}
void adicionarAvaliacaoLogic(Sistema *sys, int idFilme, const char *usuario,
                             int nota, const char *comentario) {
  int idx = -1;
  for (int i = 0; i < sys->qtdAvaliacoes; i++) {
    if (sys->avaliacoes[i].idFilme == idFilme &&
        strcmp(sys->avaliacoes[i].nomeUsuario, usuario) == 0) {
      idx = i;
      break;
    }
  }
  if (idx != -1) {
    sys->filmes[idFilme].somaNotas =
        sys->filmes[idFilme].somaNotas - sys->avaliacoes[idx].nota + nota;
    sys->avaliacoes[idx].nota = nota;
    strncpy(sys->avaliacoes[idx].comentario, comentario, 80);
  } else {
    if (sys->qtdAvaliacoes >= MAX_AVALIACOES)
      return;
    Avaliacao *av = &sys->avaliacoes[sys->qtdAvaliacoes++];
    av->idFilme = idFilme;
    strncpy(av->nomeUsuario, usuario, 29);
    av->nota = nota;
    strncpy(av->comentario, comentario, 80);
    sys->filmes[idFilme].somaNotas += nota;
    sys->filmes[idFilme].numAvaliadores++;
    if (strlen(comentario) > 0)
      sys->filmes[idFilme].numComentarios++;
  }
  salvarSistema(sys);
}

/* ========================================================================== */
/* MENUS E FUNCIONALIDADES REQUISITADAS                                      */
/* ========================================================================== */

void menuPrincipal(Sistema *sys) {
  const char *opcoes[] = {"1 - Acesso Usuario", "2 - Acesso Admin", "",
                          "0 - Sair"};
  int linhasArt = sizeof(ART_MAIN) / sizeof(ART_MAIN[0]);
  while (1) {
    int op = exibirMenuComAscii(ART_MAIN, linhasArt, GREEN, opcoes, 4);
    switch (op) {
    case 1:
      menuUsuario(sys);
      break;
    case 2:
      menuAdmin(sys);
      break;
    case 0:
      return;
    default:
      centerText("Opcao invalida!", RED);
      pausar();
    }
  }
}

void listarFilmesPaginado(Sistema *sys) {
  int linhasArt = sizeof(ART_CATALOGO) / sizeof(ART_CATALOGO[0]);
  int pagina = 0;
  int totalPaginas = (sys->qtdFilmes + 9) / 10;

  if (sys->qtdFilmes == 0) {
    centerText("Nenhum filme cadastrado.", YELLOW);
    pausar();
    return;
  }

  while (1) {
    limparTela();
    imprimirAsciiCentralizado(ART_CATALOGO, linhasArt, BLUE);
    printf("\n");
    char infoPag[50];
    snprintf(infoPag, 50, "PAGINA %d de %d", pagina + 1, totalPaginas);
    centerText(infoPag, CYAN);
    printf("\n");

    int inicio = pagina * 10;
    int fim = inicio + 10;
    if (fim > sys->qtdFilmes)
      fim = sys->qtdFilmes;

    for (int i = inicio; i < fim; i++) {
      float media = 0.0;
      if (sys->filmes[i].numAvaliadores > 0)
        media = (float)sys->filmes[i].somaNotas / sys->filmes[i].numAvaliadores;

      printf(YELLOW "  %2d) %-30s " RESET "[Nota: %.1f] (%d votos)\n", i + 1,
             sys->filmes[i].titulo, media, sys->filmes[i].numAvaliadores);
    }

    printf("\n");
    printSeparator(BLUE);
    printf("  [N] Proxima  [P] Anterior  [0] Sair\n");

    char op[10];
    lerString(op, 10, "  Opcao: ");

    if (op[0] == '0')
      break;
    if ((op[0] == 'N' || op[0] == 'n') && pagina < totalPaginas - 1)
      pagina++;
    if ((op[0] == 'P' || op[0] == 'p') && pagina > 0)
      pagina--;
  }
}

typedef struct {
  int id;
  float media;
} Rank;

int cmpDesc(const void *a, const void *b) {
  float diff = ((Rank *)b)->media - ((Rank *)a)->media;
  return (diff > 0) - (diff < 0);
}
int cmpAsc(const void *a, const void *b) {
  float diff = ((Rank *)a)->media - ((Rank *)b)->media;
  return (diff > 0) - (diff < 0);
}

void exibirTopFilmes(Sistema *sys, int invertido) {
  limparTela();
  printHeader(invertido ? "TOP 5 - PIORES AVALIACOES"
                        : "TOP 5 - MELHORES AVALIACOES",
              MAGENTA);

  Rank ranks[MAX_FILMES];
  int count = 0;

  for (int i = 0; i < sys->qtdFilmes; i++) {
    if (sys->filmes[i].numAvaliadores > 0) {
      ranks[count].id = i;
      ranks[count].media =
          (float)sys->filmes[i].somaNotas / sys->filmes[i].numAvaliadores;
      count++;
    }
  }

  if (count == 0) {
    centerText("Ainda nao ha avaliacoes suficientes.", YELLOW);
    pausar();
    return;
  }

  qsort(ranks, count, sizeof(Rank), invertido ? cmpAsc : cmpDesc);

  int limit = (count < 5) ? count : 5;
  for (int i = 0; i < limit; i++) {
    int id = ranks[i].id;
    printf(YELLOW "  #%d %s\n" RESET, i + 1, sys->filmes[id].titulo);
    printf("     Media: %.2f (%d votos)\n\n", ranks[i].media,
           sys->filmes[id].numAvaliadores);
  }
  pausar();
}

void verAvaliacoesPorFilme(Sistema *sys) {
  listarFilmesPaginado(sys);
  int id = lerInteiro("Digite o numero do filme para ver detalhes: ");
  if (id < 1 || id > sys->qtdFilmes)
    return;
  id--;

  limparTela();
  printHeader(sys->filmes[id].titulo, CYAN);
  printf("  %s\n\n", sys->filmes[id].resumo);

  int encontrou = 0;
  for (int i = 0; i < sys->qtdAvaliacoes; i++) {
    if (sys->avaliacoes[i].idFilme == id) {
      printf(YELLOW "  Usuario: %s\n" RESET, sys->avaliacoes[i].nomeUsuario);
      printf("  Nota: %d/5\n", sys->avaliacoes[i].nota);
      if (strlen(sys->avaliacoes[i].comentario) > 0)
        printf("  Comentario: %s\n", sys->avaliacoes[i].comentario);
      printf("  -------------------------\n");
      encontrou = 1;
    }
  }
  if (!encontrou)
    centerText("Nenhuma avaliacao para este filme.", YELLOW);
  pausar();
}

void menuUsuario(Sistema *sys) {
  const char *opcoes[] = {"1 - Login", "2 - Cadastrar", "", "0 - Voltar"};
  int linhasArt = sizeof(ART_USER) / sizeof(ART_USER[0]);
  int linhasLogin = sizeof(ART_LOGIN) / sizeof(ART_LOGIN[0]);

  while (1) {
    int op = exibirMenuComAscii(ART_USER, linhasArt, CYAN, opcoes, 4);
    if (op == 0)
      break;

    if (op == 2) {
      limparTela();
      centralizarVerticalmente(12);
      printHeader("NOVO CADASTRO", CYAN);
      char nome[30], senha[30];
      lerString(nome, 30, "Nome: ");
      lerString(senha, 30, "Senha: ");
      int res = cadastrarUsuarioLogic(sys, nome, senha);
      if (res == 1)
        centerText("Sucesso!", GREEN);
      else
        centerText("Erro/Ja existe!", RED);
      pausar();
    } else if (op == 1) {
      limparTela();
      centralizarVerticalmente(12);
      imprimirAsciiCentralizado(ART_LOGIN, linhasLogin, CYAN);
      printf("\n");
      char nome[30], senha[30];
      lerString(nome, 30, "Usuario: ");
      lerString(senha, 30, "Senha: ");

      if (autenticarUsuario(sys, nome, senha)) {
        const char *opsLogado[] = {"1 - Catalogo",
                                   "2 - Avaliar Filme",
                                   "3 - Detalhes/Avaliacoes de um Filme",
                                   "4 - Minhas Avaliacoes",
                                   "5 - Top 5 Melhores",
                                   "6 - Top 5 Piores",
                                   "",
                                   "0 - Logout"};
        while (1) {
          char titulo[60];
          snprintf(titulo, 60, "OLA, %s", nome);
          int sel =
              exibirMenuComAscii(ART_USER, linhasArt, GREEN, opsLogado, 8);

          if (sel == 0)
            break;
          if (sel == 1)
            listarFilmesPaginado(sys);
          else if (sel == 2) {
            listarFilmesPaginado(sys);
            int id = lerInteiro("ID do Filme para Avaliar: ");
            if (id > 0 && id <= sys->qtdFilmes) {
              limparTela();
              centralizarVerticalmente(10);
              printf(YELLOW);
              centerText(sys->filmes[id - 1].titulo, YELLOW);
              printf(RESET);
              int nota = lerInteiro("Nota (1-5): ");
              if (nota < 1 || nota > 5) {
                centerText("Nota invalida!", RED);
                pausar();
                continue;
              }
              char com[81];
              lerString(com, 81, "Comentario (max 80): ");
              adicionarAvaliacaoLogic(sys, id - 1, nome, nota, com);
              centerText("Avaliacao registrada!", GREEN);
            }
            pausar();
          } else if (sel == 3)
            verAvaliacoesPorFilme(sys);
          else if (sel == 5)
            exibirTopFilmes(sys, 0);
          else if (sel == 6)
            exibirTopFilmes(sys, 1);
          else if (sel == 4) {
            limparTela();
            printHeader("MINHAS AVALIACOES", BLUE);
            for (int i = 0; i < sys->qtdAvaliacoes; i++)
              if (strcmp(sys->avaliacoes[i].nomeUsuario, nome) == 0)
                printf("Filme: %s - Nota: %d\n",
                       sys->filmes[sys->avaliacoes[i].idFilme].titulo,
                       sys->avaliacoes[i].nota);
            pausar();
          }
        }
      } else {
        centerText("Credenciais Invalidas!", RED);
        pausar();
      }
    }
  }
}

void menuAdmin(Sistema *sys) {
  int linhasArt = sizeof(ART_ADMIN) / sizeof(ART_ADMIN[0]);
  int linhasLogin = sizeof(ART_LOGIN) / sizeof(ART_LOGIN[0]);

  limparTela();
  centralizarVerticalmente(12);
  imprimirAsciiCentralizado(ART_LOGIN, linhasLogin, MAGENTA);
  printf("\n");
  char l[30], s[30];
  lerString(l, 30, "User: ");
  lerString(s, 30, "Pass: ");

  if (strcmp(l, "admin") != 0 || strcmp(s, "1234") != 0) {
    centerText("Negado!", RED);
    pausar();
    return;
  }

  // ATUALIZADO: Menu Admin com a opção de listar filmes
  const char *opcoes[] = {"1 - Adicionar Filme",
                          "2 - Listar Filmes",
                          "3 - Remover Filme",
                          "4 - Listar Usuarios",
                          "",
                          "0 - Sair"};

  while (1) {
    int op = exibirMenuComAscii(ART_ADMIN, linhasArt, MAGENTA, opcoes, 6);

    if (op == 0)
      break;

    if (op == 4) { // Listar Usuários
      limparTela();
      printHeader("USUARIOS", BLUE);
      for (int i = 0; i < sys->qtdUsuarios; i++)
        printf("%d) %s\n", i + 1, sys->usuarios[i].nome);
      pausar();
    } else if (op == 2) { // NOVO: Listar Filmes
      listarFilmesPaginado(sys);
    } else if (op == 1) { // Adicionar Filme
      if (sys->qtdFilmes >= MAX_FILMES) {
        centerText("Cheio!", RED);
      } else {
        Filme *f = &sys->filmes[sys->qtdFilmes];
        limparTela();
        printHeader("NOVO FILME", GREEN);
        lerString(f->titulo, 50, "Titulo: ");
        lerString(f->resumo, 200, "Resumo: ");
        f->somaNotas = 0;
        f->numAvaliadores = 0;
        f->numComentarios = 0;
        sys->qtdFilmes++;
        salvarSistema(sys);
        centerText("Adicionado!", GREEN);
      }
      pausar();
    } else if (op == 3) { // Remover Filme
      listarFilmesPaginado(sys);
      int id = lerInteiro("ID para remover: ");
      if (id > 0 && id <= sys->qtdFilmes) {
        id--;
        for (int i = id; i < sys->qtdFilmes - 1; i++)
          sys->filmes[i] = sys->filmes[i + 1];
        sys->qtdFilmes--;
        salvarSistema(sys);
        centerText("Removido!", YELLOW);
      }
      pausar();
    }
  }
}
