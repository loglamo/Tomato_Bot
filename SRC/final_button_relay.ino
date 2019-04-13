int relay_pin = 8;
int led_pin = 13;
void setup() {
  Serial.begin(9600);
  
  pinMode(2, INPUT_PULLUP); // enabling internal pull-up resistor
  pinMode(relay_pin,OUTPUT);
  pinMode(led_pin,OUTPUT);  
  digitalWrite(led_pin,HIGH);}

 
void loop() {
  int val = digitalRead(2);
  Serial.println(val);
 
  if (val == LOW) {
    digitalWrite(relay_pin, HIGH);
    delay(5000);
  } else {
    digitalWrite(relay_pin, LOW);
  }
}
