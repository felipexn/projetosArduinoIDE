// Definindo os pinos para LEDs
//asfdasd
const int ledPinos[3][3] = {
  {2, 3, 4},
  {5, 6, 7},
  {8, 9, 10}
};

// Definindo os pinos para botões
const int btnCima = A0;
const int btnBaixo = A1;
const int btnEsquerda = A2;
const int btnDireita = A3;
const int btnConfirma = A4;

// Definindo variaveis para controle do jogo
int jogadorAtual = 1;
int linhaSelecionada = 0;
int colunaSelecionada = 0;
int jogadorNaPosicao[3][3] = {0};  // 0 representa posição vazia

void setup() {
  // Inicialização dos LEDs como saídas e desliga todos
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      pinMode(ledPinos[i][j], OUTPUT);
      digitalWrite(ledPinos[i][j], LOW);
    }
  }

  // Inicializacao dos botoes como entradas
  pinMode(btnCima, INPUT_PULLUP);
  pinMode(btnBaixo, INPUT_PULLUP);
  pinMode(btnEsquerda, INPUT_PULLUP);
  pinMode(btnDireita, INPUT_PULLUP);
  pinMode(btnConfirma, INPUT_PULLUP);
}

void loop() {
  // Verificar botoes direcionais
  if (digitalRead(btnCima) == HIGH) {
    atualizarPosicao(-1, 0);
    delay(200); // debounce
  }
  if (digitalRead(btnBaixo) == HIGH) {
    atualizarPosicao(1, 0);
    delay(200); // debounce
  }
  if (digitalRead(btnEsquerda) == HIGH) {
    atualizarPosicao(0, -1);
    delay(200); // debounce
  }
  if (digitalRead(btnDireita) == HIGH) {
    atualizarPosicao(0, 1);
    delay(200); // debounce
  }

  // Verificar botão de confirmação
  if (digitalRead(btnConfirma) == HIGH) {
    marcarJogada();
    delay(200); // debounce
  }

  // Atualizar LEDs de acordo com a seleção do jogador
  atualizarLEDs();

  // Verificar se ha um vencedor e reiniciar o jogo se necessario
  if (checkVencedor()) {
  for (int k = 0; k < 3; k++) {
    // Pisca todos os LEDs três vezes
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        digitalWrite(ledPinos[i][j], HIGH);
      }
    }
    delay(500);
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        digitalWrite(ledPinos[i][j], LOW);
      }
    }
    delay(500);
  }

  // Aguarde um segundo para que o jogador veja o resultado antes de reiniciar o jogo
  delay(1000);
  reiniciarJogo();
}


  // Outras acoes do loop podem ser adicionadas conforme necessario
}

void atualizarPosicao(int linha, int coluna) {
  // Desliga o LED na posição anterior
  digitalWrite(ledPinos[linhaSelecionada][colunaSelecionada], LOW);

  // Atualiza a posicao
  linhaSelecionada = (linhaSelecionada + linha + 3) % 3;
  colunaSelecionada = (colunaSelecionada + coluna + 3) % 3;

  // Liga o LED na nova posicao
  digitalWrite(ledPinos[linhaSelecionada][colunaSelecionada], HIGH);
}

void marcarJogada() {
  // Verificar se a posição já está ocupada
  if (jogadorNaPosicao[linhaSelecionada][colunaSelecionada] != 0) {
    return;
  }

  // Marcar a jogada do jogador atual
  jogadorNaPosicao[linhaSelecionada][colunaSelecionada] = jogadorAtual;

  // Armazenar a frequencia de acordo com o jogador atual (exemplo: jogador 1 -> 3 Hz, jogador 2 -> 6 Hz)
  int frequencia = (jogadorAtual == 1) ? 300 : 600;

  // Pisca o LED na nova posição de acordo com a frequencia do jogador
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledPinos[linhaSelecionada][colunaSelecionada], HIGH);
    delay(frequencia);
    digitalWrite(ledPinos[linhaSelecionada][colunaSelecionada], LOW);
    delay(frequencia);
  }

  // Alternar para o próximo jogador
  jogadorAtual = 3 - jogadorAtual; // Alternar entre 1 e 2
}

void atualizarLEDs() {
  // Mantém os LEDs ligados nas posições selecionadas
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (jogadorNaPosicao[i][j] == 1) {
        // Jogador 1 - Pisca lentamente
        digitalWrite(ledPinos[i][j], HIGH);
        delay(30);
        digitalWrite(ledPinos[i][j], LOW);
        delay(30);
      } else if (jogadorNaPosicao[i][j] == 2) {
        // Jogador 2 - Pisca rapidamente
        digitalWrite(ledPinos[i][j], HIGH);
        delay(150);
        digitalWrite(ledPinos[i][j], LOW);
        delay(150);
      }
    }
  }
}

bool checkVencedor() {
  // Verificar linhas e colunas
  for (int i = 0; i < 3; i++) {
    if (jogadorNaPosicao[i][0] != 0 && jogadorNaPosicao[i][0] == jogadorNaPosicao[i][1] && jogadorNaPosicao[i][1] == jogadorNaPosicao[i][2]) {
      return true; // Vencedor encontrado em uma linha
    }
    if (jogadorNaPosicao[0][i] != 0 && jogadorNaPosicao[0][i] == jogadorNaPosicao[1][i] && jogadorNaPosicao[1][i] == jogadorNaPosicao[2][i]) {
      return true; // Vencedor encontrado em uma coluna
    }
  }

  // Verificar diagonais
  if (jogadorNaPosicao[0][0] != 0 && jogadorNaPosicao[0][0] == jogadorNaPosicao[1][1] && jogadorNaPosicao[1][1] == jogadorNaPosicao[2][2]) {
    return true; // Vencedor encontrado na diagonal principal
  }
  if (jogadorNaPosicao[0][2] != 0 && jogadorNaPosicao[0][2] == jogadorNaPosicao[1][1] && jogadorNaPosicao[1][1] == jogadorNaPosicao[2][0]) {
    return true; // Vencedor encontrado na diagonal secundária
  }

  return false; // Nenhum vencedor encontrado
}

void reiniciarJogo() {
  // Reinicializar o estado do jogo
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      jogadorNaPosicao[i][j] = 0;
      digitalWrite(ledPinos[i][j], LOW);


      //asd
    }
  }
  jogadorAtual = 1;
  linhaSelecionada = 0;
  colunaSelecionada = 0;
}
