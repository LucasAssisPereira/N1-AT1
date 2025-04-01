#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define TOTAL_CARTAS 40
#define PONTOS_VITORIA 12
#define MAX_JOGADORES 2

typedef struct {
    char naipe;
    int valor;
    int rank;
} Carta;

typedef struct {
    char nome[20];
    int pontos;
    Carta mao[3];
    int podeAumentar;
    int deveResponderAumento;
} Jogador;

typedef struct {
    Carta baralho[TOTAL_CARTAS];
    Jogador jogadores[MAX_JOGADORES];
    int pontosRodada;
    int turno;
    int maoAtual;
    int vitoriasMaos[2];
    Carta ultimaCartaJogada[MAX_JOGADORES];
    int aumentoPendente;
    int valorAumentoPropuesto;
} EstadoJogo;

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void inicializarBaralho(Carta baralho[]) {
    char naipes[] = {'E', 'C', 'O', 'P'};
    int valores[] = {1, 2, 3, 4, 5, 6, 7, 10, 11, 12};
    int ranks[] = {14, 13, 12, 11, 10, 9, 8, 7, 6, 5};
    int i, j, index = 0;
    
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 10; j++) {
            baralho[index].naipe = naipes[i];
            baralho[index].valor = valores[j];
            baralho[index].rank = ranks[j];
            
            if (valores[j] == 4 && naipes[i] == 'P') baralho[index].rank = 1;
            else if (valores[j] == 7 && naipes[i] == 'C') baralho[index].rank = 2;
            else if (valores[j] == 1 && naipes[i] == 'E') baralho[index].rank = 3;
            else if (valores[j] == 1 && naipes[i] == 'P') baralho[index].rank = 4;
            
            index++;
        }
    }
}

void embaralharBaralho(Carta baralho[]) {
    int i, j;
    Carta temp;
    srand(time(NULL));
    for (i = 0; i < TOTAL_CARTAS; i++) {
        j = rand() % TOTAL_CARTAS;
        temp = baralho[i];
        baralho[i] = baralho[j];
        baralho[j] = temp;
    }
}

void distribuirCartas(EstadoJogo *estado) {
    int i, j, cartaIndex = 0;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < MAX_JOGADORES; j++) {
            estado->jogadores[j].mao[i] = estado->baralho[cartaIndex++];
        }
    }
}

void jogarCarta(EstadoJogo *estado) {
    int jogador = estado->turno;
    int adversario = 1 - jogador;
    int escolha;
    
    printf("\nEscolha qual carta jogar (1-3): ");
    if (scanf("%d", &escolha) != 1) {
        limparBuffer();
        printf("Entrada invalida! Digite um numero.\n");
        return;
    }
    limparBuffer();
    
    if (escolha < 1 || escolha > 3) {
        printf("Escolha invalida! Digite 1, 2 ou 3.\n");
        return;
    }
    
    if (estado->jogadores[jogador].mao[escolha-1].valor == 0) {
        printf("Voce ja jogou esta carta! Escolha outra.\n");
        return;
    }
    
    estado->ultimaCartaJogada[jogador] = estado->jogadores[jogador].mao[escolha-1];
    estado->jogadores[jogador].mao[escolha-1].valor = 0;
    
    printf("\nVoce jogou: %d de %c\n", 
           estado->ultimaCartaJogada[jogador].valor,
           estado->ultimaCartaJogada[jogador].naipe);

}

void processarAumento(EstadoJogo *estado) {
    int jogador = estado->turno;
    int adversario = 1 - jogador;
    int escolha;
    
    printf("\nAumentar para:\n");
    printf("1. 3 pontos\n");
    if (estado->pontosRodada >= 3) printf("2. 6 pontos\n");
    if (estado->pontosRodada >= 6) printf("3. 9 pontos\n");
    printf("Escolha: ");
    
    if (scanf("%d", &escolha) != 1) {
        limparBuffer();
        printf("Entrada invalida!\n");
        return;
    }
    limparBuffer();
    
    switch (escolha) {
        case 1: estado->valorAumentoPropuesto = 3; break;
        case 2: if (estado->pontosRodada >= 3) estado->valorAumentoPropuesto = 6; break;
        case 3: if (estado->pontosRodada >= 6) estado->valorAumentoPropuesto = 9; break;
        default: 
            printf("Opcao invalida!\n");
            return;
    }
    
    estado->aumentoPendente = 1;
    estado->jogadores[adversario].deveResponderAumento = 1;
    estado->jogadores[jogador].podeAumentar = 0;
    
    printf("\n%s pediu para aumentar a rodada para %d pontos!\n", 
           estado->jogadores[jogador].nome, estado->valorAumentoPropuesto);
}

