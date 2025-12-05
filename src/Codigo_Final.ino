#include <Servo.h> // Inclui a biblioteca para controlar servo motores

// CONFIGURAÇÃO DO SERVO:
Servo servoVassoura; // Cria uma espécie de variável, chamada "servoVassoura" para controlar o motor
const int PINO_SERVO = 11; // Define que o fio de sinal do servo está ligado no pino digital 11

// SEUS ÂNGULOS:
const int ANGULO_BAIXO = 130;   // Define o ângulo (em graus) para a vassoura ficar no chão
const int ANGULO_ALTO = -360;   // Define o ângulo (em graus) para a vassoura levantar

// Variável para controlar o estado da vassoura e evitar comandos repetidos (0 = Baixo, 1 = Alto)
int estadoVassoura = -1; 

// ---------------------------------------------------

// SENSOR ULTRASSÔNICO:
const int PINO_SENSOR_ECHO = 13; // Define o pino Echo (receptor) do sensor no pino 13
const int PINO_SENSOR_TRIGGER = 12; // Define o pino Trigger (emissor) do sensor no pino 12

// DRIVER DE MOTOR(Ponte H L298N):
const int PIN_MOTOR_IN1 = 2; // Define o pino IN1 do motor A no pino 2
const int PIN_MOTOR_IN2 = 3; // Define o pino IN2 do motor A no pino 3
const int PIN_MOTOR_IN3 = 4; // Define o pino IN3 do motor B no pino 4
const int PIN_MOTOR_IN4 = 5; // Define o pino IN4 do motor B no pino 5

// CONSTANTES:
const int DISTANCIA_SEGURA = 40; // Define a distância mínima (40cm) para detectar um obstáculo
const int PAUSA = 40; // Define o tempo de espera (40ms) entre as leituras do sensor

// DECLARAÇÃO DAS FUNÇÕES:
int ler_distancia(void);
void mover_frente(void);
void mover_tras(void);
void parar(void);
void levantar_vassoura(void); 
void abaixar_vassoura(void);  

// ---------------------------------------------------

void setup() {
  // CONFIGURAÇÃO DO SERVO MOTOR:
  // Começa levantando e JÁ DESLIGA pra não tremer na largada
  levantar_vassoura(); // Levanta a vassoura ao ligar (por segurança)

  // CONFIGURAÇÃO DO SENSOR ULTRASSÔNICO:
  pinMode(PINO_SENSOR_ECHO, INPUT); // Configura o Echo como ENTRADA (recebe os dados)
  pinMode(PINO_SENSOR_TRIGGER, OUTPUT); // Configura o Trigger como SAÍDA (envia os dados)
  digitalWrite(PINO_SENSOR_TRIGGER, LOW); // Garante que o Trigger comece desligado

  // CONFIGURAÇÃO DO DRIVER DE MOTOR:
  pinMode(PIN_MOTOR_IN1, OUTPUT); // Configura IN1 como saída
  pinMode(PIN_MOTOR_IN2, OUTPUT); // Configura IN2 como saída
  pinMode(PIN_MOTOR_IN3, OUTPUT); // Configura IN3 como saída
  pinMode(PIN_MOTOR_IN4, OUTPUT); // Configura IN4 como saída
  
  parar(); // Faz com que os motores comecem desligados
}

// ---------------------------------------------------

void loop() { // Tudo entre essa função void fica em Loop 
  int distancia = ler_distancia(); // Chama a função que mede a distância e armazena na variável 'distancia'

  // Estrutura de decisão: Verifica se a distância lida é válida (>0) e menor que a segura (40cm)
  if (distancia > 0 && distancia < DISTANCIA_SEGURA){ // Se houver um obstáculo
    
    parar(); // Para os motores imediatamente e já levanta a vassoura
    delay(200); // Aguarda 200ms para transicionar de ação

    mover_tras(); // Move o carrinho para trás
    delay(800); // Fica dando ré por 0,8 segundo
    
    parar(); // Para os motores
    delay(500);

    // Lógica para decidir para qual lado girar: millis() retorna o tempo que o Arduino está ligado, e "% 2" verifica se esse número é par ou ímpar:
    bool par = (millis() % 2 == 0) ? true : false;
    
    if(par){ // Se o tempo for par
      // Gira o motor 1 para frente, e o motor 2 fica desligado (curva para a esquerda)
      digitalWrite(PIN_MOTOR_IN1, HIGH);
      digitalWrite(PIN_MOTOR_IN2, LOW);
      digitalWrite(PIN_MOTOR_IN3, LOW); 
      digitalWrite(PIN_MOTOR_IN4, LOW);
    } else { // Se o tempo for ímpar
      // Gira o motor 3 para frente, e o motor 4 fica desligado (curva para a direita)
      digitalWrite(PIN_MOTOR_IN1, LOW); 
      digitalWrite(PIN_MOTOR_IN2, LOW);
      digitalWrite(PIN_MOTOR_IN3, HIGH);
      digitalWrite(PIN_MOTOR_IN4, LOW);
    }

    delay(600); // Mantém o carrinho girando por 600ms
    
    parar(); // Para
    delay(200); 
    
  } else { // Se houve caminho livre, sem obstáculos 

    mover_frente(); // Retorna a função de entrar para frente, e deixa a vassoura abaixada.
  }

  delay(PAUSA); // Aguarda o tempo de 40ms(nomeado em "PAUSA") de leitura entre os dados do sensor ultrassônico
}

