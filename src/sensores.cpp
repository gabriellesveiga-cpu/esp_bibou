
#include <Arduino.h>

#include "sensores.h"

// ====================================
// BIBOU - SISTEMA DE SENSORES
// MODO SIMULACAO
// ====================================

static float distancia = 100.0;
static float inclinacao = 0.0;

static bool obstaculo = false;

static long encoderEsquerdo = 0;
static long encoderDireito = 0;

static unsigned long ultimaLeitura = 0;

// ====================================
// INICIALIZACAO
// ====================================

void iniciarSensores() {

  distancia = 100.0;
  inclinacao = 0.0;
  obstaculo = false;

  encoderEsquerdo = 0;
  encoderDireito = 0;

  Serial.println(
    "Sensores inicializados (simulacao)"
  );
}

// ====================================
// ATUALIZAR SENSORES
// ====================================

void atualizarSensores() {

  unsigned long agora = millis();

  if (agora - ultimaLeitura < 200) {
    return;
  }

  ultimaLeitura = agora;

  // Dados ficticios para testes.
  // Ainda nao representam o ambiente real.

  distancia = 100.0;
  inclinacao = 0.0;

  obstaculo = distancia < 20.0;
}

// ====================================
// CONSULTAR DISTANCIA
// ====================================

float obterDistancia() {
  return distancia;
}

// ====================================
// DETECTAR OBSTACULO
// ====================================

bool detectarObstaculo() {
  return obstaculo;
}

// ====================================
// CONSULTAR INCLINACAO
// ====================================

float obterInclinacao() {
  return inclinacao;
}

// ====================================
// ENCODERS
// ====================================

long obterEncoderEsquerdo() {
  return encoderEsquerdo;
}

long obterEncoderDireito() {
  return encoderDireito;
}

// ====================================
// STATUS DOS SENSORES
// ====================================

String obterStatusSensores() {

  String status = "";

  status += "Distancia: ";
  status += String(distancia, 1);
  status += " cm\n";

  status += "Obstaculo: ";
  status += obstaculo ? "SIM\n" : "NAO\n";

  status += "Inclinacao: ";
  status += String(inclinacao, 1);
  status += " graus\n";

  status += "Encoder esquerdo: ";
  status += String(encoderEsquerdo);
  status += "\n";

  status += "Encoder direito: ";
  status += String(encoderDireito);

  return status;
}
