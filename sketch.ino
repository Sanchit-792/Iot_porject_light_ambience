#define BLYNK_PRINT Serial


#define BLYNK_TEMPLATE_ID "TMPL3EtNTUNqu"                   //Blynk template id
#define BLYNK_TEMPLATE_NAME "LED"                           //Blynk template name
#define BLYNK_AUTH_TOKEN "bTp8vJu-bTwPMnspJOowRGIbftE8nfX_" //Blynk authentication token

#include <WiFi.h>
#include <WiFiClient.h>             //Wifi library
#include <BlynkSimpleEsp32.h>       //Blynk library for esp32 

const int LDR_PIN = 2;    //Pin defined for LDR sensor
const int LED_PIN = 13;   //Pin defined for LED
const int PIR_PIN = 14;   //Pin defined for PIR sensor
const int BUZZ_PIN = 15;  //Pin defined for Buzzer

unsigned int last_movement_time = 0;    //Variable store the time when LED is on
bool led_status = false ;               //Stores LED state ON/OFF(true/false)
int count = 0;                          //A counter to track no. of movement

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LDR_PIN, INPUT);      
  pinMode(LED_PIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(BUZZ_PIN, OUTPUT);
  Serial.print("Connecting to WiFi");
  WiFi.begin("Wokwi-GUEST", "", 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println(" Connected!");

  Blynk.begin(BLYNK_AUTH_TOKEN, "Wokwi-GUEST", "");   //To connect to blynk iot 
  
}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();                          //Starts Blynk iot 
  int pir_val = digitalRead(PIR_PIN);   //The variable stores the digital value of the PIR sensor
  int ldr_val = analogRead(LDR_PIN);    //The variable stores the analog value(o-1023) of the LDR sensor
  const int brightness_threshold = 500; //A preset or reference value for ambient lights

  if (pir_val == HIGH && ldr_val < brightness_threshold ){    //Checks if motion is detected and brightness is level
    digitalWrite(LED_PIN, HIGH);                              //If the condition is satisified LED is turned on
    if (!led_status) {                                        //Check for led state
    Serial.println("LED status ON");                          //Prints led status on console
    led_status = true;                                        //Updates the led status
    }
    last_movement_time = millis();                            
    ++count;                                                  //Increment the counter
  }

  if(count == 1){                                             //Check for the first entry
    digitalWrite(BUZZ_PIN, HIGH);                             //If the condition is satisfied buzzer is turend on 
    delay(500);
    digitalWrite(BUZZ_PIN, LOW);
  }


  if(millis() - last_movement_time > 30000 && led_status){    //Checks if the led is on for 30 sec and for led status on
    digitalWrite(LED_PIN, LOW);                               //Stwiches of the led after 30 sec
    Serial.println("LED status OFF");                         //update led status on the console
    led_status = false;                                       //update led status
  } 

  delay(500); // this speeds up the simulation
}