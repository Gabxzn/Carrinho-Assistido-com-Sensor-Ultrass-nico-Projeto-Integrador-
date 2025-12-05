// Esse código foi usado para testar o motor giratório
#define velmotor 3
#define mla 4
#define mlb 5
#define tmp 3000
int vel=0;

void setup() {
  pinMode (velmotor, OUTPUT);
  pinMode (mla, OUTPUT);
  pinMode (mlb, OUTPUT);
  digitalWrite (mla,LOW);
  digitalWrite (mlb,LOW);
  analogWrite (velmotor,vel);

}

void loop() {
  vel=255;
  analogWrite (velmotor,vel);

  digitalWrite (mla,LOW);
  digitalWrite (mlb,HIGH);
  delay(tmp);

  digitalWrite (mla,LOW);
  digitalWrite (mlb,LOW);
  delay(100);

  digitalWrite (mla,HIGH);
  digitalWrite (mlb,LOW);
  delay(tmp);
  
}
