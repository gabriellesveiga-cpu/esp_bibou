
#include <Arduino.h>

#include "seguranca.h"
#include "motores.h"
#include "bracos.h"
#include "sensores.h"
#include "energia.h"
#include "comunicacao.h"

// =====================================
// BIBOU - SISTEMA DE SEGURANCA
// =====================================

static bool emergencia = false;
static String motivoBloqueio = "nenhum";

static const float DISTANCIA_MINIMA_CM = 15.0;

void iniciarSeguranca() {
  emergencia = false;
  motivoBloqueio = "nenhum";

  Serial.println("Sistema de seguranca iniciado!");
}

bool movimentoPermitido(String movimento) {

  movimento.trim();
  movimento.toLowerCase();

  // Parar sempre deve ser permitido
  if (movimento == "parar") {
    return true;
  }

  if (emergencia) {
    motivoBloqueio = "emergencia";
    return false;
  }

  if (bateriaCritica()) {
    motivoBloqueio = "bateria critica";
    return false;
  }

  if (estaCarregando()) {
    motivoBloqueio = "carregando";
    return false;
  }

  if (
    movimento == "frente" &&
    detectarObstaculo() &&
    obterDistancia() <= DISTANCIA_MINIMA_CM
  ) {
    motivoBloqueio = "obstaculo";
    return false;
  }

  motivoBloqueio = "nenhum";
  return true;
}

void ativarEmergencia() {

  emergencia = true;
  motivoBloqueio = "emergencia";

  pararMotores();
  pararBracos();

  Serial.println("EMERGENCIA ATIVADA!");
}

void liberarEmergencia() {

  emergencia = false;
  motivoBloqueio = "nenhum";

  Serial.println("Emergencia liberada.");
}

bool emergenciaAtiva() {
  return emergencia;
}

void atualizarSeguranca() {

  if (emergencia || bateriaCritica() || estaCarregando()) {
    if (obterMovimento() != "parado") {
      pararMotores();
    }
  }

  if (
    obterMovimento() == "frente" &&
    detectarObstaculo() &&
    obterDistancia() <= DISTANCIA_MINIMA_CM
  ) {
    pararMotores();
    motivoBloqueio = "obstaculo";
  }

  // Se a conexao Wi-Fi cair, interrompe
  // qualquer movimento em andamento.
  if (
    !wifiEstaConectado() &&
    obterMovimento() != "parado"
  ) {
    pararMotores();
    motivoBloqueio = "wifi desconectado";
  }
}

String obterStatusSeguranca() {

  String status = "";

  status += "Emergencia: ";
  status += emergencia ? "ATIVA" : "inativa";

  status += "\nMotivo bloqueio: ";
  status += motivoBloqueio;

  return status;
}
