const int encoderEsquerdo = 3;
const int encoderDireito = 2;
const int interrupt3 = 3;
int detectStateDir=0; // Variable for reading the encoder status
volatile int contaDir = 0;
volatile int contaEsq = 0;
  // Distance sensor
  int IRpin = 5;                                    // analog pin for reading the IR sensor


void setup() {
  //Setup Channel A
  pinMode(12, OUTPUT); //Initiates Motor Channel A pin
  pinMode(9, OUTPUT); //Initiates Brake Channel A pin
  
  //Setup Channel B
  pinMode(13, OUTPUT); //Initiates Motor Channel b pin
  pinMode(8, OUTPUT);  //Initiates Brake Channel b pin

  //Setup encoders
  pinMode(encoderEsquerdo, INPUT); 
  pinMode(encoderDireito, INPUT); 

  //Setup serial:   
  Serial.begin(9600);

  //Start engines:

  forward();
    
  delay(1000);
  contaEsq = 0;
  contaDir = 0;

  //Setup interrupts: 
  attachInterrupt(0,interruptEncoderDireito,CHANGE);
  attachInterrupt(1,interruptEncoderEsquerdo,CHANGE);

  
}

void interruptEncoderEsquerdo() {
  contaEsq++;
}

void interruptEncoderDireito() {
  contaDir++;
}

void forward() {
    pinMode(9, OUTPUT); //Initiates Brake Channel A pin
    pinMode(9, OUTPUT); //Initiates Brake Channel A pin
  
  digitalWrite(13, LOW);  //Establishes forward direction of Channel B
  digitalWrite(8, LOW);   //Disengage the Brake for Channel B
  analogWrite(11, 98);    //Spins the motor on Channel B at half speed  

  
  digitalWrite(12, LOW); //Establishes forward direction of Channel A
  digitalWrite(9, LOW);   //Disengage the Brake for Channel A
  analogWrite(3, 106);   //Roda o motor no canal A em meia velocidade
  
}

void turnRight() {
    pinMode(9, OUTPUT); //Initiates Brake Channel A pin
    pinMode(9, OUTPUT); //Initiates Brake Channel A pin
  
  digitalWrite(13, LOW);  //Establishes forward direction of Channel B
  digitalWrite(8, LOW);   //Disengage the Brake for Channel B
  analogWrite(11, 98);    //Spins the motor on Channel B at half speed  

  
  digitalWrite(12, HIGH); //Establishes forward direction of Channel A
  digitalWrite(9, LOW);   //Disengage the Brake for Channel A
  analogWrite(3, 106);   //Roda o motor no canal A em meia velocidade
  
}

void loop() {
  float volts = analogRead(IRpin)*0.0048828125;   // value from sensor * (5/1024) - if running 3.3.volts then change 5 to 3.3
  float distance = (65*pow(volts, -1.10)) / 10.0;          // worked out from graph 65 = theretical distance / (1/Volts)S - luckylarry.co.uk

  if(distance < 4.0) {
    turnRight();
    delay(500);
    forward();
  }
  Serial.println("D:");
  Serial.print(contaDir);
  Serial.print(" E:");
  Serial.print(contaEsq);
}
