// Bibliotecas utilizadas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// Compatibilidade com Windows para UTF-8
#ifdef _WIN32
#include <windows.h>
#endif

// Constantes e definições de estruturas
#define MAX 10
#define FILA 10

typedef struct {
    char enunciado[200];
    int resposta;
} Pergunta;

typedef struct {
    int dados[FILA];
    int inicio, fim, qtd;
} Fila;

// Declaração das funções usadas no programa
int lerInteiroVal(const char *prompt, long min, long max);
double lerDoubleVal(const char *prompt, int disallowZero);
void inicializarFila(Fila *f);
int filaVazia(Fila *f);
int filaCheia(Fila *f);
void enfileirar(Fila *f, int valor);
void mostrarFila(Fila *f);
void calculadora(void);
void limparTela(void);

// Função para limpar a tela do console
void limparTela(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Funções de entrada com validação
int lerInteiroVal(const char *prompt, long min, long max) {
    char buf[256];
    char *endptr;
    long val;

    while (1) {
        printf("%s", prompt);
        fflush(stdout);

        if (!fgets(buf, sizeof(buf), stdin)) {
            clearerr(stdin);
            printf("Voce digitou errado por favor tente novamente\n");
            continue;
        }

        size_t ln = strlen(buf);
        while (ln > 0 && (buf[ln - 1] == '\n' || buf[ln - 1] == '\r')) {
            buf[--ln] = '\0';
        }

        val = strtol(buf, &endptr, 10);
        while (*endptr == ' ' || *endptr == '\t') endptr++;

        if (endptr != buf && *endptr == '\0') {
            if (min <= max && (val < min || val > max)) {
                printf("Voce digitou errado por favor tente novamente\n");
                continue;
            }
            return (int)val;
        } else {
            printf("Voce digitou errado por favor tente novamente\n");
        }
    }
}

// Função para ler um valor double com validação
double lerDoubleVal(const char *prompt, int disallowZero) {
    char buf[256];
    char *endptr;
    double val;

    while (1) {
        printf("%s", prompt);
        fflush(stdout);

        if (!fgets(buf, sizeof(buf), stdin)) {
            clearerr(stdin);
            printf("Voce digitou errado por favor tente novamente\n");
            continue;
        }

        size_t ln = strlen(buf);
        while (ln > 0 && (buf[ln - 1] == '\n' || buf[ln - 1] == '\r')) {
            buf[--ln] = '\0';
        }

        val = strtod(buf, &endptr);
        while (*endptr == ' ' || *endptr == '\t') endptr++;

        if (endptr != buf && *endptr == '\0') {
            if (disallowZero && val == 0.0) {
                printf("Voce digitou errado por favor tente novamente\n");
                continue;
            }
            return val;
        } else {
            printf("Voce digitou errado por favor tente novamente\n");
        }
    }
}

// Funções para manipulação da fila circular
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = -1;
    f->qtd = 0;
}
int filaVazia(Fila *f) { return f->qtd == 0; }
int filaCheia(Fila *f) { return f->qtd == FILA; }

// Enfileira um valor na fila, removendo o mais antigo se estiver cheia
void enfileirar(Fila *f, int valor) {
    if (filaCheia(f)) {
        f->inicio = (f->inicio + 1) % FILA;
        f->qtd--;
    }
    f->fim = (f->fim + 1) % FILA;
    f->dados[f->fim] = valor;
    f->qtd++;
}

// Mostra o conteúdo da fila
void mostrarFila(Fila *f) {
    printf("\n=== Historico de Respostas ===\n");
    if (filaVazia(f)) {
        printf("Nenhuma resposta registrada\n");
        return;
    }
    int i = f->inicio;
    for (int c = 0; c < f->qtd; c++) {
        if (f->dados[i] == 1) {
            printf("Questao %d: Correta\n", c + 1);
        } else if (f->dados[i] == 2) {
            printf("Questao %d: Correta segunda tentativa\n", c + 1);
        } else {
            printf("Questao %d: Errada\n", c + 1);
        }
        i = (i + 1) % FILA;
    }
}
 // Função da calculadora simples
