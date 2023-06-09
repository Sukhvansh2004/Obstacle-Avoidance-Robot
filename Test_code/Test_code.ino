#include <NewPing.h>  // Library for ultrasonic sensors

#define VA 220
#define VB 255
// Pin definitions
#define TRIGGER_PIN_FRONT 13  // Trigger pin for front sensor
#define ECHO_PIN_FRONT 2     // Echo pin for front sensor
#define TRIGGER_PIN_LEFT 13   // Trigger pin for left sensor
#define ECHO_PIN_LEFT 7      // Echo pin for left sensor
#define TRIGGER_PIN_RIGHT 13  // Trigger pin for right sensor
#define ECHO_PIN_RIGHT 12     // Echo pin for right sensor
#define ENA 11         // Motor A enable pin
#define IN_A_1 10        // Motor A input 1
#define IN_A_2 9        // Motor A input 2
#define ENB 3         // Motor B enable pin
#define IN_B_1 5         // Motor B input 3
#define IN_B_2 6         // Motor B input 4

// Robot dimensions
#define ROBOT_WIDTH 22.5       // Width of the robot in cm
#define ROBOT_LENGTH 30    // Length of the robot in cm

// Ultrasonic sensor objects
NewPing frontSensor(TRIGGER_PIN_FRONT, ECHO_PIN_FRONT, 500);  // Front sensor
NewPing leftSensor(TRIGGER_PIN_LEFT, ECHO_PIN_LEFT, 500);      // Left sensor
NewPing rightSensor(TRIGGER_PIN_RIGHT, ECHO_PIN_RIGHT, 500);   // Right sensor

float linearVelocity = 28.99 * 0.001;   // Linear velocity in cm/ms
float angularVelocity = 105.07 * 0.001; // Angular velocity in degrees/ms

// Define the coordinates of point A and point B
float pointA_x = 0.0;  // x-coordinate of point A
float pointA_y = 1.0;  // y-coordinate of point A
float pointB_x = 0.0;  // x-coordinate of point B
float pointB_y = 200.0;  // y-coordinate of point B


void backward(float distance=0) {
  analogWrite(ENA, VA);
  analogWrite(ENB, VB);

  digitalWrite(IN_A_1, HIGH);
  digitalWrite(IN_A_2, LOW);

  digitalWrite(IN_B_1, HIGH);
  digitalWrite(IN_B_2, LOW);

  if(distance!=0){
    delay(distance/linearVelocity);
  }
}

void forward(float distance = 0) {
  analogWrite(ENA, VA);
  analogWrite(ENB, VB);

  digitalWrite(IN_A_1, LOW);
  digitalWrite(IN_A_2, HIGH);

  digitalWrite(IN_B_1, LOW);
  digitalWrite(IN_B_2, HIGH);

  if (distance != 0) {
    delay(distance / linearVelocity);
  }
}

void right(float angle = 0) {
  analogWrite(ENA, VA);
  analogWrite(ENB, VB);

  digitalWrite(IN_A_1, HIGH);
  digitalWrite(IN_A_2, LOW);

  digitalWrite(IN_B_1, LOW);
  digitalWrite(IN_B_2, HIGH);

  if (angle != 0) {
    delay(angle / angularVelocity);
  }
}

void left(float angle = 0) {
  analogWrite(ENA, VA);
  analogWrite(ENB, VB);

  digitalWrite(IN_A_1, LOW);
  digitalWrite(IN_A_2, HIGH);

  digitalWrite(IN_B_1, HIGH);
  digitalWrite(IN_B_2, LOW);

  if (angle != 0) {
    delay(angle / angularVelocity);
  }
}

void stationary() {
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
}

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(IN_A_1, OUTPUT);
  pinMode(IN_A_2, OUTPUT);

  pinMode(IN_B_1, OUTPUT);
  pinMode(IN_B_2, OUTPUT);
}
float currentHeading = atan2(pointA_y, pointA_x);

float calculateDistance(float x1, float y1, float x2, float y2) {
  return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}

float calculateDesiredHeading(float x1, float y1, float x2, float y2) {
  return atan2(y2 - y1, x2 - x1);
}

void navigateToDestination(float destinationX, float destinationY) {
  // Calculate the distance to the destination
  float distanceToDestination = calculateDistance(pointA_x, pointA_y, destinationX, destinationY);
  if(distanceToDestination<=5){
    stationary();
    return;
  }
    // Move forward towards the destination
    forward(15);
    pointA_x += 15*cos(currentHeading);
    pointA_y += 15*sin(currentHeading);

  // Calculate the desired heading to the destination
  float desiredHeading = calculateDesiredHeading(pointA_x, pointA_y, destinationX, destinationY);

  // Calculate the angle difference between the current heading and the desired heading
  float angleDifference = desiredHeading - currentHeading;

  // Normalize the angle difference to be within the range of -pi to pi
  if (angleDifference > PI) {
    angleDifference -= 2 * PI;
  } else if (angleDifference < -PI) {
    angleDifference += 2 * PI;
  }

  // Adjust the robot's heading based on the angle difference
  if (abs(angleDifference) > 0.5) {
    // If the angle difference is significant, perform a rotation
    if (angleDifference > 0) {
      // Rotate clockwise
      left(abs(angleDifference));
      currentHeading += abs(angleDifference);
    } else {
      // Rotate counterclockwise
      right(abs(angleDifference));
      currentHeading -= abs(angleDifference);
    }
  } 
}

void loop() {
  // Read distance from front sensor
  int frontDistance = frontSensor.ping_cm();

  // Read distances from left and right sensors
  int leftDistance = leftSensor.ping_cm();
  int rightDistance = rightSensor.ping_cm();

  // Print sensor readings
  Serial.print("Front: ");
  Serial.print(frontDistance);
  Serial.print(" cm, Left: ");
  Serial.print(leftDistance);
  Serial.print(" cm, Right: ");
  Serial.print(rightDistance);
  Serial.println(" cm");

  forward();
  delay(10);
}