void aceitarAumento(EstadoJogo *estado) {
    estado->pontosRodada = estado->valorAumentoPropuesto;
    estado->aumentoPendente = 0;
    estado->jogadores[estado->turno].deveResponderAumento = 0;
    printf("\nAumento aceito! Rodada agora vale %d pontos.\n", estado->pontosRodada);
}

void recusarAumento(EstadoJogo *estado) {
    int adversario = 1 - estado->turno;
    estado->jogadores[adversario].pontos += estado->pontosRodada;
    estado->aumentoPendente = 0;
    printf("\n%s recusou o aumento! %s ganha %d pontos.\n", 
           estado->jogadores[estado->turno].nome,
           estado->jogadores[adversario].nome,
           estado->pontosRodada);
    estado->maoAtual = 3;
}

void mostrarMao(Jogador *jogador) {
    int i;
    printf("\nSuas cartas:\n");
    for (i = 0; i < 3; i++) {
        if (jogador->mao[i].valor != 0) {
            printf("%d. %d de %c (forca %d)\n", i+1, 
                   jogador->mao[i].valor, 
                   jogador->mao[i].naipe,
                   jogador->mao[i].rank);
        } else {
            printf("%d. (Carta ja jogada)\n", i+1);
        }
    }
}

void mostrarPlacar(EstadoJogo *estado) {
    printf("\nPlacar: %s %d x %d %s | Rodada: %d pts\n",
           estado->jogadores[0].nome, estado->jogadores[0].pontos,
           estado->jogadores[1].pontos, estado->jogadores[1].nome,
           estado->pontosRodada);
}

