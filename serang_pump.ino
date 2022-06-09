#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define buz 9
#define chikSerangPin 13
#define switchLimet 13


bool isSerang = true ; // read if theris serang plugin or not
int currenPage = 0;
bool statusSerang = false ;
int typeSerang = 50 ;
int timePumping ;
int volume ;
int oldPageForErrorSerang = 0 ;




// define functions for scope this called for c++  :)
void setTypeSerang(String type = "Defualt");
void setTime(String type = "Defualt");
void printCahnging( int col = 1 , int row = 0 , String content = " ");

int count = 0;
String   inputString = "";



int numPage = 2;
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {7, 2, 3, 5};
byte colPins[COLS] = {6, 8, 4};
//initialize an instance of class
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


void chikSerang ( ) {
  isSerang = ! digitalRead (chikSerangPin);
  if (!isSerang) {
    currenPage = 0;
  }
}
// functions utilitis  below  .....
void sound(int frq = 300 ) {
  digitalWrite(buz, 1);
  delay(frq);
  digitalWrite(buz, 0);
  delay(frq);
}
void toneF (int frq = 700 , int duration = 200) {
  tone (buz, frq, duration );
}


void setup() {

  pinMode  ( chikSerangPin, INPUT_PULLUP);
  pinMode  ( switchLimet, INPUT_PULLUP);

  lcd.begin(16, 2);
  lcd.backlight();
  lcd.setCursor(2, 0);
  tone (buz, 700, 500);
  lcd.print("Welcom To (: ");
  //  delay(3000);
  //  tone (buz, 700, 500);
  lcd.clear();
  Serial.begin(9600);

}






void setvolume(String type = "Defualt") {
  if (volume > 0) {
    inputString = String (volume);
  }
  delay(200);
  while (currenPage == 2) {
    lcd.setCursor(0, 0);
    lcd.print ("Set Volume");
    lcd.setCursor(0, 1);
    lcd.print (volume);
    lcd.print (" mL");
    char key = keypad.getKey();
    if (key) {
      tone (buz, 700, 200);
      if (key == '#' && inputString.length() > 0 ) {
        inputString = "";
        lcd.clear();
        if (type == "Defualt") {
          currenPage = 3;
          setTime();
        } else {
          currenPage = 4;
          mainScreen();
        }
      } else if (key == '*') {
        inputString = "";
        lcd.clear();
        if (type == "Defualt") {
          currenPage = 1;
          setTypeSerang();
        } else {
          currenPage = 4;
          mainScreen();
        }
      }

      if (key >= '0' && key <= '9') {     // only act on numeric keys
        lcd.setCursor(0, 1);
        lcd.print ("            ");
        inputString += key;
        if (volume + int(key) < 100) {
          volume = inputString.toInt();
        } else {
          volume = 00;
          inputString = "";
        }
      } else if (key == '#') {
        if (inputString.length() > 0) {
          volume = inputString.toInt(); // YOU GOT AN INTEGER NUMBER
          inputString = "";               // clear input
          // DO YOUR WORK HERE
        }
      }
    }
  }
  lcd.clear();
}


void setTime(String type = "Defualt") {
  if (timePumping > 0) {
    inputString = String (timePumping);
  }
  delay(200);
  while (currenPage == 3) {
    lcd.setCursor(0, 0);
    lcd.print ("Set Time");
    lcd.setCursor(0, 1);
    lcd.print (timePumping);
    lcd.print (" M");
    char key = keypad.getKey();
    if (key) {
      tone (buz, 700, 200);
      if (key == '#' && inputString.length() > 0) {
        currenPage = 4;
        inputString = "";
        lcd.clear();
        mainScreen();
      } else if (key == '*') {
        inputString = "";
        lcd.clear();
        if (type == "Defualt") {
          currenPage = 2;
          setvolume();
        } else
        {
          currenPage = 4;
          mainScreen();
        }


      }
      if (key >= '0' && key <= '9') {     // only act on numeric keys
        lcd.setCursor(0, 1);
        lcd.print ("         ");
        inputString += key;
        if (timePumping + int(key) < 100) {
          timePumping
            = inputString.toInt();
        } else {
          timePumping = 00;
          inputString = "";
        }
      } else if (key == '#') {
        if (inputString.length() > 0) {
          volume = inputString.toInt(); // YOU GOT AN INTEGER NUMBER
          inputString = "";               // clear input
          // DO YOUR WORK HERE
        }
      }
    }
  }
  lcd.clear();
}

