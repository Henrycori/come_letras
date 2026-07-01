#include <SPI.h>
#include <TFT_eSPI.h> // La librería gráfica para el ESP32
#include "mi_icono.h" // Esto llama a tu archivo de la imagen

TFT_eSPI tft = TFT_eSPI(); // Creamos el objeto de la pantalla

void setup() {
  Serial.begin(115200);
  
  // Inicializamos el controlador físico de la pantalla
  tft.init();
  
  // Rotación de la pantalla (puedes probar con 0, 1, 2 o 3 si se ve al revés)
  tft.setRotation(1); 
  
  // Limpiamos todo el fondo pintándolo de negro
  tft.fillScreen(TFT_BLACK);

  Serial.println("Renderizando el icono...");

  // Dibujamos la imagen en la pantalla
  // Parámetros: (Posición_X, Posición_Y, Ancho, Alto, Nombre_del_Array)
  // Nota: "image_data_mi_icono" es el nombre exacto que sale en tu captura de pantalla
  tft.pushImage(40, 40, 64, 64, image_data_mi_icono); 
}

void loop() {
  // No se necesita nada en el bucle para mantener la imagen fija
}