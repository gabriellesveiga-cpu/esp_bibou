
#ifndef COMUNICACAO_H
#define COMUNICACAO_H

#include <Arduino.h>

void iniciarComunicacao();
void atualizarComunicacao();

bool wifiEstaConectado();
String obterIPBibou();

#endif
