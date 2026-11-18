
#include <Arduino.h>

#include "energia.h"

// =====================================
// BIBOU - GERENCIAMENTO DE ENERGIA
// =====================================

// Valores simulados
static float nivelBateria = 100.0;
static float tensaoBateria = 0.0;

static bool carregando = false;
static bool retornoSolicitado = false;

// Limites logicos de demonstracao
// Nao sao limites eletricos da bateria.
static const float NIVEL_BAIXO = 25.0;
static const float NIVEL_CRITICO = 10.0;

static unsigned long ultimaAtualizacao = 0;

// =====================================
// INICIALIZACAO
// =====================================

void iniciarEnergia() {

  nivelBateria = 100.0;
  tensaoBateria = 0.0;

  carregando = false;
  retornoSolicitado = false;

  ultimaAtualizacao = millis();

  Serial.println(
    "Energia inicializada (simulacao)"
  );
}

// =====================================
// ATUALIZACAO
// =====================================

void atualizarEnergia() {

  unsigned long agora = millis();

  if (agora - ultimaAtualizacao < 1000) {
    return;
  }

  ultimaAtualizacao = agora;

  // A bateria nao descarrega sozinha
  // durante a simulacao.
  // Seu nivel pode ser alterado
  // pelo comando bateria:VALOR.

  if (nivelBateria <= NIVEL_BAIXO) {
    retornoSolicitado = true;
  }

  if (nivelBateria > NIVEL_BAIXO) {
    retornoSolicitado = false;
  }
}

// =====================================
// CONSULTAS
// =====================================

float obterNivelBateria() {
  return nivelBateria;
}

float obterTensaoBateria() {
  return tensaoBateria;
}

bool bateriaBaixa() {
  return nivelBateria <= NIVEL_BAIXO;
}

bool bateriaCritica() {
  return nivelBateria <= NIVEL_CRITICO;
}

bool estaCarregando() {
  return carregando;
}

bool precisaRetornarBase() {
  return retornoSolicitado && !carregando;
}

// =====================================
// ESTADO DE CARREGAMENTO
// =====================================

void definirCarregando(bool novoEstado) {

  carregando = novoEstado;

  if (carregando) {
    retornoSolicitado = false;
    Serial.println("BIBOU: carregamento simulado");
  } else {
    Serial.println("BIBOU: carregamento encerrado");
  }
}

// =====================================
// SIMULAR NIVEL DA BATERIA
// =====================================

void simularNivelBateria(float nivel) {

  nivelBateria = constrain(nivel, 0.0f, 100.0f);

  retornoSolicitado =
    nivelBateria <= NIVEL_BAIXO;

  Serial.print("Bateria simulada: ");
  Serial.print(nivelBateria, 1);
  Serial.println("%");

  if (bateriaCritica()) {
    Serial.println("ALERTA: bateria critica!");
  }

  else if (bateriaBaixa()) {
    Serial.println("AVISO: bateria baixa!");
  }
}

// =====================================
// STATUS
// =====================================

String obterStatusEnergia() {

  String status = "";

  status += "Bateria: ";
  status += String(nivelBateria, 1);
  status += "%\n";

  status += "Tensao: nao medida\n";

  status += "Carregando: ";
  status += carregando ? "SIM\n" : "NAO\n";

  status += "Bateria baixa: ";
  status += bateriaBaixa() ? "SIM\n" : "NAO\n";

  status += "Bateria critica: ";
  status += bateriaCritica() ? "SIM\n" : "NAO\n";

  status += "Retorno solicitado: ";
  status += precisaRetornarBase() ? "SIM" : "NAO";

  return status;
}
