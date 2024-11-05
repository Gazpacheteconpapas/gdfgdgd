#include <ESP32Servo.h>

// Declaramos dos objetos servo
Servo servo1;
Servo servo2;

// Definimos los pines para los servos
int servo1Pin = 18;  // Pin del ESP32 para el primer servo
int servo2Pin = 19;  // Pin del ESP32 para el segundo servo

void setup() {
  // Adjuntamos los servos a los pines correspondientes
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);

  Serial.begin(115200);
  Serial.println("Iniciando control de servos...");
}

void loop() {
  if (Serial.available() > 0) {
    int angle1 = Serial.parseInt();  // Lee el ángulo para el servo1
    int angle2 = Serial.parseInt();  // Lee el ángulo para el servo2

    servo1.write(constrain(angle1, 0, 180));  // Constrain mantiene el valor dentro de 0-180
    servo2.write(constrain(angle2, 0, 180));

    Serial.print("Servo1: ");
    Serial.print(angle1);
    Serial.print(" Servo2: ");
    Serial.println(angle2);
  }
}
