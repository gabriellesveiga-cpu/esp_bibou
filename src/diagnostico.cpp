
#include <Arduino.h>

#include "diagnostico.h"
#include "comunicacao.h"
#include "motores.h"
#include "rostinho.h"
#include "energia.h"
#include "seguranca.h"
#include "base_carregamento.h"

static unsigned long ultimoDiagnostico = 0;
static const unsigned long INTERVALO = 30000;

void iniciarDiagnostico() {
  Serial.println("Diagnostico BIBOU iniciado!");
}

void mostrarDiagnostico() {

  Serial.println();
  Serial.println("===== DIAGNOSTICO BIBOU =====");

  Serial.print("Tempo ligado: ");
  Serial.print(millis() / 1000);
  Serial.println(" segundos");

  Serial.print("Memoria livre: ");
  Serial.print(ESP.getFreeHeap());
  Serial.println(" bytes");

  Serial.print("Wi-Fi: ");
  Serial.println(
    wifiEstaConectado() ? "conectado" : "desconectado"
  );

  Serial.print("IP: ");
  Serial.println(obterIPBibou());

  Serial.print("Expressao: ");
  Serial.println(obterExpressao());

  Serial.print("Movimento: ");
  Serial.println(obterMovimento());

  Serial.print("Bateria: ");
  Serial.print(obterNivelBateria());
  Serial.println("%");

  Serial.print("Emergencia: ");
  Serial.println(
    emergenciaAtiva() ? "ATIVA" : "inativa"
  );

  Serial.print("Base: ");
  Serial.println(obterEstadoBase());

  Serial.println("============================");
}

void atualizarDiagnostico() {

  unsigned long agora = millis();

  if (agora - ultimoDiagnostico >= INTERVALO) {
    ultimoDiagnostico = agora;
    mostrarDiagnostico();
  }
}
