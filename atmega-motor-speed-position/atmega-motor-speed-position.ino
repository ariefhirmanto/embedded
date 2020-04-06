/*
 * mengukur posisi dan kecepatan sesaat
 * 
  https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
library timer:
   * - https://github.com/PaulStoffregen/TimerOne
  interkoneksi:
  hijau -> D2
  putih -> D3
  hitam -> GND
  merah -> +5 volt , namun terukur 4 volt

  
*/
#include <TimerOne.h>   //

#define ENCODER_A 2 // hijau
#define ENCODER_B 3 // putih

int  waktu = 0;
int  posisi = 0;
int counter_kecepatan=0;
bool pin_A_prev; // status pin 2 sebelumnya
bool pin_B_prev; // status pin 3 sebelumnya

volatile int counter = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  pinMode(ENCODER_A, INPUT_PULLUP);
  pinMode(ENCODER_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), ISR_pin_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), ISR_pin_B, CHANGE);
  Timer1.initialize( 10000 );           //period in microseconds 
  Timer1.attachInterrupt( HitungKecepatan );   //attach the Timer1 interrupt 
}

void HitungKecepatan(){
  Serial.print(waktu);
  Serial.print(",");
  Serial.print(posisi);
  Serial.print(",");
  Serial.println(counter_kecepatan);
  counter_kecepatan=0;
  waktu++;
}

char DecodeRotary(bool   pin_A, bool  pin_A_prev, bool  pin_B, bool  pin_B_prev) {
  if (pin_A == 0 && pin_A_prev == 0 && pin_B == 0 && pin_B_prev == 1) {
    return -1;
  }
  if (pin_A == 0 && pin_A_prev == 0 && pin_B == 1 && pin_B_prev == 0) {
    return 1;
  }
  if (pin_A == 0 && pin_A_prev == 1 && pin_B == 0 && pin_B_prev == 0) {
    return 1;
  }
  if (pin_A == 0 && pin_A_prev == 1 && pin_B == 1 && pin_B_prev == 1) {
    return -1;
  }

  if (pin_A == 1 && pin_A_prev == 0 && pin_B == 0 && pin_B_prev == 0) {
    return -1;
  }
  if (pin_A == 1 && pin_A_prev == 0 && pin_B == 1 && pin_B_prev == 1) {
    return 1;
  }
  if (pin_A == 1 && pin_A_prev == 1 && pin_B == 0 && pin_B_prev == 1) {
    return 1;
  }
  if (pin_A == 1 && pin_A_prev == 1 && pin_B == 1 && pin_B_prev == 0) {
    return -1;
  }
  return 0;
}
void ISR_pin_A() {
  int delta;
  bool  pin_A, pin_B;
  pin_A = digitalRead(ENCODER_A);
  pin_B = digitalRead(ENCODER_B);

  delta = DecodeRotary(pin_A, pin_A_prev, pin_B, pin_B_prev);
  posisi += delta;
  counter_kecepatan+=delta;

  pin_A_prev = pin_A;
  pin_B_prev = pin_B;
}

void ISR_pin_B() {
  int delta;
  bool  pin_A, pin_B;
  pin_A = digitalRead(ENCODER_A);
  pin_B = digitalRead(ENCODER_B);

  delta = DecodeRotary(pin_A, pin_A_prev, pin_B, pin_B_prev);
  posisi += delta;
  counter_kecepatan+=delta;

  pin_A_prev =  pin_A;
  pin_B_prev = pin_B;
}

// the loop function runs over and over again forever
void loop() {
// lampu kedip hanya untuk menunjukkan sistem tidak hang
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(500);                       // wait for a second
}
