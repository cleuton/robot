#include <NewPing.h>
#include <Wire.h>
#include <LSM303.h>

LSM303 compass;

// 6 - front sonar trig
// 7 - front sonar echo
// a2/16 - rear  sonar trig
// a3/17 - rear  sonar echo
// 2 - left  sonar trig
// 10 - left  sonar echo
// d4    - right sonar trig
// d5    - right sonar echo

#define front_trig 6
#define front_echo 7
#define rear_trig 16
#define rear_echo 17
#define left_trig 2
#define left_echo 10
#define right_trig 4
#define right_echo 5

#define SONAR_NUM 4      // Number of sensors.
#define MAX_DISTANCE 400 // Maximum distance (in cm) to ping.
#define contaLoop 10

NewPing sonar[SONAR_NUM] = {   // Sensor object array.
  NewPing(front_trig, front_echo, MAX_DISTANCE), // Each sensor"s trigger pin, echo pin, and max distance to ping.
  NewPing(rear_trig, rear_echo, MAX_DISTANCE),
  NewPing(left_trig, left_echo, MAX_DISTANCE),
  NewPing(right_trig, right_echo, MAX_DISTANCE)
};

#define sonar_frontal 0
#define sonar_trazeiro 1
#define sonar_esq 2
#define sonar_dir 3
#define velInicial 128
#define incVel 16

// Heading aumenta para a direita e diminui para a esquerda!
#define limiteHeading 0.10

// Velocidades atuais e heading fixo:
float heading = 0;
int velocidadeA = velInicial + 8;
int velocidadeB = velInicial;
int contaVezes = 0;

// Positivo: Ele está virando para a direita, corrigir o esquerdo
// Negativo: Ele está virando para a esquerda, corrigir o direito
float verificaHeading(float lastHeading) {
  float diferenca = lastHeading - heading;
  return diferenca;
}

// Corrige a trajetória de acordo com a diferença
void corrige(float diferenca) {
  if (contaVezes > 0) {
    return;
  }
  if (diferenca > 0) {
    // Acelerar esquerdo
    velocidadeB += incVel;
    velocidadeA -= incVel;
    Serial.print("velB: ");
    Serial.print(velocidadeB);
    Serial.print(" ");
    contaVezes = 0;
  }
  else if (diferenca < 0) {
    // Acelerar direito
    velocidadeA += incVel;
    velocidadeB -= incVel;
    Serial.print("velA: ");
    Serial.print(velocidadeA);
    Serial.print(" ");
    contaVezes = 0;
  }
  Serial.print("Corrigindo dif: ");
  Serial.println(diferenca);

  andar(velocidadeA, velocidadeB);

}

// Estabelece direção: Por enquanto, só frente
void direcao() {
  // Canal A
  digitalWrite(12, LOW); //Back = Forward o motor está ao contrário
  // Canal B
  digitalWrite(13, LOW);  //Establishes backward direction of Channel B
}

// acao = 1: freiar (HIGH), acao = 0: soltar (LOW)
void freios(int acao) {
  digitalWrite(9, acao);  // Canal A
  digitalWrite(8, acao);  // Canal B
}

void andar(int velocidadeA, int velocidadeB) {
  analogWrite(3, velocidadeA);   // Canal A
  analogWrite(11, velocidadeB);    // Canal B
}



void setup() {
  Serial.begin(115200);
  Wire.begin();
  compass.init();
  compass.enableDefault();
  compass.m_min = (LSM303::vector<int16_t>) {
    -32767, -32767, -32767
  };
  compass.m_max = (LSM303::vector<int16_t>) {
    +32767, +32767, +32767
  };

  // Motores:
  // Canal A: Motor direito (da frente do carro, que é onde está o Raspberry)
  pinMode(12, OUTPUT); //Initiates Motor Channel A pin
  pinMode(9, OUTPUT); //Initiates Brake Channel A pin
  // Canal B: Motor esquerdo
  pinMode(13, OUTPUT); //Initiates Motor Channel A pin
  pinMode(8, OUTPUT);  //Initiates Brake Channel A pin

  // Pega o Heding inicial:
  compass.read();
  heading = compass.heading();
  Serial.print("direcao inicial: ");
  Serial.println(heading);

  // Inicia um movimento reto baseado no heading atual:
  direcao();
  freios(LOW);
  andar(velocidadeA, velocidadeB);
}

void loop() {

  compass.read();
  float lastHeading = compass.heading();
  Serial.print("direcao Atual: ");
  Serial.println(lastHeading);
  Serial.print("Velocidades atuais(A B):");
  Serial.print(velocidadeA);
  Serial.print(", ");
  Serial.println(velocidadeB);

  float diferencaAtual = verificaHeading(lastHeading);

  Serial.print("dif atual: ");
  Serial.println(diferencaAtual);

  corrige(diferencaAtual);
  contaVezes = contaVezes + 1;
  if (contaVezes > contaLoop) {
    contaVezes = 0;
  }
  
}
