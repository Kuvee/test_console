#define coarseEncoder 2
#define LEDPIN 13
volatile int home;
void encoderHomeInterrupt(){
 // detachInterrupt(digitalPinToInterrupt(coarseEncoder));
  home = 1;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(coarseEncoder, INPUT_PULLUP);
  pinMode(LEDPIN, OUTPUT);

  digitalWrite(LEDPIN, LOW);
    
}


void loop() {
  static bool led=false;
  // put your main code here, to run repeatedly:
  attachInterrupt(digitalPinToInterrupt(coarseEncoder), encoderHomeInterrupt, RISING);  


if (home==1){
  //Serial.print("home");
  digitalWrite(LEDPIN, led);
  led = !led;
  home = 0;  

}
  delay(100);
}
