
#ifndef SEGURANCA_H
#define SEGURANCA_H

#include <Arduino.h>

void iniciarSeguranca();
void atualizarSeguranca();

bool movimentoPermitido(String movimento);

void ativarEmergencia();
void liberarEmergencia();

bool emergenciaAtiva();

String obterStatusSeguranca();

#endif

