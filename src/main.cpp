
#include <Arduino.h>

#include "config.h"
#include "rostinho.h"
#include "motores.h"
#include "bracos.h"
#include "sensores.h"
#include "energia.h"
#include "comunicacao.h"
#include "seguranca.h"
#include "base_carregamento.h"
#include "diagnostico.h"

// ===================================
// BIBOU - COMANDOS
// ===================================

void executarComando(String comando) {

  comando.trim();
  comando.toLowerCase();

  // EMERGENCIA
  if (comando == "emergencia") {
    ativarEmergencia();
    return;
  }

  if (comando == "emergencia:liberar") {
    liberarEmergencia();
    return;
  }

  // ROSTO
  if (comando.startsWith("rosto:")) {
    definirExpressao(comando.substring(6));
  }

  // MOTORES
  else if (comando.startsWith("motor:")) {

    String movimento = comando.substring(6);

    if (movimentoPermitido(movimento)) {
      executarMovimento(movimento);
    } else {
      Serial.println("Movimento bloqueado!");
    }
  }

  // VELOCIDADE
  else if (comando.startsWith("velocidade:")) {

    String valor = comando.substring(11);
    bool valido = valor.length() > 0;

    for (unsigned int i = 0; i < valor.length(); i++) {
      if (!isDigit(valor[i])) {
        valido = false;
        break;
      }
    }

    if (valido && valor.toInt() <= 100) {
      definirVelocidade(valor.toInt());
    } else {
      Serial.println("Velocidade invalida");
    }
  }

  // BRACOS
  else if (comando.startsWith("braco:")) {

    if (emergenciaAtiva()) {
      Serial.println("Bracos bloqueados!");
      return;
    }

    if (!executarAcaoBracos(comando.substring(6))) {
      Serial.println("Acao de braco invalida");
    }
  }

  // SIMULACAO DE BATERIA
  else if (comando.startsWith("bateria:")) {

    String valor = comando.substring(8);
    bool valido = valor.length() > 0;

    for (unsigned int i = 0; i < valor.length(); i++) {
      if (!isDigit(valor[i])) {
        valido = false;
        break;
      }
    }

    if (valido && valor.toInt() <= 100) {
      simularNivelBateria(valor.toFloat());
    } else {
      Serial.println("Nivel invalido");
    }
  }

  // CARREGAMENTO
  else if (comando == "carregar:iniciar") {
    pararMotores();
    definirCarregando(true);
  }

  else if (comando == "carregar:parar") {
    definirCarregando(false);
  }

  // CONSULTAS
  else if (comando == "sensores") {
    Serial.println(obterStatusSensores());
  }

  else if (comando == "energia") {
    Serial.println(obterStatusEnergia());
  }

  else if (comando == "seguranca") {
    Serial.println(obterStatusSeguranca());
  }

  else if (comando == "status") {

    Serial.println();
    Serial.println("===== STATUS BIBOU =====");

    Serial.println("Rosto: " + obterExpressao());
    Serial.println("Movimento: " + obterMovimento());
    Serial.println("Bracos: " + obterAcaoBracos());

    Serial.print("Velocidade: ");
    Serial.println(obterVelocidade());

    Serial.println(obterStatusSensores());
    Serial.println(obterStatusEnergia());
    Serial.println(obterStatusSeguranca());

    Serial.print("Wi-Fi: ");
    Serial.println(
      wifiEstaConectado()
        ? "conectado"
        : "desconectado"
    );

    Serial.println("========================");
  }


else if (comando == "base:retornar") {
  solicitarRetornoBase();
}

else if (comando == "base:cancelar") {
  cancelarRetornoBase();
}

else if (comando == "base:localizada") {
  simularBaseLocalizada(true);
}

else if (comando == "base:nao_localizada") {
  simularBaseLocalizada(false);
}

else if (comando == "base:encaixada") {
  simularEncaixeBase(true);
}

else if (comando == "base:desencaixada") {
  simularEncaixeBase(false);
}

else if (comando == "base:status") {
  Serial.println(obterStatusBase());
}

else if (comando == "diagnostico") {
  mostrarDiagnostico();
}

  else {
    Serial.println("Comando desconhecido: " + comando);
  }
}

// ===================================
// INICIALIZACAO
// ===================================

void setup() {

  Serial.begin(VELOCIDADE_SERIAL);
  Serial.setTimeout(50);

  Serial.println();
  Serial.println("====================");
  Serial.println("    BIBOU ESP32-S3  ");
  Serial.println("====================");

  iniciarRostinho();
  iniciarMotores();
  iniciarBracos();
  iniciarSensores();
  iniciarEnergia();
  iniciarSeguranca();
  iniciarBaseCarregamento();
  iniciarDiagnostico();
  iniciarComunicacao();

  Serial.println("BIBOU pronto!");
}

// ===================================
// EXECUCAO CONTINUA
// ===================================

void loop() {

  atualizarRostinho();
  atualizarMotores();
  atualizarBracos();
  atualizarSensores();
  atualizarEnergia();
  atualizarComunicacao();
  atualizarSeguranca();
  atualizarDiagnostico();
  atualizarBaseCarregamento();

  if (Serial.available()) {
    String comando = Serial.readStringUntil('\n');
    executarComando(comando);
  }
}
