#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);  // Set the LCD I2C address, number of columns and rows

double sensorValue1 = 0;
int val1[100];
int max_v1 = 0;
double VmaxD1 = 0;
double VeffD1 = 0;
double solarVoltage = 0;

double sensorValue2 = 0;
int val2[100];
int max_v2 = 0;
double VmaxD2 = 0;
double VeffD2 = 0;
double wapdaVoltage = 0;

double sensorValue3 = 0;
int val3[100];
int max_v3 = 0;
double VmaxD3 = 0;
double VeffD3 = 0;
double generatorVoltage = 0;

int current = 0;

int solarRelay = 5;
int wapdaRelay = 4;
int generatorRelay = 3;

void setup() {
  lcd.init();  // Initialize the LCD
  lcd.backlight();  // Turn on the backlight
  Serial.begin(115200);
  pinMode(solarRelay, OUTPUT);
  pinMode(wapdaRelay, OUTPUT);
  pinMode(generatorRelay, OUTPUT);
  digitalWrite(solarRelay, HIGH);
  digitalWrite(wapdaRelay, HIGH);
  digitalWrite(generatorRelay, HIGH);
}

void loop() {
  // For Solar
  for (int i = 0; i < 100; i++) {
    sensorValue1 = analogRead(A0);
    if (analogRead(A0) > 511) {
      val1[i] = sensorValue1;
    } else {
      val1[i] = 0;
    }
    delay(1);
  }

  max_v1 = 0;

  for (int i = 0; i < 100; i++) {
    if (val1[i] > max_v1) {
      max_v1 = val1[i];
    }
    val1[i] = 0;
  }
  if (max_v1 != 0) {
    VmaxD1 = max_v1;
    VeffD1 = VmaxD1 / sqrt(2);
    solarVoltage = (((VeffD1 - 420.76) / -90.24) * -210.2) + 120;
  } else {
    solarVoltage = 0;
  }
  Serial.print("Solar Voltage: ");
  Serial.print(solarVoltage);
  VmaxD1 = 0;

  // For Wapda
  for (int i = 0; i < 100; i++) {
    sensorValue2 = analogRead(A1);
    if (analogRead(A1) > 511) {
      val2[i] = sensorValue2;
    } else {
      val2[i] = 0;
    }
  }

  max_v2 = 0;

  for (int i = 0; i < 100; i++) {
    if (val2[i] > max_v2) {
      max_v2 = val2[i];
    }
    val2[i] = 0;
  }
  if (max_v2 != 0) {
    VmaxD2 = max_v2;
    VeffD2 = VmaxD2 / sqrt(2);
    wapdaVoltage = (((VeffD2 - 420.76) / -90.24) * -210.2) + 120;
  } else {
    wapdaVoltage = 0;
  }
  Serial.print("  Wapda Voltage: ");
  Serial.print(wapdaVoltage);
  VmaxD2 = 0;

  delay(100);

  // For Generator
  for (int i = 0; i < 100; i++) {
    sensorValue3 = analogRead(A2);
    if (analogRead(A2) > 511) {
      val3[i] = sensorValue3;
    } else {
      val3[i] = 0;
    }
    delay(1);
  }

  max_v3 = 0;

  for (int i = 0; i < 100; i++) {
    if (val3[i] > max_v3) {
      max_v3 = val3[i];
    }
    val3[i] = 0;
  }
  if (max_v3 != 0) {
    VmaxD3 = max_v3;
    VeffD3 = VmaxD3 / sqrt(2);
    generatorVoltage = (((VeffD3 - 420.76) / -90.24) * -210.2) + 120;
  } else {
    generatorVoltage = 0;
  }
  Serial.print("  Generator Voltage: ");
  Serial.println(generatorVoltage);
  VmaxD3 = 0;

  delay(100);

  // Read the current sensor
  current = analogRead(A3);

  // Convert current reading to actual current value
  float amps = ((current - 512) * 5.0) / 1024.0;

  // Display voltage and current values on LCD
  lcd.clear();  // Clear the display before updating
  lcd.setCursor(0, 0);
  lcd.print("Solar: ");
  lcd.print(solarVoltage);
  lcd.print("V ");
  lcd.print(amps);
  lcd.print("A");

  lcd.setCursor(0, 1);
  lcd.print("Wapda: ");
  lcd.print(wapdaVoltage);
  lcd.print("V ");
  lcd.print(amps);
  lcd.print("A");

  lcd.setCursor(0, 2);
  lcd.print("Gener: ");
  lcd.print(generatorVoltage);
  lcd.print("V ");
  lcd.print(amps);
  lcd.print("A");

  // Check which source to use
  lcd.setCursor(0, 3);  // Set the cursor to the correct position
  if (solarVoltage >= 170) {
    digitalWrite(wapdaRelay, HIGH);
    digitalWrite(generatorRelay, HIGH);
    delay(2000);
    digitalWrite(solarRelay, LOW);
    lcd.print("Using Solar   ");
  } else if (wapdaVoltage >= 170) {
    digitalWrite(solarRelay, HIGH);
    digitalWrite(generatorRelay, HIGH);
    delay(2000);
    digitalWrite(wapdaRelay, LOW);
    lcd.print("Using Wapda   ");
  } else {
    digitalWrite(solarRelay, HIGH);
    digitalWrite(wapdaRelay, HIGH);
    delay(2000);
    digitalWrite(generatorRelay, LOW);
    lcd.print("Using Genera  ");
  }

  delay(1000);  // Delay to update the display
}