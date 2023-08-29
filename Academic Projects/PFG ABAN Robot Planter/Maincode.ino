#include <Servo.h>
#include <SoftwareSerial.h>

#define trigPin 7
#define echoPin 6
#define ledB 10

// Create servo object
Servo s24;
Servo s23;
Servo s22;

Servo s21;
Servo s20;
Servo s19;

Servo s18;
Servo s17;
Servo s16;

Servo s7;
Servo s8;
Servo s9;

Servo s10;
Servo s11;
Servo s12;

Servo s1;
Servo s2;
Servo s3;

Servo s15;
Servo s14;
Servo s13;

Servo s5;

int i0H1 = 0;
int i1H1 = 0;
int i2H1 = 0;
int i3H1 = 0;
int i4H1 = 0;
int i5H1 = 0;
int i6H1 = 0;
int i7H1 = 0;
int i8H1 = 0;

int i0T1 = 0;
int i1T1 = 0;
int i2T1 = 0;
int i3T1 = 0;
int i4T1 = 0;
int i5T1 = 0;
int i6T1 = 0;

int i1L1 = 0;
int i2L1 = 0;
int i3L1 = 0;
int i4L1 = 0;
int i5L1 = 0;
int i6L1 = 0;

int i1L2 = 0;
int i2L2 = 0;
int i3L2 = 0;
int i4L2 = 0;
int i5L2 = 0;
int i6L2 = 0;
boolean l1status = LOW;
boolean l2status = LOW;
boolean aStatus = LOW;
boolean attStatus = LOW;
int k = 0;
int a = 0;
int aa = 0;
int period = 1000;
unsigned long time_now = 0;

float distance;
long duration;
int att = 0;
int speedV = 30;
bool ldgu_flag[2], turn_flag, step_flag; //defining flags
int deg;

//FUNCTIONS
void CHC();
void R2B();
void Turn(int);
void Step();
void Lay_Down();
void Get_Up();

void setup() {
  Serial.begin(38400);
  delay(20);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(ledB, OUTPUT);
  // Leg 4
  s10.attach(31, 600, 2400);
  s11.attach(32, 600, 2400);
  s12.attach(33, 600, 2400); //rot
  // Leg 5
  s7.attach(28, 600, 2400);
  s8.attach(29, 600, 2400);
  s9.attach(30, 600, 2400); //rot
  // Leg 6
  s1.attach(22, 600, 2400);
  s2.attach(23, 600, 2400);
  s3.attach(24, 600, 2400); //rot
  // Leg 1
  s18.attach(39, 600, 2400);
  s17.attach(38, 600, 2400);
  s16.attach(37, 600, 2400); //rot
  // Leg 2
  s21.attach(42, 600, 2400);
  s20.attach(41, 600, 2400);
  s19.attach(40, 600, 2400); //rot
  // Leg 3
  s24.attach(45, 600, 2400);
  s23.attach(44, 600, 2400);
  s22.attach(43, 600, 2400); //rot

  // == Move to initial position
  
  // Leg 4
  s10.write(65);
  s11.write(35);
  s12.write(40);
  // Leg 5
  s7.write(80);
  s8.write(50);
  s9.write(25);
  // Leg 6
  s1.write(90);
  s2.write(45);
  s3.write(60);

  // Leg 1
  s18.write(60);
  s17.write(90);
  s16.write(100);
  // Leg 2
  s21.write(50);
  s20.write(85);
  s19.write(75);
  // Leg 3
  s24.write(50);
  s23.write(80);
  s22.write(80);

  delay(3000);
  //PIN SETUP
  int CHCpin = 2;
  int R2Bpin = 3;
  pinMode(CHCpin,INPUT);
  pinMode(R2Bpin,INPUT);
  //INTERRUPTS
  attachInterrupt(digitalPinToInterrupt(CHCpin), CHC, RISING);
  attachInterrupt(digitalPinToInterrupt(R2Bpin), R2B, RISING);
  Serial.begin(9600);
}

