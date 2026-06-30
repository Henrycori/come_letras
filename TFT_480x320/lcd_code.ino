#include <WiFi.h>
#include <PubSubClient.h>
#include <UTFTGLUE.h>
#include "imagenes.h"

const char* ssid = "PEPANET";
const char* password = "C0PoDn13vE2@";
const char* mqtt_server = "38.250.161.112";
const int mqtt_port = 1883;
const char* mqtt_user = "lotus";
const char* mqtt_pass = "meinherz777";

UTFTGLUE myGLCD(0,13,12,33,32,15);
const int ESCALA_PIXEL = 6;

WiFiClient espClient;
PubSubClient client(espClient);

void dibujarIconoHD(const char* mapa[], int x_inicio, int y_inicio, int r, int g, int b);
void procesarComando(String comando);

void callback(char* topic, byte* payload, unsigned int length) {
  String comando = "";
  for (int i = 0; i < length; i++) {
    comando += (char)payload[i];
  }
  
  comando.trim(); 
  comando.toUpperCase();
  
  Serial.print("Comando limpio recibido: [");
  Serial.print(comando);
  Serial.println("]");
  
  procesarComando(comando);
}

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); }
}

void reconnect() {
  while (!client.connected()) {
    String clientId = "ESP32_Dashboard_LCD";
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
      client.subscribe("esp32/lcd/comando");
    } else {
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  myGLCD.InitLCD();
  myGLCD.setRotation(3);
  myGLCD.fillScr(255, 255, 255);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void procesarComando(String comando) {
  myGLCD.fillScr(255, 255, 255);

  if (comando == "SOL") {
    dibujarIconoHD(sol_map, 120, 40, 255, 204, 0);
  } 
  else if (comando == "OSO") {
    dibujarIconoHD(oso_map, 120, 40, 140, 80, 35);
  } 
  else if (comando == "SAL") {
    dibujarIconoHD(sal_map, 120, 40, 180, 185, 190);
  } 
  else if (comando == "ALA") {
    dibujarIconoHD(ala_map, 120, 40, 100, 200, 240);
  } 
  else if (comando == "OJO") {
    dibujarIconoHD(ojo_map, 120, 40, 30, 110, 220);
  } 
  else if (comando == "ERROR") {
    myGLCD.fillScr(255, 0, 0);
    delay(3000);
    myGLCD.fillScr(255, 255, 255);
  }
}

void dibujarIconoHD(const char* mapa[], int x_inicio, int y_inicio, int r, int g, int b) {
  for (int y = 0; y < FILAS; y++) {
    for (int x = 0; x < COLUMNAS; x++) {

      if (mapa[y][x] == '\0') {
        break; 
      }
      
      char caracter = mapa[y][x];
      
      if (caracter == 'X') {
        myGLCD.setColor(0, 0, 0);
        myGLCD.fillRect(x_inicio + (x * ESCALA_PIXEL), 
                        y_inicio + (y * ESCALA_PIXEL), 
                        x_inicio + (x * ESCALA_PIXEL) + ESCALA_PIXEL - 1, 
                        y_inicio + (y * ESCALA_PIXEL) + ESCALA_PIXEL - 1);
      }
      else if (caracter == '#') {
        myGLCD.setColor(r, g, b);
        myGLCD.fillRect(x_inicio + (x * ESCALA_PIXEL), 
                        y_inicio + (y * ESCALA_PIXEL), 
                        x_inicio + (x * ESCALA_PIXEL) + ESCALA_PIXEL - 1, 
                        y_inicio + (y * ESCALA_PIXEL) + ESCALA_PIXEL - 1);
      }
    }
  }
}