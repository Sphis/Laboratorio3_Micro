#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <SPI.h>

#define CLK 7
#define DIN 6
#define DC  5
#define CS  4
#define RST 3

// Configuración de los pines de la pantalla.
Adafruit_PCD8544 display = Adafruit_PCD8544(CLK, DIN, DC, CS,RST); // Esto debe ir en orden.
// Variables LEDs alarma
const int LED_A3 = 8;
const int LED_A2 = 9;
const int LED_A1 = 10;
const int LED_A0 = 11;
 // Voltajes en DC
static float volt_1 = 0.0;
static float volt_2 = 0.0;
static float volt_3 = 0.0;
static float volt_4 = 0.0;
// Configuraciones generales
void setup() { 
  //Comunicacion serial
  Serial.begin(9600);
  // Canales de voltaje
  pinMode(A5, INPUT);
  pinMode(A4, INPUT);
  pinMode(A3, INPUT);
  pinMode(A2, INPUT);
  // LEDs de alarma.
  pinMode(LED_A3, OUTPUT);
  pinMode(LED_A2, OUTPUT);
  pinMode(LED_A1, OUTPUT);
  pinMode(LED_A0, OUTPUT);
  //Configuraciones de la pantalla
  display.begin();
  display.setContrast(50);  // Ajusta el contraste según sea necesario
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(5,0);
  display.println("VOLTIMETRO");
  display.display();
  delay(2000);
  display.clearDisplay(); // Clear display
}

// Funcion medir voltaje
float mide_voltaje(float mag) {
  float max_val = 0;
    for (int i = 0; i < 100; i++) {
        float val = analogRead(mag);
        if (val > max_val) {
            max_val = val;
        }
        delayMicroseconds(150);
    }
    max_val = (((511.0-max_val)* 48/1023.0)); // normalizar y escalar.
    return max_val;
}

// Funcion encargada de indicar sobrepaso con leds
void alarma_DC(float medida1, float medida2, float medida3, float medida4) {
  // Primer LED
  if(-20 >= medida1 || medida1 >= 20){
    digitalWrite(LED_A3, HIGH);
  }
  else{
    digitalWrite(LED_A3, LOW);
  }
  // Segundo LED
  if(-20 >= medida2 || medida2 >= 20){
    digitalWrite(LED_A2, HIGH);
  }
  else{
    digitalWrite(LED_A2, LOW);
  }
  // Tercer LED
  if(-20 >= medida3 || medida3 >= 20){
    digitalWrite(LED_A1, HIGH);
  }
  else{
    digitalWrite(LED_A1, LOW);
  }
  // Cuarto LED
  if(-20 >= medida4 || medida4 >= 20){
    digitalWrite(LED_A0, HIGH);
  }
  else{
    digitalWrite(LED_A0, LOW);
  }
}

// Alarma AC
void alarma_AC(float medida1, float medida2, float medida3, float medida4){
  if(-14.14 >= medida1 || medida1 >= 14.14){
    digitalWrite(LED_A3, HIGH);
  }
  else{
    digitalWrite(LED_A3, LOW);
  }
  // Segundo LED
  if(-14.14 >= medida2 || medida2 >= 14.14){
    digitalWrite(LED_A2, HIGH);
  }
  else{
    digitalWrite(LED_A2, LOW);
  }
  // Tercer LED
  if(-14.14 >= medida3 || medida3 >= 14.14){
    digitalWrite(LED_A1, HIGH);
  }
  else{
    digitalWrite(LED_A1, LOW);
  }
  // Cuarto LED
  if(-14.14 >= medida4 || medida4 >= 14.14){
    digitalWrite(LED_A0, HIGH);
  }
  else{
    digitalWrite(LED_A0, LOW);
  }
}

// Mostrar valores en pantalla
void tensiones_DC(){
  display.print("V1: ");display.print(volt_1);display.print("V");
  display.print("\n");
  display.print("V2: ");display.print(volt_2);display.print("V");
  display.print("\n");
  display.print("V3: ");display.print(volt_3);display.print("V");
  display.print("\n");
  display.print("V4: ");display.print(volt_4);display.print("V");
  display.display();
  display.clearDisplay();
}

void tensiones_AC(){
  display.print("V1: ");display.print(volt_1);display.print(" rms");
  display.print("\n");
  display.print("V2: ");display.print(volt_2);display.print(" rms");
  display.print("\n");
  display.print("V3: ");display.print(volt_3);display.print(" rms");
  display.print("\n");
  display.print("V4: ");display.print(volt_4);display.print(" rms");
  display.display();
  display.clearDisplay();
}

// Funcion encargada de la comunicacion
void comunicacion(float medida1, float medida2, float medida3, float medida4, float mode){
  if (mode){
      // Comunicacion serial AC
      Serial.print("Voltaje canal 1 (AC): ");
      Serial.println(volt_1);
      Serial.print("Voltaje canal 2 (AC): ");
      Serial.println(volt_2);
      Serial.print("Voltaje canal 3 (AC): ");
      Serial.println(volt_3);
      Serial.print("Voltaje canal 4 (AC): ");
      Serial.println(volt_4);
  }
  else{
      // Comunicacion serial DC
      Serial.print("Voltaje canal 1 (DC): ");
      Serial.println(volt_1);
      Serial.print("Voltaje canal 2 (DC): ");
      Serial.println(volt_2);
      Serial.print("Voltaje canal 3 (DC): ");
      Serial.println(volt_3);
      Serial.print("Voltaje canal 4 (DC): ");
      Serial.println(volt_4);
  }
}

// Loop principal donde se ejecutan las funciones del voltimetro.
void loop() {
    float mode = analogRead(A0); // Lectura de voltajes en AC
    if (mode){
      display.print("Modo AC \n");
      volt_1 = mide_voltaje(A5);
      volt_1 = (volt_1/sqrt(2)); // Conversión a rms
      volt_2 = mide_voltaje(A4);
      volt_2 = (volt_2/sqrt(2)); // Conversión a rms
      volt_3 = mide_voltaje(A3);
      volt_3 = (volt_3/sqrt(2)); // Conversión a rms
      volt_4 = mide_voltaje(A2); 
      volt_4 = (volt_4/sqrt(2)); // Conversión a rms
      tensiones_AC(); // Muestra magnitudes en la pantalla
      alarma_AC(volt_1, volt_2, volt_3, volt_4); // Alarma de sobre-tensiones
      comunicacion(volt_1, volt_2, volt_3, volt_4, mode);
    }
    else{
      display.print("Modo DC \n"); // Si el botón está en bajo, se muestran las tensiones en DC
      volt_1 = mide_voltaje(A5);
      volt_2 = mide_voltaje(A4);
      volt_3 = mide_voltaje(A3);
      volt_4 = mide_voltaje(A2);  
      tensiones_DC(); // Muestra magnitudes en la pantalla
      alarma_DC(volt_1, volt_2, volt_3, volt_4); // Alarma de los LEDs de sobre-voltaje.
      comunicacion(volt_1, volt_2, volt_3, volt_4, mode);
    }
  }