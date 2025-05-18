#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);
SoftwareSerial mySerial(0, 1); // RX, TX (Arduino -> ESP32)

const int startBtn = 2;
const int tempBtn = 3;
const int stopBtn = 4;
const int relayPin = 5;

String temps[] = {"COOL", "WARM", "HOT"};
int prices[] = {3, 4, 5};
int tempIndex = 0;

enum State {WELCOME, SELECT_TEMP, SHOW_CODE, WAITING, STARTED};
State currentState = WELCOME;

void setup() {
  pinMode(startBtn, INPUT_PULLUP);
  pinMode(tempBtn, INPUT_PULLUP);
  pinMode(stopBtn, INPUT_PULLUP);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);

  lcd.init();
  lcd.backlight();

  Serial.begin(9600);
  mySerial.begin(9600);

  showWelcome();
}

void loop() {
  // 🔴 1. STOP BUTTON — highest priority
  if (digitalRead(stopBtn) == LOW) {
    Serial.println("Stop button pressed. Resetting.");
    resetAll();
    return;
  }

  // 📡 2. Always listen to ESP32 — but only if not in WELCOME state
  // This prevents ESP messages from affecting after reset
  if (currentState != WELCOME && mySerial.available()) {
    String status = mySerial.readStringUntil('\n');
    status.trim();


    if (status == "paid") {
      digitalWrite(relayPin, LOW);
      if (currentState != STARTED) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Thank you!");
        lcd.setCursor(0, 1);
        lcd.print("Starting machine...");
        currentState = STARTED;
      }
    } else {
      digitalWrite(relayPin, HIGH);
    }
  }

  // 🔁 3. State machine
  switch (currentState) {
    case WELCOME:
      if (digitalRead(startBtn) == LOW) {
        delay(200);
        currentState = SELECT_TEMP;
        showSelection();
      }
      break;

    case SELECT_TEMP:
      if (digitalRead(tempBtn) == LOW) {
        tempIndex = (tempIndex + 1) % 3;
        delay(200);
        showSelection();
      }

      if (digitalRead(startBtn) == LOW) {
        delay(200);
        currentState = SHOW_CODE;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Enter code: 0991");
        lcd.setCursor(0, 1);
        lcd.print("into WushWash bot");

        String sendData = temps[tempIndex] + "," + String(prices[tempIndex]);
        mySerial.println(sendData);
        Serial.println(sendData);

        currentState = WAITING;
      }
      break;

    case WAITING:
      // handled above
      break;

    case STARTED:
      // optional timer
      break;
  }
}



void showWelcome() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WELCOME TO WUSHWASH");
  lcd.setCursor(0, 1);
  lcd.print("Press START to begin");
}

void showSelection() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temps[tempIndex]);
  lcd.setCursor(0, 1);
  lcd.print("Price: RM");
  lcd.print(prices[tempIndex]);
}

void resetAll() {
  currentState = WELCOME;
  digitalWrite(relayPin, HIGH);
  tempIndex = 0;
  showWelcome();
}
