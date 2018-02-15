    /*
    -Arduino Position Encoder
    -Using a generic photo-interrupter
    -Basic Test Sketch 1 / June 2014
    -Tested at TechNode Protolabz
    -www.electroschematics.com/
    */
    const int encoderEsquerdo = 2; // input pin for the interrupter 
    const int encoderDireito = 4;
    int detectStateDir=0; // Variable for reading the encoder status
    int detectStateEsq=0;
    void setup()
    {
       pinMode(encoderEsquerdo, INPUT); 
       pinMode(encoderDireito, INPUT); 
       Serial.begin(9600);                             // start the serial port
    }
    void loop() {
       detectStateDir=digitalRead(encoderDireito);
       detectStateEsq=digitalRead(encoderEsquerdo);
       Serial.println("Direito: ");
       Serial.println(detectStateDir);
       Serial.println("esquerdo: ");
       Serial.println(detectStateEsq);
       delay(1000);                                     // arbitary wait time.

    }