void limparTela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void mostrarMenu(EstadoJogo *estado) {
    int jogador = estado->turno;
    int escolha;
    int opcaoValida = 0;
    
    do {
        mostrarMao(&estado->jogadores[jogador]);
        
        printf("\nOpcoes:\n");
        printf("1. Jogar carta\n");
        
        if (estado->jogadores[jogador].podeAumentar) {
            printf("2. Aumentar aposta\n");
        }
        
        if (estado->jogadores[jogador].deveResponderAumento) {
            printf("3. Aceitar aumento\n");
            printf("4. Recusar aumento (desistir)\n");
        }
        
        printf("0. Passar a vez\n");
        printf("Escolha: ");
        
        if (scanf("%d", &escolha) != 1) {
            limparBuffer();
            printf("Entrada invalida! Digite um numero.\n");
            continue;
        }
        limparBuffer();
        
        switch (escolha) {
            case 1:
                jogarCarta(estado);
                opcaoValida = 1;
                break;
            case 2:
                if (estado->jogadores[jogador].podeAumentar) {
                    processarAumento(estado);
                    opcaoValida = 1;
                } else {
                    printf("Opcao nao disponivel no momento!\n");
                }
                break;
            case 3:
                if (estado->jogadores[jogador].deveResponderAumento) {
                    aceitarAumento(estado);
                    opcaoValida = 1;
                } else {
                    printf("Opcao nao disponivel no momento!\n");
                }
                break;
            case 4:
                if (estado->jogadores[jogador].deveResponderAumento) {
                    recusarAumento(estado);
                    opcaoValida = 1;
                } else {
                    printf("Opcao nao disponivel no momento!\n");
                }
                break;
            case 0:
                if (estado->ultimaCartaJogada[jogador].valor != 0) {
                    printf("Voce ja jogou uma carta nesta mao. Nao pode passar a vez!\n");
                } else if (estado->jogadores[jogador].deveResponderAumento) {
                    printf("Voce precisa responder ao pedido de aumento primeiro!\n");
                } else {
                    printf("Voce passou a vez.\n");
                    opcaoValida = 1;
                }
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
        
        if (!opcaoValida) {
            printf("Pressione Enter para continuar...");
            limparBuffer();
            limparTela();
            
            printf("\n--- Turno do %s ---\n", estado->jogadores[jogador].nome);
            mostrarPlacar(estado);
            
            if (estado->ultimaCartaJogada[1-jogador].valor != 0) {
                printf("\nCarta do adversario: %d de %c\n", 
                       estado->ultimaCartaJogada[1-jogador].valor,
                       estado->ultimaCartaJogada[1-jogador].naipe);
            }
        }
    } while (!opcaoValida);
}

void executarTurno(EstadoJogo *estado) {
    int jogador = estado->turno;
    int adversario = 1 - jogador;
    
    printf("\n--- Turno do %s ---\n", estado->jogadores[jogador].nome);
    mostrarPlacar(estado);
    
    if (estado->ultimaCartaJogada[adversario].valor != 0) {
        printf("\nCarta do adversario: %d de %c\n", 
               estado->ultimaCartaJogada[adversario].valor,
               estado->ultimaCartaJogada[adversario].naipe);
    }
    
    mostrarMenu(estado);
}

void executarMao(EstadoJogo *estado) {
    limparTela();
    printf("\n=== Mao %d ===\n", estado->maoAtual + 1);
    
    estado->ultimaCartaJogada[0].valor = 0;
    estado->ultimaCartaJogada[1].valor = 0;
    
    estado->turno = 0;
    while ((estado->ultimaCartaJogada[0].valor == 0 || 
            estado->ultimaCartaJogada[1].valor == 0) &&
           !estado->aumentoPendente) {
        
        executarTurno(estado);
        
        if (estado->ultimaCartaJogada[estado->turno].valor == 0 && 
            !estado->jogadores[estado->turno].deveResponderAumento) {
            estado->vitoriasMaos[1 - estado->turno]++;
            printf("\n%s passou a vez! %s vence esta mao!\n",
                   estado->jogadores[estado->turno].nome,
                   estado->jogadores[1 - estado->turno].nome);
            break;
        }
        
        estado->turno = 1 - estado->turno;
    }
    
    if (estado->ultimaCartaJogada[0].valor != 0 && 
        estado->ultimaCartaJogada[1].valor != 0) {
        
        int vencedor = (estado->ultimaCartaJogada[0].rank < estado->ultimaCartaJogada[1].rank) ? 0 :
                      (estado->ultimaCartaJogada[0].rank > estado->ultimaCartaJogada[1].rank) ? 1 : -1;
        
        if (vencedor >= 0) {
            estado->vitoriasMaos[vencedor]++;
            printf("\n%s venceu esta mao!\n", estado->jogadores[vencedor].nome);
        } else {
            printf("\nEmpate nesta mao!\n");
        }
    }
}

void executarRodada(EstadoJogo *estado) {
    int i;
    estado->pontosRodada = 1;
    estado->maoAtual = 0;
    estado->vitoriasMaos[0] = 0;
    estado->vitoriasMaos[1] = 0;
    memset(estado->ultimaCartaJogada, 0, sizeof(estado->ultimaCartaJogada));
    estado->aumentoPendente = 0;
    
    inicializarBaralho(estado->baralho);
    embaralharBaralho(estado->baralho);
    distribuirCartas(estado);
    
    for (i = 0; i < MAX_JOGADORES; i++) {
        estado->jogadores[i].podeAumentar = 1;
        estado->jogadores[i].deveResponderAumento = 0;
    }
    
    while (estado->maoAtual < 3 && 
           estado->vitoriasMaos[0] < 2 && 
           estado->vitoriasMaos[1] < 2) {
        
        executarMao(estado);
        estado->maoAtual++;
    }
    
    if (estado->vitoriasMaos[0] > estado->vitoriasMaos[1]) {
        estado->jogadores[0].pontos += estado->pontosRodada;
        printf("\n%s venceu a rodada e ganhou %d pontos!\n", 
               estado->jogadores[0].nome, estado->pontosRodada);
    } else {
        estado->jogadores[1].pontos += estado->pontosRodada;
        printf("\n%s venceu a rodada e ganhou %d pontos!\n", 
               estado->jogadores[1].nome, estado->pontosRodada);
    }
}

void executarPartida() {
    EstadoJogo estado;
    int vencedor;
    memset(&estado, 0, sizeof(estado));
    
    strcpy(estado.jogadores[0].nome, "Jogador 1");
    strcpy(estado.jogadores[1].nome, "Jogador 2");
    
    printf("=== Jogo de Truco ===\n");
    
    while (estado.jogadores[0].pontos < PONTOS_VITORIA && 
           estado.jogadores[1].pontos < PONTOS_VITORIA) {
        
        executarRodada(&estado);
        
        printf("\nPlacar: %s %d x %d %s\n",
               estado.jogadores[0].nome, estado.jogadores[0].pontos,
               estado.jogadores[1].pontos, estado.jogadores[1].nome);
               
        printf("Pressione Enter para continuar...");
        limparBuffer();
        limparTela();
    }
    
    vencedor = (estado.jogadores[0].pontos >= PONTOS_VITORIA) ? 0 : 1;
    printf("\n=== FIM DE JOGO ===\n");
    printf("%s venceu o jogo com %d pontos!\n", 
           estado.jogadores[vencedor].nome, 
           estado.jogadores[vencedor].pontos);
}

int main() {
    executarPartida();
    return 0;
}
