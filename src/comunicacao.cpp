
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#include "wifi_config.h"
#include "comunicacao.h"
#include "rostinho.h"
#include "motores.h"
#include "bracos.h"
#include "sensores.h"
#include "energia.h"
#include "seguranca.h"
#include "base_carregamento.h"

// =====================================
// BIBOU - COMUNICACAO COM BIBOU OS
// =====================================

static WebServer servidor(80);

static unsigned long ultimaTentativaWiFi = 0;
static bool servidorIniciado = false;

static const unsigned long INTERVALO_WIFI = 10000;

// Funcao definida no main.cpp
extern void executarComando(String comando);

// =====================================
// FUNCOES AUXILIARES
// =====================================

String textoJSON(String texto) {
  String resultado = "";

  for (unsigned int i = 0; i < texto.length(); i++) {
    char c = texto[i];

    if (c == '"' || c == '\\') {
      resultado += '\\';
      resultado += c;
    }
    else if (c == '\n') {
      resultado += "\\n";
    }
    else if (c == '\r') {
      resultado += "\\r";
    }
    else if ((uint8_t)c >= 32) {
      resultado += c;
    }
  }

  return resultado;
}

String campoTexto(String nome, String valor) {
  return "\"" + nome + "\":\"" +
         textoJSON(valor) + "\"";
}

String campoBool(String nome, bool valor) {
  return "\"" + nome + "\":" +
         String(valor ? "true" : "false");
}

String campoNumero(String nome, float valor) {
  return "\"" + nome + "\":" +
         String(valor, 1);
}

// =====================================
// SEGURANCA DA API
// =====================================

bool autorizado() {
  String chave = servidor.header("X-Bibou-Key");

  return (
    chave.length() > 0 &&
    chave == CHAVE_API &&
    chave != "SUA_CHAVE_PRIVADA_LONGA_E_ALEATORIA" &&
    chave != "COLOQUE_UMA_CHAVE_LONGA_E_ALEATORIA"
  );
}

bool verificarAcesso() {
  if (autorizado()) {
    return true;
  }

  servidor.send(
    401,
    "application/json",
    "{\"erro\":\"Acesso nao autorizado\"}"
  );

  return false;
}

// =====================================
// CORS PARA DESENVOLVIMENTO LOCAL
// =====================================

bool origemPermitida(String origem) {
  if (origem.length() == 0) {
    return true;
  }

  // Somente desenvolvimento local no PC.
  return (
    origem.startsWith("http://localhost:") ||
    origem.startsWith("http://127.0.0.1:")
  );
}

void adicionarCORS() {
  String origem = servidor.header("Origin");

  if (
    origem.length() > 0 &&
    origemPermitida(origem)
  ) {
    servidor.sendHeader(
      "Access-Control-Allow-Origin",
      origem
    );

    servidor.sendHeader(
      "Vary",
      "Origin"
    );

    servidor.sendHeader(
      "Access-Control-Allow-Methods",
      "GET, POST, OPTIONS"
    );

    servidor.sendHeader(
      "Access-Control-Allow-Headers",
      "Content-Type, X-Bibou-Key"
    );
  }
}

// =====================================
// STATUS DO BIBOU
// =====================================

String gerarStatusJSON() {

  String json = "{";

  json += campoTexto("nome", "BIBOU") + ",";
  json += campoBool("online", true) + ",";

  // WIFI
  json += "\"wifi\":{";
  json += campoBool(
    "conectado",
    wifiEstaConectado()
  ) + ",";
  json += campoTexto(
    "ip",
    obterIPBibou()
  );
  json += "},";

  // ROSTO
  json += campoTexto(
    "expressao",
    obterExpressao()
  ) + ",";

  // MOTORES
  json += "\"motores\":{";
  json += campoTexto(
    "movimento",
    obterMovimento()
  ) + ",";
  json += campoNumero(
    "velocidade",
    obterVelocidade()
  );
  json += "},";

  // BRACOS
  json += campoTexto(
    "bracos",
    obterAcaoBracos()
  ) + ",";

  // SENSORES
  json += "\"sensores\":{";
  json += campoBool("simulados", true) + ",";
  json += campoNumero(
    "distancia",
    obterDistancia()
  ) + ",";
  json += campoBool(
    "obstaculo",
    detectarObstaculo()
  ) + ",";
  json += campoNumero(
    "inclinacao",
    obterInclinacao()
  ) + ",";
  json += "\"encoder_esquerdo\":" +
          String(obterEncoderEsquerdo()) + ",";
  json += "\"encoder_direito\":" +
          String(obterEncoderDireito());
  json += "},";

  // ENERGIA
  json += "\"energia\":{";
  json += campoBool("simulada", true) + ",";
  json += campoNumero(
    "bateria",
    obterNivelBateria()
  ) + ",";
  json += campoBool(
    "carregando",
    estaCarregando()
  ) + ",";
  json += campoBool(
    "baixa",
    bateriaBaixa()
  ) + ",";
  json += campoBool(
    "critica",
    bateriaCritica()
  );
  json += "},";

  // SEGURANCA
  json += "\"seguranca\":{";
  json += campoBool(
    "emergencia",
    emergenciaAtiva()
  );
  json += "},";

  // BASE
  json += "\"base\":{";
  json += campoTexto(
    "estado",
    obterEstadoBase()
  ) + ",";
  json += campoBool(
    "retorno_ativo",
    retornoBaseAtivo()
  );
  json += "}";

  json += "}";

  return json;
}