void loop() {
  if (ldgu_flag[0] == true){
    if (ldgu_flag[1] == true){
      Lay_Down();}
    if (ldgu_flag[1] == false){
      Get_Up();}
  }
  if (turn_flag == true){
    Turn(deg);
  }
  if(step_flag == true){
    Step();
  }
}
//declaring functions
//FEATURES
void CHC(){
  int dec, i;
  int sensor_array[6]={0,0,0,0,0,0};
  dec=Serial.read();
  //dec = analogRead();
  
  Serial.print("\n");
  //analog to array
  while(dec > 0){
    sensor_array[i] = dec % 2;
    dec /= 2;
    i++;}
  for (i=0;i<6;i++){
    Serial.print(sensor_array[i]);
    Serial.print(",");}
  Serial.print("\n");
  delay(1000);
  
  //on and contiguous counts
  int on_count=0, contiguous_count=0, contiguous_array[6]={0,0,0,0,0,0};

  for (i=0;i<5;i++){
    if (sensor_array[i]==1 && sensor_array[i+1]==1){
      contiguous_array[i] = 1;
      contiguous_array[i+1] = 1;}
  if (sensor_array[5]==1 && sensor_array[0]==1){
    contiguous_array[0] = 1;
    contiguous_array[5] = 1;}
  }
  for (i=0;i<6;i++){
    on_count += sensor_array[i];
    contiguous_count += contiguous_array[i];
    }
    
  int configuration;
  //Configuration finder
  if (contiguous_count >= 3){
    configuration = 3;}
  else{if (contiguous_count > 1){
      configuration = 2;}
      else{if (on_count > 0){
            configuration = 1;}
            else{
            configuration = 0;}
            }
       }
  
  //Configuration selector
  int order[6]={0,1,5,2,4,3};
  switch(configuration){
    case 0://--------------------SFL 0
      Serial.print("SFL 0\n");
      turn_flag = false;
      step_flag = true;
      ldgu_flag[0] = false;
      break;
    case 1://--------------------SBP 1
      Serial.print("SBP 1\n");
      turn_flag = true;
      step_flag = true;
      ldgu_flag[0] = false;
      for (i=0;i<6;i++){
        int j = order[i];
        if(sensor_array[j] == 1){
          if (j <= 3){
            deg = j*60;
            break;}
          else{
            deg = -(360-(j*60));
            break;
          }
        }
      }
      break;
    case 2://---------------------SBP 2
      Serial.print("SBP 2\n");
      turn_flag = true;
      step_flag = true;
      ldgu_flag[0] = false;
      for (i=0;i<6;i++){
        if(contiguous_array[5]==1){
          contiguous_array[0]=0;
        }
        if(contiguous_array[i] == 1){
          if (i <= 3){
            deg = (i*60)+30;
            break;}
          else{
            deg = 30-(360-(i*60));
            break;
          }
        }
      }
      break;
    case 3://---------------------STA 3
      Serial.print("STA 3\n");
      turn_flag = false;
      step_flag = false;
      ldgu_flag[0] = true;
      ldgu_flag[1] = true;
      break;
  }
}
void R2B() {
  #define IR_PIN A0
  #define SERVO_COUNT 18
  int servoPins[SERVO_COUNT] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, A0, A1, A2, A3, A4, A5};
  Servo servos[SERVO_COUNT];
  int delayTime = 500;
  // Initialize infrared sensor
  pinMode(IR_PIN, INPUT);

  while (true) {
    // Read infrared sensor
    int irValue = analogRead(IR_PIN);

    // Move the hexapod based on the beacon's position
    if (irValue > 100) { // beacon detected
      int angleDiff = (irValue - 100) / 10; // calculate angle difference based on IR value
      
      // Determine direction to move
      int direction = 0;
      if (angleDiff > 0) {
        Turn(-angleDiff); // turn right
      } else if (angleDiff < 0) {
        Turn(-angleDiff); // turn left
      }
      
      Step(); // move forward
      delay(delayTime);
    }
  }
}

//ACTIONS

