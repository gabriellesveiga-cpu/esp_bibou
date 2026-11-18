
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#include "config.h"
#include "bracos.h"

// ===================================
// BIBOU - BRACOS ARTICULADOS
// ===================================

// Manter em simulacao ate confirmar
// os servos e as conexoes.
static const bool SIMULAR_BRACOS = true;

// PCA9685: endereco I2C padrao 0x40
Adafruit_PWMServoDriver pca(0x40);

// Canais previstos
const uint8_t SERVO_ESQUERDO = 0;
const uint8_t SERVO_DIREITO = 1;

// Valores iniciais de referencia.
// Precisam de calibracao fisica.
const int POSICAO_NEUTRA = 90;

struct PassoBracos {
  int esquerdo;
  int direito;
  unsigned long duracao;
};

const PassoBracos ACENAR[] = {
  {90, 90, 400},
  {90, 55, 350},
  {90, 110, 350},
  {90, 55, 350},
  {90, 90, 400}
};

const PassoBracos LEVANTAR[] = {
  {90, 90, 300},
  {40, 140, 700}
};

const PassoBracos ABAIXAR[] = {
  {90, 90, 500}
};

const PassoBracos ABRIR[] = {
  {40, 140, 600}
};

const PassoBracos DANCAR[] = {
  {60, 120, 300},
  {120, 60, 300},
  {50, 130, 300},
  {130, 50, 300},
  {90, 90, 400}
};

static String acaoAtual = "parados";

static const PassoBracos* sequencia = nullptr;
static size_t quantidadePassos = 0;
static size_t passoAtual = 0;

static unsigned long inicioPasso = 0;
static bool executando = false;

// ===================================
// MOVIMENTO LOGICO DOS SERVOS
// ===================================

void posicionarBracos(int esquerdo, int direito) {

  esquerdo = constrain(esquerdo, 0, 180);
  direito = constrain(direito, 0, 180);

  if (SIMULAR_BRACOS) {
    Serial.print("Braco E: ");
    Serial.print(esquerdo);
    Serial.print(" | Braco D: ");
    Serial.println(direito);
    return;
  }

  // A conversao de angulo para PWM
  // sera calibrada conforme os servos.
  // Nenhum sinal fisico e enviado ainda.
}

// ===================================
// INICIAR
// ===================================

void iniciarBracos() {

  if (!SIMULAR_BRACOS) {
    Wire.begin();
    pca.begin();
    pca.setPWMFreq(50);
  }

  posicionarBracos(
    POSICAO_NEUTRA,
    POSICAO_NEUTRA
  );

  Serial.println("Bracos inicializados!");
}

// ===================================
// PARAR
// ===================================

void pararBracos() {
  executando = false;
  sequencia = nullptr;
  quantidadePassos = 0;
  acaoAtual = "parados";

  Serial.println("Sequencia dos bracos parada");
}

// ===================================
// EXECUTAR SEQUENCIA
// ===================================

void iniciarSequencia(
  const PassoBracos* passos,
  size_t quantidade,
  String nome
) {

  sequencia = passos;
  quantidadePassos = quantidade;
  passoAtual = 0;

  acaoAtual = nome;
  executando = true;
  inicioPasso = millis();

  posicionarBracos(
    sequencia[0].esquerdo,
    sequencia[0].direito
  );
}

// ===================================
// COMANDOS
// ===================================

bool executarAcaoBracos(String acao) {

  acao.trim();
  acao.toLowerCase();

  if (acao == "parar") {
    pararBracos();
    return true;
  }

  if (acao == "acenar") {
    iniciarSequencia(
      ACENAR,
      sizeof(ACENAR) / sizeof(ACENAR[0]),
      "acenar"
    );
  }

  else if (acao == "levantar") {
    iniciarSequencia(
      LEVANTAR,
      sizeof(LEVANTAR) / sizeof(LEVANTAR[0]),
      "levantar"
    );
  }

  else if (acao == "abaixar") {
    iniciarSequencia(
      ABAIXAR,
      sizeof(ABAIXAR) / sizeof(ABAIXAR[0]),
      "abaixar"
    );
  }

  else if (acao == "abrir") {
    iniciarSequencia(
      ABRIR,
      sizeof(ABRIR) / sizeof(ABRIR[0]),
      "abrir"
    );
  }

  else if (acao == "dancar") {
    iniciarSequencia(
      DANCAR,
      sizeof(DANCAR) / sizeof(DANCAR[0]),
      "dancar"
    );
  }

  else {
    return false;
  }

  return true;
}

// ===================================
// ATUALIZAR ANIMACAO
// ===================================

void atualizarBracos() {

  if (!executando || sequencia == nullptr) {
    return;
  }

  unsigned long agora = millis();

  if (
    agora - inicioPasso >=
    sequencia[passoAtual].duracao
  ) {

    passoAtual++;

    if (passoAtual >= quantidadePassos) {
      pararBracos();
      return;
    }

    inicioPasso = agora;

    posicionarBracos(
      sequencia[passoAtual].esquerdo,
      sequencia[passoAtual].direito
    );
  }
}

// ===================================
// CONSULTAR ESTADO
// ===================================

String obterAcaoBracos() {
  return acaoAtual;
}
