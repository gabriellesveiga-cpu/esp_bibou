
#ifndef ENERGIA_H
#define ENERGIA_H

#include <Arduino.h>

void iniciarEnergia();
void atualizarEnergia();

float obterNivelBateria();
float obterTensaoBateria();

bool bateriaBaixa();
bool bateriaCritica();
bool estaCarregando();

bool precisaRetornarBase();
void definirCarregando(bool carregando);

// Apenas para testes em simulacao
void simularNivelBateria(float nivel);

String obterStatusEnergia();

#endif