void Get_Up(){
  #define SERVO_COUNT 18
  int servoPins[SERVO_COUNT] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, A0, A1, A2, A3, A4, A5};
  Servo servos[SERVO_COUNT];
  int delayTime = 500;
  // Define servo angles for getting up
  int servoAnglesGetup[SERVO_COUNT] = {90, 110, 70, 90, 50, 130, 90, 70, 110, 90, 130, 50, 90, 70, 110, 90, 50, 130};
  // Move all servos to the getup position
  for (int i = 0; i < SERVO_COUNT; i++) {
    servos[i].write(servoAnglesGetup[i]);
  }
  // Wait for the movement to complete
  delay(delayTime);
}

void Lay_Down(){
  #define SERVO_COUNT 18
  int servoPins[SERVO_COUNT] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, A0, A1, A2, A3, A4, A5};
  Servo servos[SERVO_COUNT];
  int delayTime = 500;
  // Define servo angles for laying down
  int servoAnglesLaydown[SERVO_COUNT] = {90, 50, 130, 90, 110, 70, 90, 130, 50, 90, 70, 110, 90, 130, 50, 90, 110, 70};
  // Move all servos to the laydown position
  for (int i = 0; i < SERVO_COUNT; i++) {
    servos[i].write(servoAnglesLaydown[i]);
  }
  // Wait for the movement to complete
  delay(delayTime);
}

