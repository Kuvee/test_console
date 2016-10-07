void encoderHomeInterrupt(){
  detachInterrupt(digitalPinToInterrupt(coarseEncoder));
  home = 1;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(coarseEncoder, INPUT);
  attachInterrupt(digitalPinToInterrupt(coarseEncoder), encoderHomeInterrupt, RISING);  
    
}


void loop() {
  // put your main code here, to run repeatedly:


if (home==1){
  Serial.print("home");
  home = 0;  
}
}
