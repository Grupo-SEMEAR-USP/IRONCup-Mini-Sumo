/*******PINOUT DEFINES*********/
// it is not recommended to make changes
// nao e recomendado que se faca alteracoes
// no se recomienda hacer cambios
 
// LED
#define LED 6
 
// left motor
#define pwmL 9
#define leftMotor1 7
#define leftMotor2 8
 
// right motor
#define pwmR 3
#define rightMotor1 5
#define rightMotor2 4
 
// DIP switch
#define DIP1 10
#define DIP2 11
#define DIP3 12
#define DIP4 13
 
// Robocore's line sensor
#define lineL A0
#define lineR A1
 
// Jsumo's distance sensor
#define distL A2
#define distR A3
 
// Jsumo's micro-start
#define microST 2
/*******PINOUT DEFINES - END*********/

/*******FUNCTIONS*******/
void MotorL(int pwm); // motor esquerdo
void MotorR(int pwm); // motor direito
int readDIP(); //        ler chave DIP
/*******FUNCTIONS - END*******/

boolean start = false;

void setup() {
 
  /****************PINOUT CONFIG****************/
  // OUTPUTS
  pinMode(LED, OUTPUT);         // led
 
  // right motor
  pinMode(pwmR, OUTPUT);        // right motor power
  pinMode(rightMotor1, OUTPUT); // right motor dir.
  pinMode(rightMotor2, OUTPUT); // right motor dir.
 
  // left motor
  pinMode(pwmL, OUTPUT);        // left motor power
  pinMode(leftMotor1, OUTPUT);  // left motor dir.
  pinMode(leftMotor2, OUTPUT);  // left motor dir.
 
  // INPUTS: DO NOT CHANGE / NAO MUDAR / NO CAMBIAR
  // DIP switch
  pinMode(DIP1, INPUT_PULLUP);  // NAO MUDAR
  pinMode(DIP2, INPUT_PULLUP);  // NAO MUDAR
  pinMode(DIP3, INPUT_PULLUP);  // NAO MUDAR
  pinMode(DIP4, INPUT_PULLUP);  // NAO MUDAR
  
  // line sensor
  pinMode(lineL, INPUT); // NAO MUDAR / DIGITAL 
  pinMode(lineR, INPUT); // NAO MUDAR / DIGITAL
 
  // distance sensor
  pinMode(distR, INPUT); // NAO MUDAR / DIGITAL
  pinMode(distL, INPUT); // NAO MUDAR / DIGITAL
 
  // micro-start
  pinMode(microST, INPUT); // NAO MUDAR
  /****************PINOUT CONFIG - END***************/
 
  /***************INITIAL CONDITIONS*****************/
  digitalWrite(LED, LOW); // LED desligado
  MotorL(0); //              motor esquerdo parado 
  MotorR(0); //              motor direito parado
  /*************INITIAL CONDITIONS - END*************/
}

//Gira no sentido horario 
void girar_Horario_eixo_robo(int pwm) // pwm > 0 Horário | pwm < 0 Anti Horario
{  
  
  if(pwm = 0)
  {
    // algo se colocar 0

  }
  else if(pwm > 0) // se pwm for positivo, roda horario
  {
    MotorR(-pwm);

    MotorL(pwm);
  }
  else if(pwm<0) // se pwm for negativo, roda anti-horario
  {
    MotorR(pwm);

    MotorL(-pwm);   
  }
}

//Gira para esquerda ou direira com eixo da roda
void girar_eixo_roda(int pwm) // pwm > 0 direita | pwm < 0 esquerda
{  
  if(pwm = 0)
  {
    // algo se colocar 0

  }
  else if(pwm > 0) // se pwm for positivo, vai para direita com motorR fixo
  {
    MotorR(0);

    MotorL(pwm);
  }
  else if(pwm<0) // se pwm for negativo, vai para direita com motorL fixo
  {
    MotorR(pwm);

    MotorL(0);   
  }
}

