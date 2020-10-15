//import libraries
#include <Servo.h>
#include <LiquidCrystal.h>

//defines pin numbers
int redPin= 11;
int greenPin = 10;
int bluePin = 9;
int buzzer = 4;
int mSensor = 2;
int pirStat = 0;
const int trigPin = 8;
const int echoPin = 7;

//defines variables
long duration;
int distance;

Servo radarServo;  //create servo object to control a servo
int pos = 0;    //variable to store the servo position
LiquidCrystal lcd(A5, A4, A3, A2, A1, A0); //initialise lcd pins

void setup() {

 pinMode(redPin, OUTPUT);
 pinMode(greenPin, OUTPUT);
 pinMode(bluePin, OUTPUT);
 pinMode(mSensor, INPUT);
 pinMode(buzzer, OUTPUT);
 pinMode(trigPin, OUTPUT); 
 pinMode(echoPin, INPUT);
 Serial.begin(9600); //start serial communication
 radarServo.attach(3);  //attaches the servo on pin 3 to the servo object
 lcd.begin(16,2); //define lcd layout
 
}

void loop() {
  
  noTone(buzzer);
  
  pirStat = digitalRead(mSensor); 
  if (pirStat == HIGH) { //if motion detected
    
    //clear trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    //set trigPin HIGH for 10ms
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    for (pos = 0; pos <= 180; pos++) { //moves from 0 degrees to 180 degrees
      logic(); //executes function
    }
  
    for (pos = 180; pos >= 0; pos--) { //moves from 180 degrees to 0 degrees
      logic(); //executes function
    } 
  } else {
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, LOW);
    lcd.clear();
    lcd.print("No Motion");
    delay(250);
  }
 
}

int logic(){
  // in steps of 1 degree
  radarServo.write(pos); // tell servo to go to position in variable 'pos'
  delay(10); // waits 10ms for the servo to reach the position
  distance = distCalc(); //assigns function value to variable
  Serial.print(pos);
  Serial.print(", ");
  Serial.print("Distance: ");
  Serial.println(distance); 
      
  if (distance >= 1 && distance <= 15) {
    //led turns red
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, LOW);
    //lcd output
    lcd.setCursor(0,0);
    lcd.print("Too Close!!    ");
    lcdDistance(); //calls function to print distance to lcd
    //buzzer outputs 1200khz frequency 
    tone(buzzer, 1200);
  }
  else if (distance >= 16 && distance <= 30) {
    //led turns yellow
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, HIGH);
    digitalWrite(bluePin, LOW);
    lcd.setCursor(0,0);
    lcd.print("Getting Closer ");
    lcdDistance(); //calls function to print distance to lcd
    tone(buzzer, 700);
  }
  else if (distance >= 31 && distance <= 45) {
    //led turns green
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, HIGH);
    digitalWrite(bluePin, LOW);
    lcd.setCursor(0,0);
    lcd.print("Safe Distance  ");
    lcdDistance(); //calls function to print distance to lcd
    tone(buzzer, 300);
  }
  else {
    //led white
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, HIGH);
    digitalWrite(bluePin, HIGH);
    //lcd print
    lcd.setCursor(0,0);
    lcd.print("No Threat       ");
    lcdDistance(); //calls function to print distance to lcd
    noTone(buzzer); //buzzer stops
  }
}

//print object distance to lcd
int lcdDistance(){
  lcd.setCursor(0,1); //starts on 2nd line
  lcd.print("Distance: ");
  lcd.print(distance); //prints function result
  lcd.print("    ");
}

//calculate object distance from radar
int distCalc(){
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(10);
  //Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  //read echoPin, return sound wave travel time in ms
  duration = pulseIn(echoPin, HIGH);
  //calcualte distance = time * speed of sound /2
  distance = duration*0.034/2;
  return distance; //returns end value as integer
}
