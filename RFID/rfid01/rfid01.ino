#include <WiFi.h>
#include <SPI.h>
#include <MFRC522.h>
#include <PubSubClient.h>
#include "Arduino.h"
#include "DFRobotDFPlayerMini.h" 

// --- CONFIGURACIÓN DE RED Y SERVIDOR ---
const char* ssid = "ZKronosX";
const char* password = "$Power2026";
const char* mqtt_server = "38.250.161.112";  
const int mqtt_port = 1883;                    

const char* mqtt_user = "lotus";
const char* mqtt_pass = "meinherz777";

// --- PINES Y OBJETOS DEL RFID ---
#define SDA_PIN   21    
#define RST_PIN   22    
MFRC522 mfrc522(SDA_PIN, RST_PIN);

// --- OBJETOS DE RED Y PARLANTE ---
WiFiClient espClient;
PubSubClient client(espClient);
HardwareSerial mySoftwareSerial(2); 
DFRobotDFPlayerMini myDFPlayer;

// --- VARIABLES PARA EL TEMPORIZADOR BÁSICO ---
unsigned long tiempoUltimaLectura = 0;
// Dejamos 1 segundo de cooldown físico para que el audio empiece limpio
const long tiempoEspera = 1000; 

void setup_wifi() {
  delay(10);
  Serial.println("\nConfigurando conexión WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  WiFi.begin(ssid, password);
  
  int intentos = 0;
  while (WiFi.status() != WL_CONNECTED && intentos < 30) {
    delay(500);
    Serial.print(".");
    intentos++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ ¡WiFi Conectado exitosamente!");
  } else {
    Serial.println("\n❌ Advertencia: No se pudo conectar al WiFi.");
  }
}

void reconnect() {
  while (!client.connected() && WiFi.status() == WL_CONNECTED) {
    Serial.print("Intentando conexión MQTT...");
    String clientId = "ESP32_Lector_RFID"; // Nombre único para este ESP32
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
      Serial.println("¡Conectado al Broker!");
      client.publish("esp32/dashboard/estado", "Lector en Linea");
    } else {
      Serial.print("Falló... Reintentando en 5 segundos...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  mySoftwareSerial.begin(9600, SERIAL_8N1, 16, 17);
  
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  
  SPI.begin();            
  mfrc522.PCD_Init();    
  
  Serial.println("Iniciando DFPlayer...");
  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println("❌ ERROR: Parlante no detectado.");
  } else {
    Serial.println("✅ Parlante Listo.");
    myDFPlayer.volume(20); 
  }

  Serial.println("Sistema Listo.");
}

void loop() {
  if (!client.connected() && WiFi.status() == WL_CONNECTED) {
    reconnect();
  }
  client.loop(); 

  // Temporizador no bloqueante
  if (millis() - tiempoUltimaLectura < tiempoEspera) {
    return; 
  }

  if ( ! mfrc522.PICC_IsNewCardPresent() ) return;
  if ( ! mfrc522.PICC_ReadCardSerial() ) return;

  String uidLeido = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if(mfrc522.uid.uidByte[i] < 0x10) uidLeido += "0";
    uidLeido += String(mfrc522.uid.uidByte[i], HEX);
  }
  uidLeido.toUpperCase();
  uidLeido.trim();

  String letra = "";
  String componente = "";
  int numeroPista = 0; 

  // --- ASIGNACIÓN DE COMPONENTES ---
  if (uidLeido == "6A083E02") { letra = "A"; componente = "Memorias Ram"; numeroPista = 1; }
  else if (uidLeido == "A3D32530") { letra = "L"; componente = "Almacenamiento Solido SSD"; numeroPista = 2; }
  else if (uidLeido == "13D49E35") { letra = "R"; componente = "DDR4 SODIMM Kingston"; numeroPista = 3; }
  else if (uidLeido == "23174934") { letra = "S"; componente = "SSD SATA Crucial"; numeroPista = 4; }
  else if (uidLeido == "A373330E") { letra = "O"; componente = "AirTag Ob jeto O"; numeroPista = 5; }
  else if (uidLeido == "033655A8") { letra = "J"; componente = "Componente J"; numeroPista = 6; }

  if (letra != "") {
    // Actualizamos el tiempo para no saturar
    tiempoUltimaLectura = millis(); 
    
    Serial.print("Tarjeta Detectada [");
    Serial.print(letra);
    Serial.print("]: ");
    Serial.println(componente);

    // 1. Suena el parlante sin importar nada más
    myDFPlayer.play(numeroPista);
      
    // 2. Envía la letra a MQTT (Node-RED se encargará de hacer la magia)
    if (client.connected()) {
      client.publish("esp32/dashboard/letra", letra.c_str());
      client.publish("esp32/dashboard/componente", componente.c_str());
    }

  } else {
    Serial.print("Tarjeta desconocida. UID: ");
    Serial.println(uidLeido);
    tiempoUltimaLectura = millis(); 
  }

  // Cortar comunicación con la tarjeta actual para evitar rebotes físicos
  mfrc522.PICC_HaltA();       
  mfrc522.PCD_StopCrypto1();  
}