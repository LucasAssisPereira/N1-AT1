# 📝 Descrição 
  Implementação do clássico jogo de Truco para dois jogadores, desenvolvido em linguagem C. O jogo segue as regras tradicionais do Truco paulista com sistema completo de cartas, rodadas e disputas por pontos.

# 🛠️ Como Compilar e Executar

  #### Compilar o jogo
    gcc truco.c -o truco

  #### Executar o jogo
    ./truco
# 🎮 Funcionalidades Implementadas
 #### Sistema completo de cartas:
  
  - Baralho com 40 cartas (7 a 12, sem 8 e 9)
  
  - Hierarquia correta das cartas (Zap, 7 de Copas, Espadilha, etc.)
  
  #### Mecânica de jogo:
  
  - Distribuição aleatória de cartas
  
  - Turnos alternados entre jogadores
  
  - Sistema de 3 mãos por rodada
  
  - Pontuação progressiva (1, 3, 6 ou 9 pontos por rodada)
  
  #### Sistema de Truco:
  
  - Pedido de aumento (Truco)
  
  - Aceitar ou recusar aumento
  
  - Passar a vez quando permitido
  
# 📜 Regras do Jogo
    
- Hierarquia das Cartas (da mais forte para a mais fraca):
  
- 4 de Paus (Zap)
  
- 7 de Copas
  
- Ás de Espadas (Espadilha)
    
- Ás de Paus
    
- Demais cartas: 7, 6, 5, 4, 3, 2, Q, J, K
    
- Fluxo do Jogo:
    
- Cada rodada começa valendo 1 ponto
    
- Os jogadores podem aumentar para 3, 6 ou 9 pontos
    
- Quem vencer 2 das 3 mãos ganha a rodada
    
- Primeiro jogador a atingir 12 pontos vence o jogo
  
# Controles Durante o Jogo:
   1. Jogar carta 
   2. Aumentar aposta (Truco) 
   3. Aceitar aumento 
   4. Recusar aumento 
   0. Passar a vez 
      
# 🖥️ Interface do Jogo
  - Durante a partida, o jogo exibe:

  - Cartas na mão do jogador atual

  - Última carta jogada pelo adversário

  - Placar atualizado

  - Valor atual da rodada

  - Número da mão atual (1 a 3)

# ⚙️ Estrutura do Código
  O programa está organizado em:
  
  EstadoJogo: Estrutura principal que armazena todo o estado do jogo
  
  Funções para manipulação do baralho (inicializar, embaralhar, distribuir)
  
  Funções para controle do fluxo do jogo (rodadas, mãos, turnos)
  
  Funções de interface (exibição do menu, placar, mão do jogador)
  
# ✅ Requisitos
  Compilador C (gcc recomendado)
  
  Terminal que suporte caracteres ASCII básicos
  
  Sistema operacional Linux/Windows/macOS
