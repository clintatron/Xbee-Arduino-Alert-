/* AlertNodeDemo
   ahdavidson, 10/2016

   This is a simple demo of a program that can use an XBee to receive alert messages
   sent by a SmartCity gateway. See accompanying docs for details on how to connect the XBee.

   Then use the AlertNodeLib library to simplify your code to receive the messages.
   The basic idea is that you need to ask the node object each time through your main loop
   (via the alertReceived() method) if there is an incoming alert message for you.
   If there is, then you can process it; otherwise just go on and do whatever else you
   need to do (checking your sensors, etc.) in your main loop. If there is an alert, you
   can decide what action your node should take based on what kind of alert has been sent.

*/

// you must include these 2 libraries for the AlertNode and XBee message stuff
#include <SoftwareSerial.h>
#include <AlertNodeLib.h>
#include "Wire.h"
#include "Adafruit_LiquidCrystal.h"


Adafruit_LiquidCrystal lcd(0);

// use a button in the program
#include <Button.h>

// some constants to identify my node to the library object
const String myNodeName = "Node Andy";

// because every Arduino program has to blink an LED
const int ledPin = LED_BUILTIN;  // pin 13, no doubt

// the pin the button is connected to
const int btnPin = 7;

// the pin the buzzer is connected to
const int buzzer = 9; //buzzer to arduino pin 9

// this connects your program to your XBee module
// you must use pin 2 for TX and pin 3 for RX
// the XBee must be set to 9600 baud
AlertNode myNode;

// set up a button to use for input
Button button(btnPin);

// state variables
boolean myDebugging = true;


void setup() {

  // just for our own debugging
  Serial.begin(9600);
  Serial.println("\n\n*** Starting AlertNode demo");

  // you want to blink that LED, right?
  pinMode(ledPin, OUTPUT);

  // just flash it for a second at startup so we know things have initialized
  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);

  // set up the LCD's number of rows and columns: 
  lcd.begin(16, 2);

  // comment this line out to suppress debugging messages from the library
  //myNode.setDebugOn();

  // this starts up your XBee and does other AlertNode initialization stuff.
  // the argument is just to identify your node by a name.

  myNode.begin(myNodeName);

}


void loop() {
  
 // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  //lcd.print(millis()/1000);
  
  // check for an incoming alert
  // alertReceived() returns 0 (NO_ALERT) if none have been sent.
  // it returns a code to indicate follows if there is one:
  //       AlertNode::FIRE
  //       AlertNode::GAS
  //       AlertNode::FLOOD
  //       AlertNode::BURGLARY
  //       AlertNode::EARTHQUAKE
  //       AlertNode::ZOMBIE
  //       AlertNode::APOCALYPSE
  // there are other alert codes defined for specific clients.

  int alert = myNode.alertReceived();
  if (alert != AlertNode::NO_ALERT) {
    // in this demo, we'll just print out a message about it
    logAlert(myNodeName, alert);
    // here is where you would do the real work of processing this alert:
    // if (alert == AlertNode::FIRE) {
    //   SOUND THE ALARM
    // }
  }

  // do other stuff for your node now
  // in this example, I'll just use the button to toggle the Alert debugging on and off
  if (button.checkButtonAction() == Button::PRESSED) {
    myDebugging = !myDebugging;
    Serial.print("+++ Setting debugging ");
    Serial.println(myDebugging ? "ON" : "OFF");
    myNode.setDebug(myDebugging);
  }

}


void logAlert (String myName, int alert) {

  // just blink the led and write a message out to the serial monitor
  digitalWrite(ledPin, HIGH);
  delay(100);

  if (myDebugging) {
    Serial.print("*** alert received at ");
    float sec = millis() / 1000.0;
    //lcd.println(sec);
    Serial.print(alert);
    //lcd.print(": ");
    lcd.println(myNode.alertName(alert));
    tone(buzzer, 1000); // Send 1KHz sound signal...
    delay(1000);        // ...for 1 sec
    noTone(buzzer);     // Stop sound...
    delay(1000);        // ...for 1sec
    tone(buzzer, 1000); // Send 1KHz sound signal...
    delay(1000);        // ...for 1 sec
    noTone(buzzer);     // Stop sound...
    delay(1000);        // ...for 1sec
    tone(buzzer, 1000); // Send 1KHz sound signal...
    delay(1000);        // ...for 1 sec
    noTone(buzzer);     // Stop sound...
    delay(1000);        // ...for 1sec
  }

  // end of the blink
  digitalWrite(ledPin, LOW);

}
