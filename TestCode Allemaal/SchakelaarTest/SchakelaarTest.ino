

#define LED 2
#define SCHAKELAAR 5
#define DELAY 


bool datalogging;

void setup() {
  
  Serial.begin(9600);

  pinMode(LED, OUTPUT);
  pinMode(SCHAKELAAR, INPUT);
}

void loop() {

int SchakelaarStatus = digitalRead(SCHAKELAAR);
 // Serial.println(SchakelaarStatus);
  
  if (SchakelaarStatus==HIGH){
    datalogging = 1;
    digitalWrite(LED, HIGH);
     }

  if (SchakelaarStatus==LOW){
    datalogging = 0;
    digitalWrite(LED, LOW);
  }
    Serial.println(datalogging);
      delay(250);
} 
