// O código abaixo é um dos nossos testes antes de chegar ao final. A principal diferença desse é que a vassoura "tremia" quanto dava ré/posicionada para o alto.
#include <Servo.h> // BIBLIOTECA DO SERVO

// --- CONFIGURAÇÃO DO SERVO ---
Servo servoVassoura; 
const int PINO_SERVO = 11; 

const int ANGULO_BAIXO = 130;   
const int ANGULO_ALTO = -360;   

int estadoVassoura = -1; 

// ---------------------------------------------------

const int PINO_SENSOR_ECHO = 13;
const int PINO_SENSOR_TRIGGER = 12;

const int PIN_MOTOR_IN1 = 2;
const int PIN_MOTOR_IN2 = 3;
const int PIN_MOTOR_IN3 = 4;
const int PIN_MOTOR_IN4 = 5;

// --- AJUSTES DE REFLEXO 
const int DISTANCIA_SEGURA = 40; // Aumentado para 40cm (Freia antes)
const int PAUSA = 40;            // Reduzido para 40ms (Olhos mais abertos)

// ---------------------------------------------------

int ler_distancia(void);
void mover_frente(void);
void mover_tras(void);
void parar(void);
void levantar_vassoura(void); 
void abaixar_vassoura(void);  

// ---------------------------------------------------

void setup() {
  levantar_vassoura(); 

  pinMode(PINO_SENSOR_ECHO, INPUT); 
  pinMode(PINO_SENSOR_TRIGGER, OUTPUT); 
  digitalWrite(PINO_SENSOR_TRIGGER, LOW); 

  pinMode(PIN_MOTOR_IN1, OUTPUT); 
  pinMode(PIN_MOTOR_IN2, OUTPUT); 
  pinMode(PIN_MOTOR_IN3, OUTPUT); 
  pinMode(PIN_MOTOR_IN4, OUTPUT); 
  
  parar(); 
}

// ---------------------------------------------------

void loop() {
  int distancia = ler_distancia();

  if (distancia > 0 && distancia < DISTANCIA_SEGURA){
    
    // --- OBSTÁCULO DETECTADO ---
    parar(); 
    delay(200); // Pausa breve para estabilizar

    mover_tras();
    delay(800); // Ré (ajuste se precisar ir mais longe)
    
    parar();
    delay(500);

    // LÓGICA DE GIRO (PAR/ÍMPAR)
    bool par = (millis() % 2 == 0) ? true : false;
    
    if(par){
      // Gira para um lado
      digitalWrite(PIN_MOTOR_IN1, HIGH);
      digitalWrite(PIN_MOTOR_IN2, LOW);
      digitalWrite(PIN_MOTOR_IN3, LOW); // Garante o outro lado parado
      digitalWrite(PIN_MOTOR_IN4, LOW);
    } else {
      // Gira para o outro
      digitalWrite(PIN_MOTOR_IN1, LOW); // Garante o outro lado parado
      digitalWrite(PIN_MOTOR_IN2, LOW);
      digitalWrite(PIN_MOTOR_IN3, HIGH);
      digitalWrite(PIN_MOTOR_IN4, LOW);
    }

    delay(600); // Tempo de giro (Aumentei um pouco para garantir a curva)
    
    parar();
    delay(200); // Estabiliza antes de voltar a andar
    
  } else { 
    // --- CAMINHO LIVRE ---
    mover_frente();
  }

  delay(PAUSA); // Pausa curta para reação rápida
}

// ---------------------------------------------------
// FUNÇÕES
// ---------------------------------------------------

int ler_distancia(void){
  digitalWrite(PINO_SENSOR_TRIGGER,HIGH);
  delayMicroseconds(10);
  digitalWrite(PINO_SENSOR_TRIGGER,LOW);
  

  long duracao = pulseIn(PINO_SENSOR_ECHO, HIGH, 25000);
  
  if (duracao == 0) return 100; // Se der timeout, considera livre 
  return duracao / 58; 
}

void mover_frente(void){
  abaixar_vassoura(); 
  digitalWrite(PIN_MOTOR_IN1, HIGH);
  digitalWrite(PIN_MOTOR_IN2, LOW);
  digitalWrite(PIN_MOTOR_IN3, HIGH);
  digitalWrite(PIN_MOTOR_IN4, LOW);
}

void mover_tras(void){
  levantar_vassoura(); 
  digitalWrite(PIN_MOTOR_IN1, LOW);
  digitalWrite(PIN_MOTOR_IN2, HIGH);
  digitalWrite(PIN_MOTOR_IN3, LOW);
  digitalWrite(PIN_MOTOR_IN4, HIGH);
}

void parar(void){
  levantar_vassoura(); 
  digitalWrite(PIN_MOTOR_IN1, LOW);
  digitalWrite(PIN_MOTOR_IN2, LOW);
  digitalWrite(PIN_MOTOR_IN3, LOW);
  digitalWrite(PIN_MOTOR_IN4, LOW);
}

// ---------------------------------------------------
// FUNÇÕES DA VASSOURA
// ---------------------------------------------------

void levantar_vassoura(){
  if (estadoVassoura != 1) {
    servoVassoura.attach(PINO_SERVO); 
    servoVassoura.write(ANGULO_ALTO); 
    delay(300); 
    // Mantém ligado para segurar
    estadoVassoura = 1; 
  }
}

void abaixar_vassoura(){
  if (estadoVassoura != 0) {
    servoVassoura.attach(PINO_SERVO); 
    servoVassoura.write(ANGULO_BAIXO); 
    delay(300); 
    servoVassoura.detach(); // Desliga para não tremer no chão
    estadoVassoura = 0; 
  }
}
