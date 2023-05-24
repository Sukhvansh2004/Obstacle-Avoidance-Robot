#define sensor_l_echo 12
#define sensor_trig 13
#define sensor_f_echo 2
#define sensor_r_echo 4

#define ENA 11
#define ENB 6

#define IN_A_1 10
#define IN_A_2 9

#define IN_B_1 8 
#define IN_B_2 7


int rd=15;
int ld=15;
int fd=10;
int dt=30;

int offset_x=0;
int offset_y=100;


int reading(trigPin,echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);

  int distance_cm = duration * 0.034 / 2;

  return distance_cm;  
}

void backward(){
  analogWrite(ENA,242);
  analogWrite(ENB,255);
  
  digitalWrite(IN_A_1,HIGH);
  digitalWrite(IN_A_2,LOW);

  digitalWrite(IN_B_1,HIGH);
  digitalWrite(IN_B_2,LOW);

}

void forward(){
  analogWrite(ENA,242);
  analogWrite(ENB,255);
  
  digitalWrite(IN_A_1,LOW);
  digitalWrite(IN_A_2,HIGH);

  digitalWrite(IN_B_1,LOW);
  digitalWrite(IN_B_2,HIGH);

}

void right(){
  analogWrite(ENA,242);
  analogWrite(ENB,255);
  
  digitalWrite(IN_A_1,HIGH);
  digitalWrite(IN_A_2,LOW);

  digitalWrite(IN_B_1,LOW);
  digitalWrite(IN_B_2,HIGH);
}

void left(){
  analogWrite(ENA,242);
  analogWrite(ENB,255);
  
  digitalWrite(IN_A_1,LOW);
  digitalWrite(IN_A_2,HIGH);

  digitalWrite(IN_B_1,HIGH);
  digitalWrite(IN_B_2,LOW);
}

void stationary(){
  
  digitalWrite(IN_A_1,LOW);
  digitalWrite(IN_A_2,LOW);

  digitalWrite(IN_B_1,LOW);
  digitalWrite(IN_B_2,LOW);
}
void setup() {

  Serial.begin(9600);
  pinMode(sensor_l_echo,INPUT);
  pinMode(sensor_r_echo,INPUT);
  pinMode(sensor_f_echo,INPUT);
  pinMode(sensor_trig,OUTPUT);
  
  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);

  pinMode(IN_A_1,OUTPUT);
  pinMode(IN_A_2,OUTPUT);

  pinMode(IN_B_1,OUTPUT);
  pinMode(IN_B_2,OUTPUT);
  
}

int p=0;
void loop() {
  int df=reading(sensor_trig,sensor_f_echo);
  int dr=reading(sensor_trig,sensor_r_echo);
  int dl=reading(sensor_trig,sensor_l_echo);
  Serial.print(df);
  Serial.print(" ");
  Serial.print(dl);
  Serial.print(" ");
  Serial.print(dr);
  Serial.println(" ");

  if(df<fd && p++>10){
    if(dl<dr){
      backward();
      delay(300);
      right();
      delay(856/2);
      
    }
    else{
      backward();
      delay(300);
      left();
      delay(856/2);
    }
    stationary();
  }
  else if(dr<rd){
    left();
    delay(dt);
    p=0;
  }
  else if(dl<ld){
    right();
    delay(dt);
    p=0;
  }
  else{
    forward();
    delay(dt);
  }
}