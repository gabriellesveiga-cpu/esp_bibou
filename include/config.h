
#ifndef CONFIG_H
#define CONFIG_H

// ===================================
// BIBOU - CONFIGURACOES GERAIS
// ===================================

// true = sem tela conectada
// false = tela TFT conectada
#define MODO_SIMULACAO true

// TELA TFT ILI9341
#define TFT_CS 5
#define TFT_DC 2
#define TFT_RST 4
#define TFT_SCK 18
#define TFT_MOSI 23

// CORES
#define COR_FUNDO 0x0000
#define COR_OLHOS 0xB59F
#define COR_DETALHE 0x07FF

// TEMPOS
#define INTERVALO_PISCADA 3500
#define DURACAO_PISCADA 150

// COMUNICACAO SERIAL
#define VELOCIDADE_SERIAL 115200

#endif
