#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAM 5
#define JOGADOR 1
#define INIMIGO 2

// ==========================
// Estruturas
// ==========================

typedef struct {
    int dono;
    int tropas;
} Territorio;

typedef struct {
    char descricao[100];
    int territoriosNecessarios;
} Missao;

// ==========================
// Protótipos
// ==========================

Territorio** alocarMapa(int tamanho);
void liberarMapa(Territorio **mapa, int tamanho);
void inicializarMapa(Territorio **mapa, int tamanho);
void exibirMapa(Territorio **mapa, int tamanho);
void faseDeAtaque(Territorio **mapa);
int simularAtaque(int tropasAtacante, int tropasDefensor);
Missao sortearMissao();
int verificarVitoria(Territorio **mapa, Missao missao);
void limparBuffer();

// ==========================
// Função Principal
// ==========================

int main() {
    srand(time(NULL));

    Territorio **mapa = alocarMapa(TAM);
    inicializarMapa(mapa, TAM);

    Missao missao = sortearMissao();

    int opcao;
    int venceu = 0;

    do {
        printf("\n===== WAR ESTRUTURADO =====\n");
        printf("1 - Exibir mapa\n");
        printf("2 - Atacar\n");
        printf("3 - Exibir missão\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                exibirMapa(mapa, TAM);
                break;
            case 2:
                faseDeAtaque(mapa);
                venceu = verificarVitoria(mapa, missao);
                if (venceu) {
                    printf("\nMISSÃO CUMPRIDA! VOCÊ VENCEU!\n");
                    opcao = 0;
                }
                break;
            case 3:
                printf("\nMissão: %s\n", missao.descricao);
                break;
            case 0:
                printf("Encerrando jogo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }

    } while(opcao != 0);

    liberarMapa(mapa, TAM);
    return 0;
}

// ==========================
// Implementações
// ==========================

Territorio** alocarMapa(int tamanho) {
    Territorio **mapa = malloc(tamanho * sizeof(Territorio*));
    for (int i = 0; i < tamanho; i++) {
        mapa[i] = malloc(tamanho * sizeof(Territorio));
    }
    return mapa;
}

void liberarMapa(Territorio **mapa, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        free(mapa[i]);
    }
    free(mapa);
}

void inicializarMapa(Territorio **mapa, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            if (rand() % 2 == 0)
                mapa[i][j].dono = JOGADOR;
            else
                mapa[i][j].dono = INIMIGO;

            mapa[i][j].tropas = rand() % 5 + 1;
        }
    }
}

void exibirMapa(Territorio **mapa, int tamanho) {
    printf("\nMAPA:\n");
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            printf("[%d:%d] ", mapa[i][j].dono, mapa[i][j].tropas);
        }
        printf("\n");
    }
}

void faseDeAtaque(Territorio **mapa) {
    int x1, y1, x2, y2;

    printf("Território atacante (linha coluna): ");
    scanf("%d %d", &x1, &y1);

    printf("Território defensor (linha coluna): ");
    scanf("%d %d", &x2, &y2);

    if (mapa[x1][y1].dono != JOGADOR) {
        printf("Você só pode atacar com seus territórios!\n");
        return;
    }

    if (mapa[x2][y2].dono == JOGADOR) {
        printf("Não pode atacar seu próprio território!\n");
        return;
    }

    if (mapa[x1][y1].tropas < 2) {
        printf("Tropas insuficientes para atacar!\n");
        return;
    }

    int resultado = simularAtaque(
        mapa[x1][y1].tropas,
        mapa[x2][y2].tropas
    );

    if (resultado) {
        printf("Ataque venceu!\n");
        mapa[x2][y2].dono = JOGADOR;
        mapa[x2][y2].tropas = mapa[x1][y1].tropas - 1;
        mapa[x1][y1].tropas = 1;
    } else {
        printf("Ataque falhou!\n");
        mapa[x1][y1].tropas = 1;
    }
}

int simularAtaque(int tropasAtacante, int tropasDefensor) {
    int dadoA = rand() % 6 + 1;
    int dadoD = rand() % 6 + 1;

    printf("Dado atacante: %d\n", dadoA);
    printf("Dado defensor: %d\n", dadoD);

    return dadoA > dadoD;
}

Missao sortearMissao() {
    Missao m;

    int sorteio = rand() % 2;

    if (sorteio == 0) {
        strcpy(m.descricao, "Conquistar 10 territórios.");
        m.territoriosNecessarios = 10;
    } else {
        strcpy(m.descricao, "Conquistar 15 territórios.");
        m.territoriosNecessarios = 15;
    }

    return m;
}

int verificarVitoria(Territorio **mapa, Missao missao) {
    int contador = 0;

    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            if (mapa[i][j].dono == JOGADOR)
                contador++;
        }
    }

    return contador >= missao.territoriosNecessarios;
}