void Turn(int deg){
   
  if (deg >0) {
    //Move Leg1
  if (i1L1 <= 10) {
    s18.write(60 - i1L1 * 2);
    s17.write(90 - i1L1 * 3);
    i1L1++;
  }
  // Rotate the leg
  if (i2L1 <= 30) {
    s16.write(100 - i2L1);
    i2L1++;

  }
  // Move back to touch the ground
  if (i2L1 > 20 & i3L1 <= 10) {
    s18.write(40 + i3L1 * 2);
    s17.write(60 + i3L1 * 3);
    i3L1++;
  }
  // Stance phase - move leg while touching the ground
  // Rotate back to initial position
  if (i2L1 >= 30) {
    s16.write(70 + i4L1);
    i4L1++;
    l1status = HIGH;
  }
  // Reset the counters for repeating the process
  if (i4L1 >= 30) {
    i1L1 = 0;
    i2L1 = 0;
    i3L1 = 0;
    i4L1 = 0;
    i5L1 = 0;
  }
    //Move Leg3
  if (i1L1 <= 10) {
    s24.write(50 - i1L1 * 2);
    s23.write(80 - i1L1 * 3);
  }
  if (i2L1 <= 30) {
    s22.write(80 - i2L1);

  }
  if (i2L1 > 20 & i3L1 <= 10) {
    s24.write(30 + i3L1 * 2);
    s23.write(50 + i3L1 * 3);
  }
  if (i2L1 >= 30) {
    s22.write(50 + i4L1);
  }
    //Move Leg5 Left
  if (i1L1 <= 10) {
    s7.write(80 + i1L1 * 2);
    s8.write(50 + i1L1 * 3);
  }
  if (i2L1 <= 30) {
    s9.write(60 - i2L1);

  }
  if (i2L1 > 20 & i3L1 <= 10) {
    s7.write(100 - i3L1 * 2);
    s8.write(80 - i3L1 * 3);
  }
  if (i2L1 >= 30) {
    s9.write(30 + i4L1);
  }
    if (l1status == HIGH) {
      //Move Leg2
  if (i1L2 <= 10) {
    s21.write(50 - i1L2 * 2);
    s20.write(80 - i1L2 * 3);
    i1L2++;
  }
  if (i2L2 <= 30) {
    s19.write(75 - i2L2);
    i2L2++;

  }
  if (i2L2 > 20 & i3L2 <= 10) {
    s21.write(30 + i3L2 * 2);
    s20.write(50 + i3L2 * 3);
    i3L2++;
  }
  if (i2L2 >= 30) {
    s19.write(45 + i4L2);
    i4L2++;
  }
  if (i4L2 >= 30) {
    i1L2 = 0;
    i2L2 = 0;
    i3L2 = 0;
    i4L2 = 0;
    i5L2 = 0;
  }
      //Move Leg4 Left
  if (i1L2 <= 10) {
    s10.write(65 + i1L2 * 2);
    s11.write(35 + i1L2 * 3);
  }
  if (i2L2 <= 30) {
    s12.write(60 - i2L2);

  }
  if (i2L2 > 20 & i3L2 <= 10) {
    s10.write(85 - i3L2 * 2);
    s11.write(65 - i3L2 * 3);
  }
  if (i2L2 >= 30) {
    s12.write(30 + i4L2);
  }
      //Move Leg6 Left
  if (i1L2 <= 10) {
    s1.write(90 + i1L2 * 2);
    s2.write(45 + i1L2 * 3);
  }
  if (i2L2 <= 30) {
    s3.write(90 - i2L2);
  }
  if (i2L2 > 20 & i3L2 <= 10) {
    s1.write(110 - i3L2 * 2);
    s2.write(75 - i3L2 * 3);
  }
  if (i2L2 >= 30) {
    s3.write(60 + i4L2);
  }
    }
  }
  else {
    //Move Leg1 Right
  if (i1L1 <= 10) {
    s18.write(60 - i1L1 * 2);
    s17.write(90 - i1L1 * 3);
    i1L1++;
  }
  if (i2L1 <= 30) {
    s16.write(70 + i2L1);
    i2L1++;

  }
  if (i2L1 > 20 & i3L1 <= 10) {
    s18.write(40 + i3L1 * 2);
    s17.write(60 + i3L1 * 3);
    i3L1++;
  }
  if (i2L1 >= 30) {
    s16.write(100 - i4L1);
    i4L1++;
    l1status = HIGH;
  }
  if (i4L1 >= 30) {
    i1L1 = 0;
    i2L1 = 0;
    i3L1 = 0;
    i4L1 = 0;
    i5L1 = 0;
  }
    //Move Leg3 Right
  if (i1L1 <= 10) {
    s24.write(50 - i1L1 * 2);
    s23.write(80 - i1L1 * 3);
  }
  if (i2L1 <= 30) {
    s22.write(50 + i2L1);

  }
  if (i2L1 > 20 & i3L1 <= 10) {
    s24.write(30 + i3L1 * 2);
    s23.write(50 + i3L1 * 3);
  }
  if (i2L1 >= 30) {
    s22.write(80 - i4L1);
  }
    //Move Leg5
  if (i1L1 <= 10) {
    s7.write(80 + i1L1 * 2);
    s8.write(50 + i1L1 * 3);
  }
  if (i2L1 <= 30) {
    s9.write(30 + i2L1);

  }
  if (i2L1 > 20 & i3L1 <= 10) {
    s7.write(100 - i3L1 * 2);
    s8.write(80 - i3L1 * 3);
  }
  if (i2L1 >= 30) {
    s9.write(60 - i4L1);
  }
    if (l1status == HIGH) {
      //Move Leg2 Right
  if (i1L2 <= 10) {
    s21.write(50 - i1L2 * 2);
    s20.write(80 - i1L2 * 3);
    i1L2++;
  }
  if (i2L2 <= 30) {
    s19.write(45 + i2L2);
    i2L2++;

  }
  if (i2L2 > 20 & i3L2 <= 10) {
    s21.write(30 + i3L2 * 2);
    s20.write(50 + i3L2 * 3);
    i3L2++;
  }
  if (i2L2 >= 30) {
    s19.write(75 - i4L2);
    i4L2++;
  }
  if (i4L2 >= 30) {
    i1L2 = 0;
    i2L2 = 0;
    i3L2 = 0;
    i4L2 = 0;
    i5L2 = 0;
  }
      //Move Leg4
  if (i1L2 <= 10) {
    s10.write(65 + i1L2 * 2);
    s11.write(35 + i1L2 * 3);
  }
  if (i2L2 <= 30) {
    s12.write(35 + i2L2);

  }
  if (i2L2 > 20 & i3L2 <= 10) {
    s10.write(85 - i3L2 * 2);
    s11.write(65 - i3L2 * 3);
  }
  if (i2L2 >= 30) {
    s12.write(65 - i4L2);
  }
      //Move Leg6
  if (i1L2 <= 10) {
    s1.write(90 + i1L2 * 2);
    s2.write(45 + i1L2 * 3);
  }
  if (i2L2 <= 30) {
    s3.write(60 + i2L2);
  }
  if (i2L2 > 20 & i3L2 <= 10) {
    s1.write(110 - i3L2 * 2);
    s2.write(75 - i3L2 * 3);
  }
  if (i2L2 >= 30) {
    s3.write(90 - i4L2);
  }
    }
  }

}

