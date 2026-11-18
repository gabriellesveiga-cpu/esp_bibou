
#ifndef BASE_CARREGAMENTO_H
#define BASE_CARREGAMENTO_H

#include <Arduino.h>

void iniciarBaseCarregamento();
void atualizarBaseCarregamento();

void solicitarRetornoBase();
void cancelarRetornoBase();

void simularBaseLocalizada(bool localizada);
void simularEncaixeBase(bool encaixado);

bool retornoBaseAtivo();
String obterEstadoBase();
String obterStatusBase();

#endif