// =====================================
// VALIDACAO DE COMANDOS
// =====================================

bool comandoPermitido(String comando) {

  comando.trim();
  comando.toLowerCase();

  // Comandos exatos: movimentos
  if (
    comando == "motor:frente" ||
    comando == "motor:tras" ||
    comando == "motor:esquerda" ||
    comando == "motor:direita" ||
    comando == "motor:parar"
  ) {
    return true;
  }

  // Expressoes
  if (
    comando == "rosto:normal" ||
    comando == "rosto:feliz" ||
    comando == "rosto:triste" ||
    comando == "rosto:surpreso" ||
    comando == "rosto:sono" ||
    comando == "rosto:bravo" ||
    comando == "rosto:esquerda" ||
    comando == "rosto:direita"
  ) {
    return true;
  }

  // Bracos
  if (
    comando == "braco:acenar" ||
    comando == "braco:levantar" ||
    comando == "braco:abaixar" ||
    comando == "braco:abrir" ||
    comando == "braco:dancar" ||
    comando == "braco:parar"
  ) {
    return true;
  }

  // Velocidade
  if (comando.startsWith("velocidade:")) {

    String numero = comando.substring(11);

    if (numero.length() == 0) return false;

    for (unsigned int i = 0; i < numero.length(); i++) {
      if (!isDigit(numero[i])) return false;
    }

    return numero.toInt() <= 100;
  }

  // Seguranca
  if (comando == "emergencia") {
    return true;
  }

  // Base de carregamento
  if (
    comando == "base:retornar" ||
    comando == "base:cancelar"
  ) {
    return true;
  }

  return false;
}

// =====================================
// ROTAS HTTP
// =====================================

void configurarRotas() {

  const char* headers[] = {
    "X-Bibou-Key",
    "Origin"
  };

  servidor.collectHeaders(headers, 2);

  // Teste de conexao
  servidor.on("/ping", HTTP_GET, []() {

    adicionarCORS();

    servidor.send(
      200,
      "application/json",
      "{\"mensagem\":\"BIBOU ONLINE\"}"
    );
  });

  // Status completo
  servidor.on("/status", HTTP_GET, []() {

    adicionarCORS();

    if (!verificarAcesso()) return;

    servidor.send(
      200,
      "application/json",
      gerarStatusJSON()
    );
  });

  // Enviar comando
  servidor.on("/comando", HTTP_POST, []() {

    adicionarCORS();

    if (!verificarAcesso()) return;

    String comando = servidor.arg("comando");

    comando.trim();
    comando.toLowerCase();

    if (!comandoPermitido(comando)) {
      servidor.send(
        400,
        "application/json",
        "{\"erro\":\"Comando invalido\"}"
      );
      return;
    }

    // Executa no main.cpp
    executarComando(comando);

    servidor.send(
      200,
      "application/json",
      gerarStatusJSON()
    );
  });

  // Preflight do navegador
  servidor.on("/comando", HTTP_OPTIONS, []() {

    String origem = servidor.header("Origin");

    if (!origemPermitida(origem)) {
      servidor.send(403);
      return;
    }

    adicionarCORS();
    servidor.send(204);
  });

  servidor.on("/status", HTTP_OPTIONS, []() {

    String origem = servidor.header("Origin");

    if (!origemPermitida(origem)) {
      servidor.send(403);
      return;
    }

    adicionarCORS();
    servidor.send(204);
  });

  servidor.onNotFound([]() {
    servidor.send(
      404,
      "application/json",
      "{\"erro\":\"Rota nao encontrada\"}"
    );
  });
}

// =====================================
// INICIAR COMUNICACAO
// =====================================

void iniciarComunicacao() {

  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);

  configurarRotas();

  Serial.println("Conectando ao hotspot do iPhone...");

  WiFi.begin(
    WIFI_CELULAR,
    SENHA_CELULAR
  );

  servidor.begin();
  servidorIniciado = true;

  Serial.println("Servidor BIBOU preparado!");
}

// =====================================
// ATUALIZACAO
// =====================================

void atualizarComunicacao() {

  if (servidorIniciado) {
    servidor.handleClient();
  }

  unsigned long agora = millis();

  if (
    WiFi.status() != WL_CONNECTED &&
    agora - ultimaTentativaWiFi >= INTERVALO_WIFI
  ) {

    ultimaTentativaWiFi = agora;

    Serial.println("Tentando reconectar Wi-Fi...");

    WiFi.reconnect();
  }
}

// =====================================
// CONSULTAS
// =====================================

bool wifiEstaConectado() {
  return WiFi.status() == WL_CONNECTED;
}

String obterIPBibou() {

  if (!wifiEstaConectado()) {
    return "";
  }

  return WiFi.localIP().toString();
}