void Step() {
  //---Obstacle avoidance section
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // distance in cm
  return distance;
  //---Movement cycle section
  //Move Leg1
  // Swign phase - move leg though air - from initial to final position
  // Rise the leg
  if (i1L1 <= 10) {
    s18.write(60 - i1L1 * 2);
    s17.write(90 - i1L1 * 3);
    i1L1++;
  }
  // Rotate the leg
  if (i2L1 <= 30) {
    s16.write(100 - i2L1);
    i2L1++;
  }
  // Move back to touch the ground
  if (i2L1 > 20 & i3L1 <= 10) {
    s18.write(40 + i3L1 * 2);
    s17.write(60 + i3L1 * 3);
    i3L1++;
  }
  // Stance phase - move leg while touching the ground
  // Rotate back to initial position
  if (i2L1 >= 30) {
    s16.write(70 + i4L1);
    i4L1++;
    l1status = HIGH;
  }
  // Reset the counters for repeating the process
  if (i4L1 >= 30) {
    i1L1 = 0;
    i2L1 = 0;
    i3L1 = 0;
    i4L1 = 0;
    i5L1 = 0;
  }

    //Move Leg3
  if (i1L1 <= 10) {
    s24.write(50 - i1L1 * 2);
    s23.write(80 - i1L1 * 3);
  }
  if (i2L1 <= 30) {
    s22.write(80 - i2L1);

  }
  if (i2L1 > 20 & i3L1 <= 10) {
    s24.write(30 + i3L1 * 2);
    s23.write(50 + i3L1 * 3);
  }
  if (i2L1 >= 30) {
    s22.write(50 + i4L1);
  }

    //Move Leg5
  if (i1L1 <= 10) {
    s7.write(80 + i1L1 * 2);
    s8.write(50 + i1L1 * 3);
  }
  if (i2L1 <= 30) {
    s9.write(30 + i2L1);

  }
  if (i2L1 > 20 & i3L1 <= 10) {
    s7.write(100 - i3L1 * 2);
    s8.write(80 - i3L1 * 3);
  }
  if (i2L1 >= 30) {
    s9.write(60 - i4L1);
  }
    if (l1status == HIGH) {
      //Move Leg2
  if (i1L2 <= 10) {
    s21.write(50 - i1L2 * 2);
    s20.write(80 - i1L2 * 3);
    i1L2++;
  }
  if (i2L2 <= 30) {
    s19.write(75 - i2L2);
    i2L2++;

  }
  if (i2L2 > 20 & i3L2 <= 10) {
    s21.write(30 + i3L2 * 2);
    s20.write(50 + i3L2 * 3);
    i3L2++;
  }
  if (i2L2 >= 30) {
    s19.write(45 + i4L2);
    i4L2++;
  }
  if (i4L2 >= 30) {
    i1L2 = 0;
    i2L2 = 0;
    i3L2 = 0;
    i4L2 = 0;
    i5L2 = 0;
  }

      //Move Leg4
  if (i1L2 <= 10) {
    s10.write(65 + i1L2 * 2);
    s11.write(35 + i1L2 * 3);
  }
  if (i2L2 <= 30) {
    s12.write(35 + i2L2);

  }
  if (i2L2 > 20 & i3L2 <= 10) {
    s10.write(85 - i3L2 * 2);
    s11.write(65 - i3L2 * 3);
  }
  if (i2L2 >= 30) {
    s12.write(65 - i4L2);
  }
      //Move Leg6
  if (i1L2 <= 10) {
    s1.write(90 + i1L2 * 2);
    s2.write(45 + i1L2 * 3);
  }
  if (i2L2 <= 30) {
    s3.write(60 + i2L2);
  }
  if (i2L2 > 20 & i3L2 <= 10) {
    s1.write(110 - i3L2 * 2);
    s2.write(75 - i3L2 * 3);
  }
  if (i2L2 >= 30) {
    s3.write(90 - i4L2);
  }
    }
}
