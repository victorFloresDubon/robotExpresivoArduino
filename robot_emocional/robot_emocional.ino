#include <LedControl.h>
#include <Servo.h>
#include <String.h>
#define CABEZA_PIN 3
#define BRAZO_IZQ_PIN 5
#define BRAZO_DER_PIN 7

#define delMatrix 1000
#define DEL_SERVO 500

LedControl lc = LedControl(11, 13, 10, 1);

String mensaje;

const char* keyWord[] = {
  "feliz",
  "bien",
  "triste",
  "enojado",
  "bravo",
  "good",
  "bad",
  "sad",
  "angry",
  "mad",
  "nada",
  "alegre",
  "genial",
  "contento"
};

const byte alegre[8] = {
  B00000000,
  B01100110,
  B01100110,
  B00011000,
  B00011000,
  B01000010,
  B00111100,
  B00000000
};

const byte triste[8] = {
  B00000000,
  B01100110,
  B01100110,
  B00011000,
  B00011000,
  B00111100,
  B01000010,
  B00000000
};

const byte enojado[8] = {
  B00000000,
  B01000010,
  B00100100,
  B00000000,
  B00000000,
  B00111100,
  B01000010,
  B00000000
};

const byte inexpresivo[8] = {
  B00000000,
  B01100110,
  B01100110,
  B00011000,
  B00011000,
  B00000000,
  B01111110,
  B00000000
};

// Servo motores para las articulaciones
Servo cabeza,
      brazoIzq,
      brazoDer;

void generarTension(){
  for(byte i=0; i < 8; i++){
    for(byte j=0; j < 8; j++){
      lc.setLed(0, i, j, true);
      delay(100);
      lc.setLed(0, i, j, false);
      delay(100);
    }
  }
}
// Configuración de puertos
void setup() {
  // Configuramos el puerto serial 
  Serial.begin(115200);
  // Verifica que el puerto serial esté disponible
//  while(!Serial){
//    ;
//  }
  // Matriz LED 8x8
  lc.shutdown(0, false);
  lc.setIntensity(0, 1);
  lc.clearDisplay(0);
  // Servomotores para las articulaciones 
  cabeza.attach(CABEZA_PIN);
  brazoIzq.attach(BRAZO_IZQ_PIN);
  brazoDer.attach(BRAZO_DER_PIN);
  // Inicia los brazos en 180°
  cabeza.write(90);
  delay(DEL_SERVO);
  brazoIzq.write(90);
  delay(DEL_SERVO);
  brazoDer.write(90);
  delay(DEL_SERVO);
  generarTension();
  Serial.println(" Escriba una oracion: ");
}

void loop() {
  if (Serial.available()){
    // Lee el mensaje enviado por el módulo WiFi
    mensaje = Serial.readStringUntil('\n');
    // Convierte la cadena a lowercase
    mensaje.toLowerCase();
    Serial.println(mensaje);
    if (mostrarEmocion(mensaje)){
    } else {
      showInexpresivo();
    }
  } else {
    showInexpresivo();
  }
}

/**
 * Método de búsqueda de palabras clave
 * retorna verdadero si es encontrada la palabra
 */
bool mostrarEmocion(String mensaje){
  // Bandera de encontrado
  bool encontrado = false;
  // Verifica las coincidencias
  for(int i=0; i<14; i++){
    if(mensaje.indexOf(keyWord[i]) >= 0){
      // Alegre
      if (keyWord[i] == "feliz" || keyWord[i] == "good" || keyWord[i] == "bien" || 
          keyWord[i] == "alegre" || keyWord[i] == "genial" || keyWord[i] == "contento")
      {
        showAlegre();
        break;
        encontrado = true;
      }
      // Triste
      if (keyWord[i] == "triste" || keyWord[i] == "sad"){
        showTriste();
        break;
        encontrado = true;
      }
      // Enojado/Molesto
      if (keyWord[i] == "enojado" || keyWord[i] == "mad" || keyWord[i] == "bravo" || keyWord[i] == "angry"){
        showEnojado();
        break;
        encontrado = true;
      }
      // Inexpresivo
      if (keyWord[i] == "nada" || mensaje == ""){
        showInexpresivo();
        break;
        encontrado = true;
      }
    }
  }
  return encontrado;
}
// Dibuja carita alegre
void showAlegre(){
  for (byte i=0; i<8; i++){
    lc.setRow(0, i, alegre[i]);
  }
  delay(delMatrix);
  cabeza.write(0);
  delay(DEL_SERVO);
  brazoIzq.write(180);
  delay(DEL_SERVO);
  brazoDer.write(180);
  delay(DEL_SERVO);
}
// Dibuja carita triste
void showTriste(){
  for (byte i=0; i<8; i++){
    lc.setRow(0, i, triste[i]);
  }
  delay(delMatrix);
  cabeza.write(180);
  delay(DEL_SERVO);
  brazoIzq.write(90);
  delay(DEL_SERVO);
  brazoDer.write(0);
  delay(DEL_SERVO);
}
// Dibuja carita enojada
void showEnojado(){
  for (byte i=0; i<8; i++){
    lc.setRow(0, i, enojado[i]);
  }
  delay(delMatrix);
  cabeza.write(180);
  delay(DEL_SERVO);
  cabeza.write(0);
  delay(DEL_SERVO);
  brazoIzq.write(90);
  delay(DEL_SERVO);
  brazoDer.write(0);
  delay(DEL_SERVO);
  brazoIzq.write(0);
  delay(DEL_SERVO);
  brazoDer.write(0);
  delay(DEL_SERVO);
}
// Dibuja carita enojada
void showInexpresivo(){
  for (byte i=0; i<8; i++){
    lc.setRow(0, i, inexpresivo[i]);
  }
  delay(delMatrix);
  cabeza.write(90);
  delay(DEL_SERVO);
  brazoIzq.write(90);
  delay(DEL_SERVO);
  brazoDer.write(0);
  delay(DEL_SERVO);  
}
