#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
//  production 0.1
#define buz 9
#define fanPin A3
#define chikSerangPin 0
#define switchGlogging 13
#define tempPin A0
// DIRECTION LOW FOR FORWARD ROTATION
//SET DIRECTION HIGH FOR BACKWARD ROTATION
#define dir 10
//for test
int current_stteps = 0 ;


//#define Step 105
#define Step 11
bool clogging_serang = false;
int in_temp = 40 ;
bool isSerang = true ; // read if theris serang plugin or not
int currenPage = 0;
bool statusSerang = false ;
int typeSerang = 50 ;
long timePumping = 1;
long volume ;
int oldPageForErrorSerang = 0 ;
long stepps_for_complete_Syrang = 2500;
long stepps = 200 ;
long delay_step = 1 ;
// click on pull or push fro serang
bool isClick = true ;



// define functions for scope this called for c++  :)
void setTypeSerang(String type = "Defualt");
void setTime(String type = "Defualt");
void printCahnging( int col = 1 , int row = 0 , String content = " ");
void noSerangScreen ();
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
    noSerangScreen();
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

  delay (2000);
  pinMode(dir, OUTPUT);   // DIRECTION AS OUTPUT
  pinMode(Step, OUTPUT);  // STEP AS OUTPUT
  pinMode  ( chikSerangPin, INPUT_PULLUP);
  pinMode  ( switchGlogging, INPUT_PULLUP);
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.setCursor(2, 0);
  tone (buz, 700, 500);
  lcd.print("Welcom To (: ");
  //  delay(3000);
  //  tone (buz, 700, 500);
  lcd.clear();
//  Serial.begin(9600);
  pinMode (tempPin,  INPUT );
  //  tempScreen ();
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
        if (volume + String(key).toInt () < typeSerang ) {
          volume = inputString.toInt();
        } else {
          toneF();
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
    case 5:
      printCahnging( 1 , 12 , "ON");
      break;
    case 6:
      printCahnging( 1 , 12 , "Temp");
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
  current_stteps = 0;
  char key = keypad.getKey();
  if (key) {
    tone (buz, 700, 200);
    if (key == '*') {
      count++;
      if (count == 7) {
        count = 0;
      }

      printParameter ();
      delay (200);
    } else if (key == '#') {
      // isClick = true for enter in while loop if tap on # key and count == 3 or 4
      isClick = true;
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
          while ( isClick) {
            char key = keypad.getKey();
            if (key) {
              if (key == '#') {
                tone (buz, 700, 200);
                isClick = false;
                break;
              }
            }
            // pull Stepper motor below
            pull();
          }
          break;
        case 4:
          while ( isClick) {
            char key = keypad.getKey();
            if (key) {
              if (key == '#') {
                tone (buz, 700, 200);
                isClick = false;
                break;
              }
            }
            // push Stepper motor below
            push();
          }
          break;

        case 5:

          while ( isClick) {
            char key = keypad.getKey();
            if (key) {
              if (key == '#') {
                tone (buz, 700, 200);
                isClick = false;
                break;
              }
            }
            runing ();
            for (int i = 0 ; i < 3; i++ ) {
              sound ();
            }
            isClick = false ;
          }
          break ;
        case 6:
          tempScreen ();

          break ;

      }
    }
  }
}

void noSerangScreen ( ) {
  lcd .clear ();
  lcd.setCursor(4, 0);
  lcd.print ( "ERROR !!  ");
  lcd.setCursor(0, 1);
  lcd.print ( "No Serang Plugin");
  while (isSerang == 0) {
    isSerang = !digitalRead(chikSerangPin);
    sound ();
    currenPage = oldPageForErrorSerang;
  }
  lcd .clear ();
  mainScreen ();
}


void printStatusSerang(String status ) {
  lcd.setCursor(0, 0);
  lcd.print ("Status:");
  lcd.print (status);
}

