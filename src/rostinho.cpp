
#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#include "config.h"
#include "rostinho.h"

Adafruit_ILI9341 tela(TFT_CS, TFT_DC, TFT_RST);

String expressaoAtual = "normal";

unsigned long ultimaPiscada = 0;
unsigned long fimPiscada = 0;

bool piscando = false;

// ===================================
// DESENHAR OLHO
// ===================================

void desenharOlho(
  int x,
  int y,
  int largura,
  int altura
) {
  tela.fillRoundRect(
    x,
    y,
    largura,
    altura,
    min(largura, altura) / 2,
    COR_OLHOS
  );
}

// ===================================
// DESENHAR EXPRESSAO
// ===================================

void desenharExpressao() {

  if (MODO_SIMULACAO) {
    Serial.println(
      "Expressao: " + expressaoAtual
    );
    return;
  }

  tela.fillScreen(COR_FUNDO);

  if (expressaoAtual == "normal") {
    desenharOlho(85, 85, 45, 70);
    desenharOlho(190, 85, 45, 70);
  }

  else if (expressaoAtual == "feliz") {
    tela.drawRoundRect(
      85, 105, 45, 35, 15, COR_OLHOS
    );

    tela.drawRoundRect(
      190, 105, 45, 35, 15, COR_OLHOS
    );

    desenharOlho(85, 125, 45, 10);
    desenharOlho(190, 125, 45, 10);
  }

  else if (expressaoAtual == "triste") {
    desenharOlho(90, 110, 35, 45);
    desenharOlho(195, 110, 35, 45);

    tela.fillCircle(
      130, 165, 5, COR_DETALHE
    );
  }

  else if (expressaoAtual == "surpreso") {
    desenharOlho(95, 75, 30, 85);
    desenharOlho(200, 75, 30, 85);
  }

  else if (expressaoAtual == "sono") {
    desenharOlho(85, 120, 45, 6);
    desenharOlho(190, 120, 45, 6);
  }

  else if (expressaoAtual == "bravo") {
    tela.fillTriangle(
      85, 95,
      130, 110,
      85, 125,
      COR_OLHOS
    );

    tela.fillTriangle(
      235, 95,
      190, 110,
      235, 125,
      COR_OLHOS
    );
  }

  else if (expressaoAtual == "esquerda") {
    desenharOlho(65, 85, 45, 70);
    desenharOlho(170, 85, 45, 70);
  }

  else if (expressaoAtual == "direita") {
    desenharOlho(105, 85, 45, 70);
    desenharOlho(210, 85, 45, 70);
  }
}

// ===================================
// INICIAR ROSTINHO
// ===================================

void iniciarRostinho() {

  if (!MODO_SIMULACAO) {
    SPI.begin(
      TFT_SCK,
      -1,
      TFT_MOSI,
      TFT_CS
    );

    tela.begin();
    tela.setRotation(1);
  }

  desenharExpressao();
}

// ===================================
// DEFINIR EXPRESSAO
// ===================================

void definirExpressao(String novaExpressao) {

  novaExpressao.trim();
  novaExpressao.toLowerCase();

  if (
    novaExpressao == "normal" ||
    novaExpressao == "feliz" ||
    novaExpressao == "triste" ||
    novaExpressao == "surpreso" ||
    novaExpressao == "sono" ||
    novaExpressao == "bravo" ||
    novaExpressao == "esquerda" ||
    novaExpressao == "direita"
  ) {

    expressaoAtual = novaExpressao;
    piscando = false;

    desenharExpressao();
  }
}

// ===================================
// CONSULTAR EXPRESSAO
// ===================================

String obterExpressao() {
  return expressaoAtual;
}

// ===================================
// ATUALIZAR ANIMACOES
// ===================================

void atualizarRostinho() {

  unsigned long agora = millis();

  if (piscando && agora >= fimPiscada) {
    piscando = false;
    desenharExpressao();
  }

  if (
    expressaoAtual == "normal" &&
    !piscando &&
    agora - ultimaPiscada >= INTERVALO_PISCADA
  ) {

    piscando = true;

    fimPiscada =
      agora + DURACAO_PISCADA;

    ultimaPiscada = agora;

    if (MODO_SIMULACAO) {
      Serial.println("BIBOU piscou!");
    }

    else {
      tela.fillScreen(COR_FUNDO);

      desenharOlho(85, 120, 45, 6);
      desenharOlho(190, 120, 45, 6);
    }
  }
}
