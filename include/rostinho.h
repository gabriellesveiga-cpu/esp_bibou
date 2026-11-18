
#ifndef ROSTINHO_H
#define ROSTINHO_H

#include <Arduino.h>

void iniciarRostinho();
void atualizarRostinho();

void definirExpressao(String novaExpressao);
String obterExpressao();

#endif