void calculadora() {
    char prompt[256];
    printf("\n=== Calculadora uma operacao ===\n");
    printf("1 - Soma\n");
    printf("2 - Subtracao\n");
    printf("3 - Multiplicacao\n");
    printf("4 - Divisao\n");
    printf("0 - Voltar\n");

    int opcao = lerInteiroVal("Escolha: ", 0, 4);
    if (opcao == 0) return;

    const char *opName = "";
    switch (opcao) {
        case 1: opName = "Soma"; break;
        case 2: opName = "Subtracao"; break;
        case 3: opName = "Multiplicacao"; break;
        case 4: opName = "Divisao"; break;
    }

    snprintf(prompt, sizeof(prompt), "%s - Digite o primeiro numero: ", opName);
    double a = lerDoubleVal(prompt, 0);

    if (opcao == 4) {
        snprintf(prompt, sizeof(prompt), "%s - Digite o segundo numero nao pode ser 0: ", opName);
        double b = lerDoubleVal(prompt, 1);
        printf("Resultado: %.2lf\n", a / b);
    } else {
        snprintf(prompt, sizeof(prompt), "%s - Digite o segundo numero: ", opName);
        double b = lerDoubleVal(prompt, 0);
        double resultado = 0.0;
        if (opcao == 1) resultado = a + b;
        else if (opcao == 2) resultado = a - b;
        else if (opcao == 3) resultado = a * b;
        printf("Resultado: %.2lf\n", resultado);
    }
}

// Função principal com tratamento de exceções
int main(void) {
#ifdef _WIN32
    SetConsoleOutputCP(65001);  // saida UTF-8
    SetConsoleCP(65001);        // entrada UTF-8
#endif

// Definição das perguntas do quiz
    Pergunta quiz[MAX] = {
        {"Quanto e 7 x 7", 49},
        {"Resolva 12 + 15", 27},
        {"Qual e a raiz quadrada de 81", 9},
        {"Resolva 45 - 19", 26},
        {"Quanto e 6 x 9", 54},
        {"Resolva 35 / 7", 5},
        {"Quanto e 269 + 37", 306},
        {"Qual e o resultado de 6 x 10 x 4", 240},
        {"Quanto e (12 x 11) + 40", 172},
        {"Resolva 259 / 7", 37}
    };

    Fila historico;
    inicializarFila(&historico);

    int pontos = 0;
    int pontosUltimaQuestao = 0;

    limparTela();
    printf("Bem vindos ao Matematequiz, o jogo de de perguntas e respostas de matematica basica em C\n");
    int iniciar = lerInteiroVal("Deseja comecar o jogo 1 = Sim 0 = Nao: ", 0, 1);
    if (iniciar == 0) {
        printf("Programa encerrado Ate a proxima\n");
        return 0;
    }

    for (int i = 0; i < MAX; i++) {
        limparTela();

        if (i > 0) {
            printf("Pontuacao da ultima questao: %d\n", pontosUltimaQuestao);
            printf("Pontuacao acumulada: %d\n\n", pontos);
        }

        printf("\n--- Pergunta %d ---\n", i + 1);
        printf("%s\n", quiz[i].enunciado);

        int usarCalc = lerInteiroVal("Deseja usar a calculadora antes de responder 1 = Sim 0 = Nao: ", 0, 1);
        if (usarCalc == 1) {
            calculadora();
        }

        int resposta = lerInteiroVal("Sua resposta: ", INT_MIN, INT_MAX);

        int pontosRodada = 0;

        if (resposta == quiz[i].resposta) {
            if (usarCalc == 1) {
                pontosRodada = 1;
                printf("Voce acertou mas usou a calculadora 1 ponto\n");
            } else {
                pontosRodada = 3;
                printf("Voce acertou sem ajuda 3 pontos\n");
            }
            pontos += pontosRodada;
            enfileirar(&historico, 1);
        } else {
            enfileirar(&historico, 0);
            printf("Que pena voce errou\n");

            int tentarNovamente = lerInteiroVal("Deseja tentar novamente esta questao 1 = Sim 0 = Nao: ", 0, 1);
            int acertouSegundaTentativa = 0;

            if (tentarNovamente == 1) {
                limparTela();
                printf("\n--- Pergunta %d Segunda tentativa ---\n", i + 1);
                printf("%s\n", quiz[i].enunciado);

                resposta = lerInteiroVal("Sua nova resposta: ", INT_MIN, INT_MAX);
                if (resposta == quiz[i].resposta) {
                    pontosRodada = 1;
                    pontos += pontosRodada;
                    acertouSegundaTentativa = 1;
                    enfileirar(&historico, 2);
                    printf("Correto na segunda tentativa 1 ponto\n");
                } else {
                    printf("Ainda errado\n");
                }
            }

            if (!acertouSegundaTentativa) {
                int verResposta = lerInteiroVal("Deseja ver a resposta correta 1 = Sim 0 = Nao: ", 0, 1);
                if (verResposta == 1) {
                    printf("Resposta correta: %d\n", quiz[i].resposta);
                }
            }
        }

        pontosUltimaQuestao = pontosRodada;
        printf("Pontuacao desta questao: %d\n", pontosRodada);
        printf("Pontuacao acumulada: %d\n", pontos);
        printf("\nPressione Enter para continuar...");
        getchar();
    }

    limparTela();
    printf("\n=== Resultado Final ===\n");
    printf("Pontuacao: %d de %d possiveis\n", pontos, MAX * 3);

    mostrarFila(&historico);

    return 0;
}