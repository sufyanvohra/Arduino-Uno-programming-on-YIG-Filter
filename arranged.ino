
#include <stdio.h>
#include <Keypad.h>
#include <UTFT.h>
#include <string.h>


const byte ROW_NUM = 5; // Four rows
const byte COLUMN_NUM = 4; // Three columns
// Define the Keymap
char keys[ROW_NUM][COLUMN_NUM] = {
  {'Q', 'W', '#', '*'},
  {'1', '2', '3', 'U'},
  {'4', '5', '6', 'D'},
  {'7', '8', '9', 'X'},
  {'L', '0', 'R', 'E'},
};
// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte pin_rows[ROW_NUM] = {10, 9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {2, 3, 4, 5}; //connect to the column pinouts of the keypad


// Declare which fonts we will be using
extern uint8_t SmallFont[];

// Initialize display
// Library only supports software SPI at this time
//NOTE: support  DUE , MEGA , UNO
//SDI=11  SCL=13  /CS =10  /RST=8  D/C=9

UTFT myGLCD(ST7735S_4L_80160, SDA, SCL, 11, 13, 12); //LCD:  4Line  serial interface      SDI  SCL  /CS  /RST  D/C    NOTE:Only support  DUE   MEGA  UNO

// Declare which fonts we will be using
extern uint8_t BigFont[];

int color = 0;
word colorlist[] = {VGA_WHITE, VGA_BLACK, VGA_RED, VGA_BLUE, VGA_GREEN, VGA_FUCHSIA, VGA_YELLOW, VGA_AQUA};
int  bsize = 4;

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );



void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));

  //Interupt for the new value
  pinMode(A5, INPUT);
;

  pinMode(A3, OUTPUT);
  digitalWrite(A3, LOW);
 // Setup the LCD
  myGLCD.InitLCD();
  myGLCD.setFont(BigFont);


}

void loop() {


  String word = takeInput();
  

   Serial.println("The bits sent are: ");



  myGLCD.fillScr(0, 0, 255);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 0, 0);
  myGLCD.print("Steps: ", 150 , 50, 180);
  myGLCD.print(word, 150 , 20 , 180);


int  y = word.toInt();



  printOut1(y); // Send 12 bit values to the pins
  Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

 

// Waiting for interrupt part to test values

  nextInput();



}

void printOut1(int c) {
  int Dpin[] = {2, 3, 4, 5, 6, 7, 8, 9, 10};
  for (int i = 2; i <= 10; i++)
  {
    pinMode(i, OUTPUT);
  }


  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
 // pinMode(A4, OUTPUT);

  
// GIVING 12 bits output.

  int outBit1 = 10;
  for (int bits = 11; bits > 2; bits--) {
    // Compare bits 7-0 in byte
    if (c & (1 << bits)) {
      Serial.print ("1");
      digitalWrite(outBit1, HIGH);
      //       Serial.println("Portno");
      //       Serial.println(outBit1);
    }
    else {
      Serial.print ("0");
      digitalWrite(outBit1, LOW);
      //        Serial.println("Portno");
      //        Serial.println(outBit1);
    }

    outBit1--;
  }
  for (int bits2 = 2; bits2 > -1; bits2--) {

    // Compare bits 7-0 in byte
    if (c & (1 << bits2)) {
      Serial.print ("1");
      if ( bits2 == 2 )
      {
        digitalWrite(A2, HIGH);
      }
      if ( bits2 == 1 )
      {
        digitalWrite(A1, HIGH);
      }
      if ( bits2 == 0 )
      {
        digitalWrite(A0, HIGH);
      }

    }

    else {
      Serial.print ("0");
      if ( bits2 == 2 )
      {
        digitalWrite(A2, LOW);
      }
      if ( bits2 == 1 )
      {
        digitalWrite(A1, LOW);
      }
      if ( bits2 == 0 )
      {
        digitalWrite(A0, LOW);
      }

    }
  }

  digitalWrite(A3,HIGH )  ; // clock for the YIG sensor
   delay (100);
  digitalWrite(A3, LOW)  ;



}

void nextInput()


   int x = 0;
do{
  char newinput = keypad.waitForKey();
  if (newinput == 'E'){
     break;}

}while(1);

}

String takeInput(){
  String word = "";
  myGLCD.fillScr(0, 0, 0);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 0, 255);
  myGLCD.print("Steps:", 150 , 50, 180);


  int arr[4];
  int i = 0;
  for (i = 0; i <= 3; i++)
  {
    int key = keypad.waitForKey();
    if (key) {
      arr[i] = key; // Initializing each element seperately
      if (isDigit(key)) {
        // convert the incoming byte to a char and add it to the string:
        word += (char)key;
      }

    }
  }
  Serial.print("The numbers entered are: " );
  Serial.print(word);
  Serial.println(" ");
  return word;
}
