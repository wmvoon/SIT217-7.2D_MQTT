#include <CuteBuzzerSounds.h>

//For serial receive.
const byte numChars = 30;
char receivedChars[numChars]; // an array to store the received data
String received; //The data as a string
boolean newData = false;

//Setting the pins
const int LED = 12;
int buzzer = 5;

//Initialize Motor Code
// Motor One
int ENA = 6;
int IN1 = 7;
int IN2 = 8;
// Motor Two
int ENB = 11;
int IN3 = 9;
int IN4 = 10;

void setup() {
  Serial.begin(9600); // Starts the serial communication
  
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(LED, OUTPUT);

  digitalWrite(ENA, 255);
  digitalWrite(ENB, 255);

  cute.init(buzzer);
  cute.play(S_CONNECTION);
}

void loop() {

  recvWithEndMarker();

  //read intructions from subscribers to the Arduino 
  if (newData == true)
  {
    String instruction = received.substring(0, 100);
    Serial.println(instruction);
    instruction.toUpperCase();

    if (instruction == "ON LIGHT")
    {
      torch();
    }
    else if (instruction == "OFF LIGHT")
    {
      torchOff();
    }
    else if (instruction == "FORWARD")
    {
      goStraight();
      delay(1000);
      stopMoving();
    }
    else if (instruction == "REVERSE")
    {
      reverse();
      delay(1000);
      stopMoving();
    }
    else if (instruction == "TURN LEFT")
    {
      turnLeft();
      delay(1000);
      stopMoving();
    }
    else if (instruction == "TURN RIGHT")
    {
      turnRight();
      delay(1000);
      stopMoving();
    }
    else if (instruction == "ON ALARM")
    {
      alarm();
    }
    else if (instruction == "OFF ALARM")
    {
      alarmOff();
    }
    else if (instruction == "SOUND1")
    {
      cute.play(S_HAPPY);
    }
    else if (instruction == "SOUND2")
    {
      cute.play(S_CUDDLY);
    }
    else if (instruction == "SOUND3")
    {
      cute.play(S_SUPER_HAPPY);
    }
    else if (instruction == "SOUND4")
    {
      cute.play(S_SAD);
    }
    else if (instruction == "SOUND5")
    {
      cute.play(S_SLEEPING);
    }
    newData = false;
  }
}

void recvWithEndMarker()
{
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;

  while (Serial.available() > 0 && newData == false)
  {
    rc = Serial.read();

    if (rc != endMarker)
    {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars)
      {
        ndx = numChars - 1;
      }
    }
    else
    {
      receivedChars[ndx] = '\0'; // terminate the string
      received = String(receivedChars);
      ndx = 0;
      newData = true;
    }
  }
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void stopMoving() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void goStraight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void reverse() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void alarm() {
  tone(buzzer, 1000); // Send 1KHz sound signal...
  delay(500);        // ...for 0.5 sec
  tone(buzzer, 750); // Send 0.5KHz sound signal...
  delay(500);        // ...for 0.5 sec
}
void alarmOff() {
  noTone(buzzer);
}

void torch() {
  digitalWrite(LED, HIGH);
}

void torchOff() {
  digitalWrite(LED, LOW);
}