void mainScreen () {
  readTemp();
  printParameter();
  while ( currenPage == 4 ) {
    lcd.setCursor(0, 0);
    lcd.print ("Status:");
    if (statusSerang) {
      printStatusSerang("ON  ");
    } else {
      printStatusSerang(" OFF ");
    }
    lcd.setCursor(14, 0);
    lcd.print (typeSerang);
    lcd.setCursor(0, 1);
    lcd.print (timePumping);
    lcd .print ("M  ");
    lcd.print (volume);
    lcd.print ("ML ");
    oldPageForErrorSerang = 4;
    chikSerang();
    controllMainScreen();
  }
}
void runing () {
  chikSerang();
  printStatusSerang("  ON");
  readTemp();
  //  SET DIRICTION MOTOR
  digitalWrite(dir, HIGH);
  //  calculation all parameter ..
  calculation();
  for (int i = 0 ; i < stepps; i++ ) {
    clogging_serang = digitalRead(switchGlogging);
    if (clogging_serang ) {
      clogginError();
    }

    //    chac temp motor
    readTemp();

    char key = keypad.getKey();
    if (key == '#') {
      sound ();
      mainScreen();
    }
    printStatusSerang("  ON");
    chikSerang();

    while (!isSerang) {
      sound ();
      chikSerang();
    }
    digitalWrite(Step, HIGH);
    delay(delay_step / 2);
    digitalWrite(Step, LOW);
    delay(delay_step / 2);
  }
}


void calculation () {
  //LOGICT FOR CALCULAT STEPS AND SPEED
  //typeSerang => TYPE SYRANG
  //timePumping => REQUIRE TIME PUMPING
  //volume => REAUIRED VOLUM
  //stepps => REQUIRED  STEPPES FOR COMPLET ONE SYRANG
  //delay_step => REQUIRED DELAY TIME BETWEEN STEPPS

  //  get reauierd number stepps for complet a requierd volum .
  stepps = (long)volume * (long)stepps_for_complete_Syrang   ;
  stepps = stepps / typeSerang ;
  //  convert between secund and millesecund
  delay_step = ((long)timePumping * 60 ) * 1000   ; // result is millesecund

  //    get requierd time delay for one step
  delay_step = (long)delay_step / (long)stepps;
}
void clogginError() {
  while (clogging_serang) {
    clogging_serang = digitalRead(switchGlogging);
    sound();
  }
}
void push () {
  current_stteps++;
  printStatusSerang("Push");
  digitalWrite(dir, HIGH);
  digitalWrite(Step, HIGH);
  delay(1);
  digitalWrite(Step, LOW);
  delay(1);
}
void pull ()
{

  printStatusSerang("Pull");
  digitalWrite(dir, LOW);
  digitalWrite(Step, HIGH);
  delay(1);
  digitalWrite(Step, LOW);
  delay(1);
}


void tempScreen () {

  lcd.clear ();

  while (1) {
    char key = keypad.getKey();
    delay(200);
    lcd.setCursor(0, 0);
    lcd .print ("temp motor is ");
    float temp = readTemp();
    lcd.setCursor(0, 1);
    lcd.print (String(temp));
    lcd.setCursor(11, 1);
    lcd.print (">");
    lcd.print(in_temp);
    if (key) {
      toneF();
      if (key >= '0' && key <= '9') {     // only act on numeric keys
        lcd.clear ();
        inputString += key;
        if (in_temp + int(key) < 100) {
          in_temp = inputString.toInt();
        } else {
          in_temp = 00;
          inputString = "";
        }
      } else if (key == '#') {
        inputString = String( in_temp);
        if (inputString.length() > 0) {
          in_temp  = inputString.toInt(); // YOU GOT AN INTEGER NUMBER
          inputString = "";               // clear input
          toneF();
          lcd .clear ();
          mainScreen ();
        }
      }
    }
  }
}

float readTemp () {
  float reade = (analogRead(A0) * 4.88) / 1000;
  reade = reade * 13.43;
  if (reade >= in_temp) {
    digitalWrite(fanPin, HIGH);
  }
  if (reade + 3 <= in_temp)  {
    digitalWrite(fanPin, LOW);
  }
  return reade;
}
