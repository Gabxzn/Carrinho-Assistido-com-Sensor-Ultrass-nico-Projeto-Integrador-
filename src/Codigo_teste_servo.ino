//Código teste para antes de implementarmos a vassoura (somente testando o servo junto com o carrinho)
#include <Servo.h> // BIBLIOTECA DO SERVO

// --- CONFIGURAÇÃO DO SERVO ---
Servo servoVassoura; 
const int PINO_SERVO = 11; 
const int ANGULO_BAIXO = 5;   // Vassoura no chao (varrendo)
const int ANGULO_ALTO = 110;   // Vassoura levantada (protegida)

// ---------------------------------------------------

// declaracao dos pinos do sensor ultrassonico
const int PINO_SENSOR_ECHO = 13;
const int PINO_SENSOR_TRIGGER = 12;

// declaracao dos pinos do driver de motor
const int PIN_MOTOR_IN1 = 2;
const int PIN_MOTOR_IN2 = 3;
const int PIN_MOTOR_IN3 = 4;
const int PIN_MOTOR_IN4 = 5;

// declaracao das constantes auxiliares para controlar os motores
const int DISTANCIA_SEGURA = 25; // [cm]
const int PAUSA = 100; // [ms]

// ---------------------------------------------------

int ler_distancia(void);
void mover_frente(void);
void mover_tras(void);
void girar_esquerda(void); // Nova função de giro forte
void girar_direita(void);  // Nova função de giro forte
void parar(void);
void levantar_vassoura(void); 
void abaixar_vassoura(void);  

// ---------------------------------------------------

void setup() {
  // Inicializa o gerador de números aleatórios usando o ruído de uma porta analógica vazia
  randomSeed(analogRead(A0));

  // Configura o servo motor
  servoVassoura.attach(PINO_SERVO);
  levantar_vassoura(); // Começa com a vassoura levantada por segurança

  // configura os pinos do sensor ultrassonico
  pinMode(PINO_SENSOR_ECHO, INPUT); 
  pinMode(PINO_SENSOR_TRIGGER, OUTPUT); 
  digitalWrite(PINO_SENSOR_TRIGGER, LOW); 

  // configura os pinos do driver de motor
  pinMode(PIN_MOTOR_IN1, OUTPUT); 
  pinMode(PIN_MOTOR_IN2, OUTPUT); 
  pinMode(PIN_MOTOR_IN3, OUTPUT); 
  pinMode(PIN_MOTOR_IN4, OUTPUT); 
  
  parar(); 
}

// ---------------------------------------------------

void loop() {
  // le a distancia
  int distancia = ler_distancia();

  // verifica se ha um obstaculo na frente
  if(distancia < DISTANCIA_SEGURA){
    // 1. OBSTACULO DETECTADO
    
    parar(); 
    delay(200); // Pequena pausa

    // move o robo para tras 
    mover_tras();
    delay(800); // Ré um pouco mais longa para garantir espaço
    parar();
    delay(500);

    // --- NOVA LÓGICA DE GIRO (CORREÇÃO DO LOOP) ---
    
    // 1. Escolhe aleatoriamente esquerda (0) ou direita (1)
    int direcao = random(0, 2); 
    
    // 2. Sorteia um tempo de giro aleatório entre 500ms e 1000ms
    // Isso faz com que o ângulo varie sempre, evitando ficar preso
    int tempo_giro = random(500, 1000); 

    if(direcao == 0){
      girar_esquerda();
    } else {
      girar_direita();
    }

    // Fica girando pelo tempo sorteado
    delay(tempo_giro);
    
    // Para e estabiliza antes de voltar a andar
    parar();
    delay(500);
    
  } else { // senao (CAMINHO LIVRE)
    // move o robo para a frente e ABAIXA A VASSOURA
    mover_frente();
  }

  // pausa para a proxima leitura
  delay(PAUSA);
}

// ---------------------------------------------------
// FUNÇÕES DE CONTROLE
// ---------------------------------------------------

int ler_distancia(void){
  digitalWrite(PINO_SENSOR_TRIGGER,HIGH);
  delayMicroseconds(10);
  digitalWrite(PINO_SENSOR_TRIGGER,LOW);
  return pulseIn(PINO_SENSOR_ECHO, HIGH) / 58; // [cm]
}

void mover_frente(void){
  abaixar_vassoura(); 
  // Motor 1 Frente, Motor 2 Frente
  digitalWrite(PIN_MOTOR_IN1, HIGH);
  digitalWrite(PIN_MOTOR_IN2, LOW);
  digitalWrite(PIN_MOTOR_IN3, HIGH);
  digitalWrite(PIN_MOTOR_IN4, LOW);
}

void mover_tras(void){
  levantar_vassoura(); 
  // Motor 1 Trás, Motor 2 Trás
  digitalWrite(PIN_MOTOR_IN1, LOW);
  digitalWrite(PIN_MOTOR_IN2, HIGH);
  digitalWrite(PIN_MOTOR_IN3, LOW);
  digitalWrite(PIN_MOTOR_IN4, HIGH);
}

// --- NOVAS FUNÇÕES DE GIRO NO EIXO ---
// Nota: Se o robô girar para o lado errado do comando, inverta o conteúdo das funções

void girar_direita(void){
  levantar_vassoura(); // Garante vassoura em cima
  // Para girar no eixo: Um motor vai para frente, o outro para trás
  // Motor 1 (Esquerda) -> FRENTE
  digitalWrite(PIN_MOTOR_IN1, HIGH);
  digitalWrite(PIN_MOTOR_IN2, LOW);
  // Motor 2 (Direita) -> TRÁS
  digitalWrite(PIN_MOTOR_IN3, LOW);
  digitalWrite(PIN_MOTOR_IN4, HIGH);
}

void girar_esquerda(void){
  levantar_vassoura(); // Garante vassoura em cima
  // Motor 1 (Esquerda) -> TRÁS
  digitalWrite(PIN_MOTOR_IN1, LOW);
  digitalWrite(PIN_MOTOR_IN2, HIGH);
  // Motor 2 (Direita) -> FRENTE
  digitalWrite(PIN_MOTOR_IN3, HIGH);
  digitalWrite(PIN_MOTOR_IN4, LOW);
}

void parar(void){
  levantar_vassoura(); 
  digitalWrite(PIN_MOTOR_IN1, LOW);
  digitalWrite(PIN_MOTOR_IN2, LOW);
  digitalWrite(PIN_MOTOR_IN3, LOW);
  digitalWrite(PIN_MOTOR_IN4, LOW);
}

// ---------------------------------------------------
// FUNÇÕES DO SERVO
// ---------------------------------------------------

void levantar_vassoura(){
  servoVassoura.write(ANGULO_ALTO);
}

void abaixar_vassoura(){
  servoVassoura.write(ANGULO_BAIXO);
}
