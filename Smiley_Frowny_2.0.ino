/*
 * Sprite Lights
 * Author: Richard Holgate
 * Created: 2/27/2016
 * Based on Chapter 6: Arduino for Dummies
 * Accelerometer code taken from Waime Wordpress blog @ https://waime.wordpress.com/2015/03/15/basic-measurement-with-accelerometer-gy-61-with-adxl335-chip/
 */
 
// Pin Definitions
int rowPin[] = {2,3,4,5,6,7,8,9};
int blue[]  = {20,21,22,23,24,25,26,27}; //blue  LED pins
int red[]   = {28,29,30,31,32,33,36,37}; //red   LED pins
int green[] = {42,43,44,45,39,38,40,41}; //green LED pins//Order is a little wonky

//other variables
// you can use all the analog inputs to control the GY-61
const int xPin   = A0;
const int yPin   = A1;
const int zPin   = A2;

int x = 0;
int y = 0;
int z = 0;

//arrays used to hold bitmaps
byte smile[] = {
  B00111100,
  B01000010,
  B10100101,
  B10000001,
  B10100101,
  B10011001,
  B01000010,
  B00111100
};

byte unhappy[] = {
  B00111100,
  B01000010,
  B10100101,
  B10000001,
  B10111101,
  B10000001,
  B01000010,
  B00111100
};

byte frown[] = {
  B00111100,
  B01000010,
  B10100101,
  B10000001,
  B10011001,
  B10100101,
  B01000010,
  B00111100
};

void setup()
{
  //LED matrix setup
  for(int i = 0; i < 8; i++){
    //set arduino pins as outputs
    pinMode(rowPin[i], OUTPUT);
    pinMode(blue[i]  , OUTPUT);
    pinMode(red[i]   , OUTPUT);
    pinMode(green[i] , OUTPUT);
  }
  
  Serial.begin(9600);
}

void loop(){
  //turn off all the colors
  clearLED();
 //accelerometer
  readXYZ();
  //display
  displayFace();
  //displayXYZ(); //for debugging of accelerometer
  

}

void readXYZ() {
  x = analogRead(xPin);
  y = analogRead(yPin);
  z = analogRead(zPin);
  
  //Map - convert 340 -> 0 and 270 -> 90 //these values are just for me, might need to be adjusted
  //constrain to values between -90 and 90
  x = constrain(map(x,340,270,0,90),-90,90);
  y = constrain(map(y,340,270,0,90),-90,90);
}

void displayXYZ() {
  Serial.print("x = ");
  Serial.print(x);
  Serial.print("  y = ");
  Serial.print(y);
  Serial.print("  z = ");
  Serial.print(z); 
  Serial.print("\n");
}

void clearLED() {
  for(int i = 0; i < 8; i++){
    digitalWrite(blue[i] , HIGH);
    digitalWrite(red[i]  , HIGH);
    digitalWrite(green[i], HIGH);
  }
}

void displayFace() {
  if(x < -70 || x > 70 || y < -70 || y > 70) {
    displaySprite(frown, red); 
  } else if (x < -30 || x > 30 || y < -30 || y > 30) {
    displaySprite(unhappy, blue);
  } else {
    displaySprite(smile, green);
  }
}

void displaySprite(byte * bitmap, int * color){
  for(int count = 0; count < 8; count++) { //utility counter
    for(int i = 0; i < 8; i++) {
      digitalWrite(rowPin[i], LOW);  //Turn off all row pins
    }
    for(int i = 0; i < 8; i++){  //Only active the arduino pin of the column
      if(i == count) {
        digitalWrite(color[i], LOW);
      }
      else{
        digitalWrite(color[i], HIGH);  //turn all the other rows off
      }
    }
    for(int row = 0; row < 8; row++) {  //iterate through each pixel in the column
      int bit = (bitmap[count] >> row) & 1;  //bit shift in smile to do bitwise comparison, assign the result to the bit
      if(bit == 1) {
        digitalWrite(rowPin[row], HIGH);
      } //end if
    } //end for loop
  } //end for loop
} //end function
