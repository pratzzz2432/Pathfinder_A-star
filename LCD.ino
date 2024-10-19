#include <LiquidCrystal.h>

/* Create object named lcd of the class LiquidCrystal */
LiquidCrystal lcd(13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3);  /* For 8-bit mode */
// LiquidCrystal lcd(13, 12, 11, 6, 5, 4, 3);  /* For 4-bit mode */

unsigned char Character1[8] = { 0x04, 0x1F, 0x11, 0x11, 0x1F, 0x1F, 0x1F, 0x1F };  /* Custom Character 1 */
unsigned char Character2[8] = { 0x01, 0x03, 0x07, 0x1F, 0x1F, 0x07, 0x03, 0x01 };  /* Custom Character 2 */

String receivedString = "";  // Variable to store the string received from serial

void setup() {
  lcd.begin(16, 2);               /* Initialize 16x2 LCD */
  lcd.clear();                    /* Clear the LCD */
  lcd.createChar(0, Character1);  /* Generate custom character */
  lcd.createChar(1, Character2);
  
  Serial.begin(9600);  // Initialize serial communication at 9600 baud rate
  while (!Serial) {
    ;  // Wait for the serial port to connect. Needed for some boards like Leonardo.
  }
  
  Serial.println("Send a string to display on the LCD");
}

void loop() {
  if (Serial.available() > 0) {
    receivedString = Serial.readStringUntil('\n');  // Read the string until newline
    receivedString.trim();  // Remove any trailing or leading whitespaces
  }

  // If receivedString is empty, skip to avoid displaying garbage
  if (receivedString.length() > 0) {
    lcd.clear();  // Clear the LCD before displaying the string

    if (receivedString.length() > 16) {
      // Scroll long strings
      for (int i = 0; i < receivedString.length() - 15; i++) {
        lcd.setCursor(0, 0);  // Set cursor at the beginning of the first line
        lcd.print(receivedString.substring(i, i + 16));  // Print 16-char segment
        delay(500);  // Add delay for scrolling effect
        lcd.clear();
      }
    } else {
      // Display short strings directly
      lcd.setCursor(0, 0);
      lcd.print(receivedString);
    }

    lcd.setCursor(0, 1);  // Set cursor to the second line
    lcd.write(byte(0));   // Display custom characters
    lcd.write(byte(1));

    delay(200);  // Delay for 2 miliseconds before repeating the loop
  }
}
