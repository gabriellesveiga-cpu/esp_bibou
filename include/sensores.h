
#ifndef SENSORES_H
#define SENSORES_H

#include <Arduino.h>

void iniciarSensores();
void atualizarSensores();

float obterDistancia();
bool detectarObstaculo();
float obterInclinacao();

long obterEncoderEsquerdo();
long obterEncoderDireito();

String obterStatusSensores();

#endif

