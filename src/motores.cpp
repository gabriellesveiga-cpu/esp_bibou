
#include <Arduino.h>
#include "motores.h"

// ====================================
// BIBOU - CONTROLE DE MOTORES
// Versao logica / simulada
// ====================================

static String movimentoAtual = "parado";
static int velocidadeAtual = 50;

// Tempo maximo sem novo comando
static const unsigned long TEMPO_SEGURANCA = 1000;

static unsigned long ultimoComando = 0;

// ====================================
// INICIALIZACAO
// ====================================

void iniciarMotores() {
  movimentoAtual = "parado";
  velocidadeAtual = 50;
  ultimoComando = millis();

  Serial.println("Motores inicializados (simulacao)");
}

// ====================================
// PARADA
// ====================================

void pararMotores() {
  movimentoAtual = "parado";

  // Futuramente: desligar os canais
  // do driver de motores.

  Serial.println("BIBOU: motores parados");
}

// ====================================
// VELOCIDADE
// ====================================

void definirVelocidade(int velocidade) {
  velocidadeAtual = constrain(velocidade, 0, 100);

  Serial.print("Velocidade configurada: ");
  Serial.print(velocidadeAtual);
  Serial.println("%");
}

// ====================================
// COMANDOS DE MOVIMENTO
// ====================================

bool executarMovimento(String comando) {
  comando.trim();
  comando.toLowerCase();

  if (comando == "parar") {
    pararMotores();
    return true;
  }

  if (
    comando == "frente" ||
    comando == "tras" ||
    comando == "esquerda" ||
    comando == "direita"
  ) {
    movimentoAtual = comando;
    ultimoComando = millis();

    Serial.println("Movimento: " + movimentoAtual);
    return true;
  }

  Serial.println("Movimento desconhecido");
  return false;
}

// ====================================
// ATUALIZACAO DE SEGURANCA
// ====================================

void atualizarMotores() {
  if (
    movimentoAtual != "parado" &&
    millis() - ultimoComando >= TEMPO_SEGURANCA
  ) {
    Serial.println("Seguranca: comando expirou");
    pararMotores();
  }
}

// ====================================
// CONSULTAS
// ====================================

String obterMovimento() {
  return movimentoAtual;
}

int obterVelocidade() {
  return velocidadeAtual;
}

