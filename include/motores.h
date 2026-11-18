
#ifndef MOTORES_H
#define MOTORES_H

#include <Arduino.h>

void iniciarMotores();
void atualizarMotores();

bool executarMovimento(String comando);
void pararMotores();

void definirVelocidade(int velocidade);

String obterMovimento();
int obterVelocidade();

#endif
