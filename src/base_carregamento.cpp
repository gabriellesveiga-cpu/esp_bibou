
#include <Arduino.h>

#include "base_carregamento.h"
#include "energia.h"
#include "motores.h"
#include "seguranca.h"

// =====================================
// BIBOU - BASE DE CARREGAMENTO
// SIMULACAO DE NAVEGACAO
// =====================================

static String estado = "aguardando";

static bool retornoSolicitado = false;
static bool baseLocalizada = false;
static bool encaixeConfirmado = false;

static unsigned long ultimaAtualizacao = 0;
static const unsigned long INTERVALO = 1000;

// =====================================
// INICIALIZACAO
// =====================================

void iniciarBaseCarregamento() {

  estado = "aguardando";
  retornoSolicitado = false;
  baseLocalizada = false;
  encaixeConfirmado = false;

  Serial.println("Base de carregamento iniciada!");
}

// =====================================
// CONTROLE
// =====================================

void solicitarRetornoBase() {

  if (emergenciaAtiva()) {
    Serial.println("Retorno bloqueado: emergencia");
    return;
  }

  if (estaCarregando()) {
    Serial.println("BIBOU ja esta carregando.");
    return;
  }

  retornoSolicitado = true;
  baseLocalizada = false;
  encaixeConfirmado = false;

  estado = "procurando";

  pararMotores();

  Serial.println("BIBOU procurando a base...");
}

void cancelarRetornoBase() {

  retornoSolicitado = false;
  baseLocalizada = false;
  encaixeConfirmado = false;

  estado = "aguardando";

  pararMotores();

  Serial.println("Retorno a base cancelado.");
}

void simularBaseLocalizada(bool localizada) {
  baseLocalizada = localizada;
}

void simularEncaixeBase(bool encaixado) {
  encaixeConfirmado = encaixado;
}

// =====================================
// ATUALIZACAO
// =====================================

void atualizarBaseCarregamento() {

  unsigned long agora = millis();

  if (agora - ultimaAtualizacao < INTERVALO) {
    return;
  }

  ultimaAtualizacao = agora;

  // Emergencia interrompe o retorno
  if (emergenciaAtiva()) {

    if (retornoSolicitado) {
      cancelarRetornoBase();
      estado = "interrompido";
    }

    return;
  }

  // Se o carregamento ja foi iniciado
  if (estaCarregando()) {

    retornoSolicitado = false;
    estado = "carregando";

    pararMotores();

    return;
  }

  // Iniciar automaticamente com bateria baixa
  if (
    precisaRetornarBase() &&
    !retornoSolicitado &&
    estado != "interrompido"
  ) {
    solicitarRetornoBase();
  }

  if (!retornoSolicitado) {
    return;
  }

  // Etapa 1: procurar base
  if (estado == "procurando") {

    if (baseLocalizada) {
      estado = "aproximando";
      Serial.println("Base localizada!");
    }

    return;
  }

  // Etapa 2: aproximacao simulada
  if (estado == "aproximando") {

    // Nenhum motor e acionado aqui.
    estado = "alinhando";

    Serial.println("BIBOU alinhando com a base...");

    return;
  }

  // Etapa 3: encaixe simulado
  if (estado == "alinhando") {

    if (encaixeConfirmado) {

      estado = "encaixado";
      retornoSolicitado = false;

      pararMotores();

      Serial.println("Encaixe confirmado!");
      Serial.println("Aguardando carga...");
    }

    return;
  }
}

// =====================================
// CONSULTAS
// =====================================

bool retornoBaseAtivo() {
  return retornoSolicitado;
}

String obterEstadoBase() {
  return estado;
}

String obterStatusBase() {

  String status = "===== BASE BIBOU =====\n";

  status += "Estado: " + estado + "\n";

  status += "Retorno ativo: ";
  status += retornoSolicitado ? "sim\n" : "nao\n";

  status += "Base localizada: ";
  status += baseLocalizada ? "sim\n" : "nao\n";

  status += "Encaixe confirmado: ";
  status += encaixeConfirmado ? "sim\n" : "nao\n";

  status += "======================";

  return status;
}
