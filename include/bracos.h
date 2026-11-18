
#ifndef BRACOS_H
#define BRACOS_H

#include <Arduino.h>

void iniciarBracos();
void atualizarBracos();

bool executarAcaoBracos(String acao);
void pararBracos();

String obterAcaoBracos();

#endif
