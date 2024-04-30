#include<SoftwareSerial.h>

SoftwareSerial mySerial(0, 1);

#define LED_PIN 13

// Define motor driver pins
#define IN1 10
#define IN2 9
#define IN3 8
#define IN4 7

#define TRIG_PIN_f 2
#define ECHO_PIN_f 3

#define TRIG_PIN_l 11
#define ECHO_PIN_l 12

#define TRIG_PIN_r 4
#define ECHO_PIN_r 5

int carDirection=0;
bool isAuto=false;

void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);

  // Set motor driver pins as OUTPUT
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(A0,INPUT);

  // Set ultrasonic sensor pins
   pinMode(TRIG_PIN_f, OUTPUT);
  pinMode(ECHO_PIN_f, INPUT);

  pinMode(TRIG_PIN_l, OUTPUT);
  pinMode(ECHO_PIN_l, INPUT);

  pinMode(TRIG_PIN_r, OUTPUT);
  pinMode(ECHO_PIN_r, INPUT);

  delay(2000);                
}

void loop() {  
  helper();
}



// Define motor control functions
void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void moveReverse() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveRight() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void moveLeft() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}


void automaticMode(){

  long durationf;
  long durationl;
  long durationr;

  int distancef;
  int distancel;
  int distancer;

  // Trigger front sensor
  digitalWrite(TRIG_PIN_f, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN_f, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_f, LOW);
  durationf = pulseIn(ECHO_PIN_f, HIGH);
  distancef = durationf * 0.034 / 2;  // Speed of sound is 0.034 cm per microsecond

  // Trigger left sensor
  digitalWrite(TRIG_PIN_l, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN_l, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_l, LOW);
  durationl = pulseIn(ECHO_PIN_l, HIGH);
  distancel = durationl * 0.034 / 2;  // Speed of sound is 0.034 cm per microsecond

  // Trigger right sensor
  digitalWrite(TRIG_PIN_r, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN_r, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_r, LOW);
  durationr = pulseIn(ECHO_PIN_r, HIGH);
  distancer = durationr * 0.034 / 2;  // Speed of sound is 0.034 cm per microsecond


  if (distancef <= 15) {
    if (distancer > distancel) {
      if ((distancer <= 15) && (distancel <= 15)) {
        stopMotors();
        delay(200);
        moveReverse();
        delay(2000);

      } else {
        // moveReverse();
        // delay(500);
        moveRight();
        delay(1000);

      }
    } else if (distancer < distancel) {
      if ((distancer <= 15) && (distancel <= 15)) {
        stopMotors();
        delay(200);
        moveReverse();
        delay(2000);

      } else {
        // moveReverse();
        // delay(500);
        moveLeft();
        delay(1000);

      }
    }
    }
  
    else if (distancer <= 15) {
      moveLeft();
      delay(500);

    } else if (distancel <= 15) {
      moveRight();
      delay(500);

      
    } else {
      moveForward();

    }
}

//Function to read the values from app
void helper() {
  char data;
  if (mySerial.available()) {
    data = mySerial.read();
    switch (data) {
      case '1':   // Forward
        handleDirectionCommand(1);
        break;

      case '2':   // Reverse
        handleDirectionCommand(2);
        break;

      case '3':   // Right
        handleDirectionCommand(3);
        break;

      case '4':   // Left
        handleDirectionCommand(4);
        break;
      
      case '5':
        isAuto=true;
        break;
      
      case '6':
        stopMotors();  
        isAuto=false;
        break;

      default:
        // Handle invalid command
        stopMotors();
        carDirection = 0;
        break;
    }
  }

  if(isAuto){
    automaticMode();
  }
}

//Manual Mode
void handleDirectionCommand(int direction) {
  if (carDirection == direction) {
    stopMotors();
    carDirection = 0;
  } else {
    switch (direction) {
      case 1:   // Forward
        moveForward();
        break;

      case 2:   // Reverse
        moveReverse();
        break;

      case 3:   // Right
        moveRight();
        break;

      case 4:   // Left
        moveLeft();
        break;
    }
    carDirection = direction;
  }
}

