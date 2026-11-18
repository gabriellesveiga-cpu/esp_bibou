
#include <Arduino.h>
#include <Arduino_GFX_Library.h>

// =====================================
// BIBOU - TESTE DA TELA ILI9341
// INTERFACE PARALELA DE 8 BITS
// =====================================

// Pinos de controle
#define LCD_CS   16
#define LCD_DC   13
#define LCD_RST  15
#define LCD_WR   12

// Pinos de dados
#define LCD_D0    4
#define LCD_D1    5
#define LCD_D2    6
#define LCD_D3    7
#define LCD_D4    8
#define LCD_D5    9
#define LCD_D6   10
#define LCD_D7   11

// Barramento paralelo ESP32-S3
Arduino_DataBus *barramento =
  new Arduino_ESP32PAR8(
    LCD_DC,
    LCD_CS,
    LCD_WR,
    GFX_NOT_DEFINED,
    LCD_D0,
    LCD_D1,
    LCD_D2,
    LCD_D3,
    LCD_D4,
    LCD_D5,
    LCD_D6,
    LCD_D7
  );

// Controlador ILI9341
Arduino_GFX *tela =
  new Arduino_ILI9341(
    barramento,
    LCD_RST,
    1
  );

// Cores
#define FUNDO 0x0000
#define ROXO  0xB59F

void setup() {

  Serial.begin(115200);

  Serial.println("Iniciando tela BIBOU...");

  if (!tela->begin()) {
    Serial.println("Falha ao iniciar a tela!");
    return;
  }

  tela->fillScreen(FUNDO);

  // Olho esquerdo
  tela->fillRoundRect(
    85, 80,
    45, 75,
    22,
    ROXO
  );

  // Olho direito
  tela->fillRoundRect(
    190, 80,
    45, 75,
    22,
    ROXO
  );

  Serial.println("Olhinhos desenhados!");
}

void loop() {
  // Primeiro teste: imagem parada
}