//Função que verifica o estado dos sensores de linha 
void estado_linha(int *direita, int *esquerda)
{
    *(direita) = digitalRead(lineR);
 
    *(esquerda) = digitalRead(lineL);
}

//Verifica os sensores de distancia
void estado_inimigos(int *esquerda, int *direita)
{
    *esquerda = digitalRead(distL);
 
    *direita = digitalRead(distR);
}

void loop() {

  // Qual o sinal inicial do microST? O botão seta ele constantemente para HIGH? 
  while(digitalRead(microST))
  {
    
  }
 
}

void movimentacao(int pwm) // utiliza o pwm para escolher o sentido da movimenção
{  
  //fica parado
  if(pwm=0)
  {
    // algo se colocar 0
  }
  //movimenta para frente
  else if(pwm>0) // se pwm for positivo, vai para frente
  {
    MotorR(pwm);
    MotorL(pwm);
  }
  //movimenta para tras
  else if(pwm<0) // se pwm for negativo, vai para tras
  {
    MotorR(-pwm);
    MotorL(-pwm);
  }
}
 
/**LEFT MOTOR CONTROL / CONTROLE DO MOTOR ESQUERDO / CONTROL DEL MOTOR IZQUIERDO**/
// pwm = 0 -> stopped / parado / parado
// 0<pwm<=255 -> forward / para frente / seguir adelante
// -255<=pwm<0 -> backward / para tras / seguir espalda

void MotorL(int pwm){
  // leftMotor1=0 and leftMotor2=0 -> stopped / parado / parado 
  // leftMotor1=0 and leftMotor2=1 -> moves forward / avanca / avanzar
  // leftMotor1=1 and leftMotor2=0 -> moves back / recua / retrocede
  // leftMotor1=1 and leftMotor2=1 -> stopped (braked) / parado (travado) / parado (frenado)
 
  if(pwm==0){
    digitalWrite(leftMotor1, LOW);
    digitalWrite(leftMotor2, LOW);
  }
  else if(pwm<0)
  {
    analogWrite(pwmL, -pwm);
    digitalWrite(leftMotor1, HIGH);
    digitalWrite(leftMotor2, LOW);
  }
  else
  {
    analogWrite(pwmL, pwm);
    digitalWrite(leftMotor1, LOW);
    digitalWrite(leftMotor2, HIGH);
  }
}
 
/**RIGHT MOTOR CONTROL / CONTROLE DO MOTOR DIREITO / CONTROL DEL MOTOR DERECHO**/
// pwm = 0 -> stopped / parado / parado
// 0<pwm<=255 -> forward / frente / adelante
// -255<=pwm<0 -> backward / tras / espalda
void MotorR(int pwm){
  // rightMotor1=0 and rightMotor2=0 -> stopped / parado / parado 
  // rightMotor1=0 and rightMotor2=1 -> moves forward / avanca / avanzar
  // rightMotor1=1 and rightMotor2=0 -> moves back / recua / retrocede
  // rightMotor1=1 and rightMotor2=1 -> stopped (braked) / parado (travado) / parado (frenado)
 
  if(pwm==0){
    digitalWrite(rightMotor1, LOW);
    digitalWrite(rightMotor2, LOW);
  }
  else if(pwm<0)
  {
    analogWrite(pwmR, -pwm);
    digitalWrite(rightMotor1, HIGH);
    digitalWrite(rightMotor2, LOW);
  }
  else
  {
    analogWrite(pwmR, pwm);
    digitalWrite(rightMotor1, LOW);
    digitalWrite(rightMotor2, HIGH);
  }
}
 
/** read DIP switch / ler chave DIP / leer el interruptor DIP **/
// returns a value between 0 and 15
// retorna um valor entre 0 e 15
// devuelve un valor entre 0 y 15
int readDIP(){
  int n=0;
  if(digitalRead(DIP4)==HIGH)
    n=1;
  if(digitalRead(DIP3)==HIGH)
    n|= (1<<1);
  if(digitalRead(DIP2)==HIGH)
    n|= (1<<2);
  if(digitalRead(DIP1)==HIGH)
    n|= (1<<3);
}
