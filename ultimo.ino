#include <ESP32Servo.h>
Servo servoo;
const int IN1 = 16;
const int IN2 = 17;
const int ENA = 13;
const int IN3 = 18;
const int IN4 = 19;
const int ENB = 12;
const int pinServo = 27;
int valor = 90;
int valorlim = 90;  // Valor neutral para detener el servo

#include <XboxSeriesXControllerESP32_asukiaaa.hpp>
XboxSeriesXControllerESP32_asukiaaa::Core xboxController;

void setup() {
  Serial.begin(115200);
  Serial.println();
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
  servoo.attach(pinServo);
  xboxController.begin();
  Serial.print("Inicio");
  servoo.write(valor); // Coloca el servo en posición neutral al inicio
}

void loop() {
  xboxController.onLoop();

  if (xboxController.isConnected()) {
    if (xboxController.isWaitingForFirstNotification()) {
      Serial.println("waiting for first notification");
    } else {
      Serial.print(xboxController.xboxNotif.toString());
      unsigned long receivedAt = xboxController.getReceiveNotificationAt();
      uint16_t joystickMax = XboxControllerNotificationParser::maxJoy;

      // Control del servo de rotación continua con el joystick derecho
      if (xboxController.xboxNotif.btnDirUp) {valorlim=valorlim+5;}
      if (xboxController.xboxNotif.btnDirDown) {valorlim=valorlim-5;}
      int joyValue = xboxController.xboxNotif.joyRVert;
      
      // Definimos una zona muerta alrededor del centro del joystick para evitar fluctuaciones
      if (joyValue < 30000) {
        // El joystick está hacia abajo, rotación en un sentido
        //valor = map(joyValue, 0, 30000, 0, 89); // Mapea a valores de 0 a 89 (rotación en una dirección)
        valor=valorlim-15;
      } else if (joyValue > 35535) {
        // El joystick está hacia arriba, rotación en el sentido opuesto
        //valor = map(joyValue, 35535, 65535, 91, 180); // Mapea a valores de 91 a 180 (rotación en dirección opuesta)
        valor=valorlim+15;
      } else {
        // Joystick en posición central, detener el servo
        valor = valorlim;
      }

      servoo.write(valor); // Actualiza el valor del servo de rotación continua
    }
  } else {
    Serial.println("not connected");
    if (xboxController.getCountFailedConnection() > 2) {
      ESP.restart();
    }
  }

  Serial.println("at " + String(millis()));

  // Control del motor DC
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 160 * xboxController.xboxNotif.trigRT / 1023);
    digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 160 * xboxController.xboxNotif.trigLT / 1023);
}
