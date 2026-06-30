#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"

// Usamos el puerto Serial 2 del ESP32 (Pines 16 y 17)
HardwareSerial mySoftwareSerial(2); 
DFRobotDFPlayerMini myDFPlayer;

void setup() {
  Serial.begin(115200);
  mySoftwareSerial.begin(9600, SERIAL_8N1, 16, 17);

  Serial.println("\n--- PRUEBA DE DIAGNÓSTICO DFPLAYER MINI ---");
  Serial.println("Iniciando comunicación con el módulo...");

  // Intentar conectar con el parlante
  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println("❌ ERROR FATAL: No se pudo comunicar con el DFPlayer.");
    Serial.println("Revisa los cables TX y RX, la energía o la MicroSD.");
    
    // Detener el programa aquí si hay error
    while(true); 
  }

  Serial.println("✅ DFPlayer Mini detectado correctamente.");
  
  // Establecer volumen (0 a 30)
  myDFPlayer.volume(20); 
  Serial.println("Volumen configurado a 20. Iniciando prueba de sonido...");
  delay(2000);
}

void loop() {
  Serial.println("▶️ Reproduciendo: 0007.mp3 (Carpeta mp3) - Esperando 30 segundos...");
  myDFPlayer.playMp3Folder(7);
  delay(30000); // 30,000 milisegundos = 30 segundos

  Serial.println("▶️ Reproduciendo: 0001.mp3 (Carpeta mp3) - Esperando 30 segundos...");
  myDFPlayer.playMp3Folder(1);
  delay(30000); // 30,000 milisegundos = 30 segundos

  Serial.println("▶️ Reproduciendo: 0002.mp3 (Carpeta mp3) - Esperando 30 segundos...");
  myDFPlayer.playMp3Folder(2);
  delay(30000); // 30,000 milisegundos = 30 segundos
  
  Serial.println("🔄 Reiniciando ciclo de prueba...");
  Serial.println("-----------------------------------");
}