/*
   Codigo para manejar el motor de CC de la bandeja de lectora de CD
   en ambos sentidos con el SN754410 hasta 1 Amperio,
   Taller reciclaje electronico makespace madrid
*/

int moviendo = LOW; // empiezo parado
int abriendo = LOW; // estoy cerrando si no estoy parado
// Salidas al puente en H: SN754410
int pin_veloc = 3;  // pin del control velocidad. Enable 1,2. Pin1 del SN754410
int pin_mot1A = 6;  // pin para el sentido. Conectado motor
int pin_mot2A = 7;  // pin para el sentido, contrario al 1A
// Entradas de los finales de carrera y pulsador
int pin_close = 11; // final carrera cerrado
int pin_open = 10; // final carrera abierto
int pin_pulsa = 12; // pulsador de abrir

void setup() {
  pinMode (pin_mot1A,OUTPUT);
  pinMode (pin_mot2A,OUTPUT);
  pinMode (pin_veloc,OUTPUT);
  pinMode (pin_close,INPUT);
  pinMode (pin_open,INPUT);
  pinMode (pin_pulsa,INPUT);
  pinMode (LED_BUILTIN, OUTPUT); //para comprobar
}

void loop() {
  // Negamos las entradas porque son activas a nivel bajo
  int valor_pulsa = !digitalRead (pin_pulsa);
  int valor_open  = !digitalRead (pin_open);  //lectura del final de carrera: bandeja abierta
  int valor_close = !digitalRead (pin_close); //lectura del final de carrera: bandeja cerrada
  
  if (valor_close == HIGH) { // bandeja cerrada
    if (valor_pulsa == HIGH) { //se ha pulsado pulsador
      moviendo = HIGH; // empiezo el movimiento
      abriendo = HIGH; // abriendo
    } else {
      moviendo = LOW;  // bandeja quieta
    }
  } else {
    if (valor_open == HIGH) {    // esta abierto del todo
      if (valor_pulsa == HIGH) { // han pulsado, voy a cerrar
        moviendo = HIGH; // empiezo el movimiento
        abriendo = LOW;  // cerrando
      } else {
        moviendo = LOW;
      }
    } else {  // no estamos ni en el final ni en el principio
      // Esto es que la bandeja no se esta moviendo, pero no esta en ninguno de los
      // extremos. Cuando sucede esto es que la han empujado, estando en el final
      // por lo tanto: cerramos
      if (moviendo == LOW) {
        moviendo = HIGH;
        abriendo = LOW; // cerrando
      }
    }
  }

  if (moviendo == LOW) { // si no estoy moviendo
    digitalWrite (pin_veloc, LOW);
  } else { // estamos moviendonos
    if ( abriendo == HIGH) {
      digitalWrite(pin_mot1A, HIGH); // sentido de abrir
      digitalWrite(pin_mot2A, LOW);
    } else {
      digitalWrite(pin_mot2A, HIGH); // sentido de cerrar
      digitalWrite(pin_mot1A, LOW);
    }
    analogWrite (pin_veloc, 100); // PWM porcentaje sobre 255
    //digitalWrite(pin_veloc,HIGH);
  }
  digitalWrite(LED_BUILTIN,moviendo); // si se mueve, encendemos el LED para depurar
}
