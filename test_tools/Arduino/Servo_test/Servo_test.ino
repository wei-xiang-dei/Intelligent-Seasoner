
#include <Servo.h>
const int servo_pin = 10;
Servo myservo;
int val;
float num;
void setup() {
  // put your setup codehere, to run once:
  Serial.begin(115200);

  myservo.attach(servo_pin);
  myservo.write(160);
//  delay(2000);
}

void loop() {
    for(int i=0;i<180;i++){
      myservo.write(i);
      delay(10);
    }
    for(int i=0;i<180;i++){
      myservo.write(180-i);
      delay(10);
    }
//    myservo.write(160);
//  for(int i=0;i<80;i++){
//    myservo.write(80+i);
//    delay(10);
//  }
//  for(int i=0;i<80;i++){
//    myservo.write(160-i);
//    delay(10);
//  }
//delay(2000);
//myservo.write(160);
//delay(2000);
//myservo.write(130);
//delay(2000);
//myservo.write(110);
//delay(2000);
//myservo.write(90);
//delay(2000);
//myservo.write(60);
//delay(2000);
//myservo.write(0);
}