// ---------------------------------------------------
// FUNÇÕES AUXILIARES:
// ---------------------------------------------------

int ler_distancia(void){ // Lê a distância com o sensor e envia um pulso ultrassônico
  digitalWrite(PINO_SENSOR_TRIGGER,HIGH); // Liga o trigger do sensor
  delayMicroseconds(10); // Aciona-o por 10 microssegundos
  digitalWrite(PINO_SENSOR_TRIGGER,LOW); // Desliga-o
  
  // Mede quanto tempo o som demorou para ir e voltar(ms), com timeout para não travar
  long duracao = pulseIn(PINO_SENSOR_ECHO, HIGH, 25000);
  
  if (duracao == 0) return 100; // Se der erro de leitura, assume livre
  return duracao / 58; // Divide por 58(conversão para cm)
}

void mover_frente(void){ // Função que move o carrinho para a frente
  abaixar_vassoura(); // Abaixa a vassoura
  
  digitalWrite(PIN_MOTOR_IN1, HIGH); // Motor A (frente) ligado
  digitalWrite(PIN_MOTOR_IN2, LOW); // Motor A (trás) desligado
  digitalWrite(PIN_MOTOR_IN3, HIGH); // Motor B (frente) ligado
  digitalWrite(PIN_MOTOR_IN4, LOW); // Motor B (trás) desligado
}

void mover_tras(void){ // Função que move o carrinho para trás
  levantar_vassoura(); // Levanta a vassoura
  
  digitalWrite(PIN_MOTOR_IN1, LOW); // Motor A (frente) desligado
  digitalWrite(PIN_MOTOR_IN2, HIGH); // Motor A (trás) ligado
  digitalWrite(PIN_MOTOR_IN3, LOW); // Motor B (frente) desligado
  digitalWrite(PIN_MOTOR_IN4, HIGH); // Motor B (trás) ligado
}

void parar(void){ // Função que para todos os motores
  levantar_vassoura(); // Levanta a vassoura
  
  // Desliga todos os pinos dos motores
  digitalWrite(PIN_MOTOR_IN1, LOW);
  digitalWrite(PIN_MOTOR_IN2, LOW);
  digitalWrite(PIN_MOTOR_IN3, LOW);
  digitalWrite(PIN_MOTOR_IN4, LOW);
}

// ---------------------------------------------------
// FUNÇÕES PARA A VASSOURA (COM CORREÇÃO DE TREMEDEIRA)
// ---------------------------------------------------

void levantar_vassoura(){
  // Só executa se a vassoura NÃO estiver levantada
  if (estadoVassoura != 1) {
    servoVassoura.attach(PINO_SERVO); // Conecta o servo ao pino físico
    servoVassoura.write(ANGULO_ALTO); // Envia o comando do ângulo
    delay(500); 
    
    // Desliga o motor lá no alto.
    servoVassoura.detach(); 
    
    estadoVassoura = 1; 
  }
}

void abaixar_vassoura(){
  // Só executa se a vassoura NÃO estiver abaixada
  if (estadoVassoura != 0) {
    servoVassoura.attach(PINO_SERVO); // Conecta o servo ao pino físico
    servoVassoura.write(ANGULO_BAIXO); // Envia o comando de ângulo
    delay(500); // Espera chegar
    servoVassoura.detach(); // Desliga o motor no chão para parar tremedeira
    estadoVassoura = 0; 
  }
}
