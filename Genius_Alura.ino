//Leds
#define LED_VERDE 2
#define LED_AMARELO 3
#define LED_VERMELHO 4
#define LED_AZUL 5
#define INDEFINIDO -1


//Botões
#define BT_VERDE 12
#define BT_AMARELO 11
#define BT_VERMELHO 10
#define BT_AZUL 9

//Tempo
#define UM_SEGUNDO 1000
#define MEIO_SEGUNDO 500
#define MILIS_SEGUNDO 100

//Extras
#define TAMANHO_JOGO 3
#define RANDOM_PORT 0

int sequenciaJogo[TAMANHO_JOGO];
int ledsRespondidos;
int ultimaRespostaUsuario = -1;

boolean falha = false;

int rodada = 0;

enum Estados {
  PRONTO_PARA_RODAR_SEQUENCIA,
  AGUARDANDO_RESPOSTA,
  FINALIZADO_SUCESSO,
  FINALIZADO_FALHA
};


void setup() {
  Serial.begin(9600);
  inicializaPinos();
  iniciaJogo();
  ledsRespondidos = 0;
}

void loop() {
  int acao = verificaEstado();
  Serial.println(acao);
  switch (acao) {
    case PRONTO_PARA_RODAR_SEQUENCIA:
      preparaProximaRodada();
      break;
    case AGUARDANDO_RESPOSTA:
      verificaUltimoBtApertado();
      break;
    case FINALIZADO_SUCESSO:
      acenderLuzeSocesso();
      break;
    case FINALIZADO_FALHA:
      acenderLuzesFalha();
      break;
  }
  //delay(MEIO_SEGUNDO);
}

void acenderLuzesFalha() {
  piscaTodosOsLeds();
  piscaTodosOsLeds();
  piscaTodosOsLeds();
}

void acenderLuzeSocesso() {
  piscaSequenciaRapida();
  piscaSequenciaRapida();
  piscaSequenciaRapida();
  piscaTodosOsLeds();
}

void piscaSequenciaRapida() {
  digitalWrite(LED_VERDE, HIGH);
  delay(MILIS_SEGUNDO);
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_AMARELO, HIGH);
  delay(MILIS_SEGUNDO);
  digitalWrite(LED_AMARELO, LOW);
  digitalWrite(LED_AZUL, HIGH);
  delay(MILIS_SEGUNDO);
  digitalWrite(LED_AZUL, LOW);
  digitalWrite(LED_VERMELHO, HIGH);
  delay(MILIS_SEGUNDO);
  digitalWrite(LED_VERMELHO, LOW);
}

void preparaProximaRodada() {
  rodada++;
  executaSequencia();
  ledsRespondidos = 0;
}

void verificaUltimoBtApertado() {
  int ledAcender = verificaRespostaUsuario();
  if (ledAcender != -1) {
    piscaLed(ledAcender);
    ultimaRespostaUsuario = ledAcender;
    verificaRespostaCorreta();
  }

}

void verificaRespostaCorreta() {
  Serial.println("Resposta do usuário" + ultimaRespostaUsuario);
  Serial.println("Led da Rodada" + sequenciaJogo[ledsRespondidos]);
  if (ultimaRespostaUsuario == sequenciaJogo[ledsRespondidos]) {
    ledsRespondidos++;
  } else {
    falha = true;
  }
}

int verificaEstado() {
  if (falha) {
    return FINALIZADO_FALHA;
  } else if (rodada < TAMANHO_JOGO && ledsRespondidos == rodada) {
    return PRONTO_PARA_RODAR_SEQUENCIA;
  } else if (ledsRespondidos < rodada && ledsRespondidos < TAMANHO_JOGO) {
    return AGUARDANDO_RESPOSTA;
  } else if (rodada == TAMANHO_JOGO) {
    return FINALIZADO_SUCESSO;
  }
}

void executaSequencia() {
  for (int i = 0; i < rodada; i++) {
    piscaLed(sequenciaJogo[i]);
  }
}

void inicializaPinos() {
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);

  pinMode(BT_VERDE, INPUT_PULLUP);
  pinMode(BT_AMARELO, INPUT_PULLUP);
  pinMode(BT_VERMELHO, INPUT_PULLUP);
  pinMode(BT_AZUL, INPUT_PULLUP);

  pinMode(RANDOM_PORT, INPUT);
}

void iniciaJogo() {
  int versaoJogo = analogRead(RANDOM_PORT);
  randomSeed(versaoJogo);
  for (int i = 0; i < TAMANHO_JOGO; i++) {
    sequenciaJogo[i] = geraLedAleatorio();
  }
}

int geraLedAleatorio() {
  return random(LED_VERDE, LED_AZUL + 1);
}

int verificaRespostaUsuario() {
  if (!digitalRead(BT_VERDE)) {
    return LED_VERDE;
  }
  if (!digitalRead(BT_AMARELO)) {
    return LED_AMARELO;
  }
  if (!digitalRead(BT_VERMELHO)) {
    return LED_VERMELHO;
  }
  if (!digitalRead(BT_AZUL)) {
    return LED_AZUL;
  }
  return INDEFINIDO;
}

void piscaLed(int portaLed) {
  digitalWrite(portaLed, HIGH);
  delay(UM_SEGUNDO);
  digitalWrite(portaLed, LOW);
  delay(MEIO_SEGUNDO);
}

void piscaTodosOsLeds() {
  digitalWrite(LED_VERDE, HIGH);
  digitalWrite(LED_AMARELO, HIGH);
  digitalWrite(LED_AZUL, HIGH);
  digitalWrite(LED_VERMELHO, HIGH);
  delay(MEIO_SEGUNDO);
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_AMARELO, LOW);
  digitalWrite(LED_AZUL, LOW);
  digitalWrite(LED_VERMELHO, LOW);
  delay(MEIO_SEGUNDO);
}