void setTypeSerang(String type = "Defualt") {
  inputString = String (typeSerang);
  while (currenPage == 1) {
    lcd.setCursor(0, 0);
    lcd.print ("Set Type Serang");
    lcd.setCursor(0, 1);
    lcd.print (typeSerang);
    lcd.print (" mL");
    char key = keypad.getKey();
    if (key) {
      tone (buz, 700, 200);
      if (key == '#' && inputString.length() > 0) {
        inputString = "";
        lcd.clear();
        if (type == "Defualt") {
          currenPage = 2;
          setvolume();
        } else {
          currenPage = 4;
          mainScreen();
        }
      } else if (key == '*') {
        lcd.clear();
        if (key != "Defualt") {
          currenPage = 4;
          mainScreen();
        }
      }
      if (key >= '0' && key <= '9') {    // only act on numeric keys
        lcd.setCursor(0, 1);
        lcd.print ("         ");
        inputString += key;
        if (typeSerang + int(key) < 100) {
          typeSerang = inputString.toInt();
        } else {
          typeSerang = 00;
          inputString = "";
        }
      } else if (key == '#') {
        if (inputString.length() > 0) {
          typeSerang = inputString.toInt(); // YOU GOT AN INTEGER NUMBER
          inputString = "";               // clear input
          // DO YOUR WORK HERE
        }
      }
    }
  }

}






void loop() {
  chikSerang();
  //  read if Serang plugdin or not
  lcd.clear();
  if (isSerang) {
    if (typeSerang != NULL && timePumping != NULL && volume != NULL ) {
      mainScreen();
    } else {
      currenPage = 1;
      switch (currenPage) {
        case 1:
          setTypeSerang ();
          break;
        case 2:
          setvolume ();
          break;
        case 3:
          setTime ();
          break;
        case 4:
          mainScreen ();
          break;
      }
    }
  } else {
    //    error Serang ..
    lcd.clear();
    currenPage = 0;
    noSerangScreen( );

  }
}


void printParameter() {
  switch (count) {
    case 0:
      printCahnging( 1 , 12 , "Time");
      break;
    case 1:
      printCahnging( 1 , 10 , "Volume");
      break;
    case 2:
      printCahnging( 1 , 12 , "Type");
      break;
    case 3:
      printCahnging( 1 , 12 , "Pull");
      break;
    case 4:
      printCahnging( 1 , 12 , "Push");
      break;
  }
}


void  printCahnging ( int col = 1 , int row = 0 , String content = " "  ) {
  lcd.setCursor(row - 2, col);
  lcd.print ("       " );
  lcd.setCursor(row, col);
  lcd.print (content);
}


void controllMainScreen () {
  char key = keypad.getKey();
  if (key) {
    tone (buz, 700, 200);
    if (key == '*') {
      count++;
      if (count == 5) {
        count = 0;
      }

      printParameter ();
      delay (200);
    } else if (key == '#') {
      switch (count) {
        case 0:
          lcd.clear();
          delay(200);
          currenPage = 3;
          setTime("main");
          break;
        case 1:
          lcd.clear();
          delay(200);
          currenPage = 2;
          setvolume("main");
          break;
        case 2:
          lcd.clear();
          delay(200);
          currenPage = 1;
          setTypeSerang("main");
          break;
        case 3:
          //          printCahnging( 1 , 12 , "Pull");
          break;
        case 4:
          //          printCahnging( 1 , 12 , "Push");
          break;
      }
    }
  }
}

void noSerangScreen (  ) {
  lcd.setCursor(4, 0);
  lcd.print ( "ERROR !!  ");
  lcd.setCursor(0, 1);
  lcd.print ( "No Serang Plugin");
  while (isSerang == 0) {
    isSerang = !digitalRead(chikSerangPin);
    sound ();
    currenPage = oldPageForErrorSerang;
  }
}



void mainScreen () {
  printParameter();
  while ( currenPage == 4 ) {
    lcd.setCursor(0, 0);
    lcd.print ("Status:");
    if (statusSerang) {
      lcd.print ("  On");
    } else {
      lcd.print (" OFF");
    }

    lcd.setCursor(14, 0);
    lcd.print (typeSerang);
    lcd.setCursor(0, 1);
    lcd.print (timePumping);
    lcd .print ("M  ");
    lcd.print (volume);
    lcd.print ("ML");
    oldPageForErrorSerang = 4;
    chikSerang();
    controllMainScreen();


  }
}








void println (String content) {
  Serial.println (content);
}
void print (String content) {
  Serial.print(content);
}
