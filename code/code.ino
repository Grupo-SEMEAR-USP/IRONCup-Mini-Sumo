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


//A função deve receber os sensores dir e esq, não seria mais fácil manter as variáveis de sensor como globais, para acesso/consulta em todas as funções sem precisar recebê-las?
void trajeto_simples(int pwm) // < precisa receber &dir e &esq ? e se utilizar var global?
{
  int dir = 0;
  int esq = 0;
  const int tempo_base = 25500;
  //200 ms => 180 graus
	while(true)
	{
		movimentacao(pwm);	//vai para frente (em função de pwm)
		while(!(dir || esq)) //enquanto sensor dir e esq forem 0
		{
				estado_linha(&dir,&esq); //checa estado linha até mudar
		}		
		if(dir) //se sensor direito habilitado
		{
			re_eixo_roda(pwm); //gira sentido horario para trás um pouco (em função de pwm)
			delay(tempo_base/pwm);
			//***atentar para o fato de q a velocidade vai alterar 
			//***o tempo q precisa para o robo rodar ou ir para traz
			//***usar um delay inversamente proporcional a velocidade
		}
		else if(esq) //se sensor esquerdo habilitado
		{
			re_eixo_roda(-pwm);//gira sentido anti horario para trás um pouco (em função de pwm)
			delay(tempo_base/abs(pwm));//roda sentido horario um pouco	(em função de pwm)
		}	
	}
} //fim trajeto_simples


//Função que verifica o estado dos sensores de linha 
void estado_linha(int *direita, int *esquerda)
{
     if(analogRead(lineR) <= 740)
      *(direita) = 1;
     else
      *(direita) = 0;

     if(analogRead(lineL) <= 740)
      *(esquerda) = 1;
     else
      *(esquerda) = 0;
}

//Verifica os sensores de distancia
void estado_inimigos(int *direita, int *esquerda)
{
    *esquerda = digitalRead(distL);
 
    *direita = digitalRead(distR);
}

//Gira para esquerda ou direira com eixo da roda
void re_eixo_roda(int pwm) // pwm > 0 sentido horario | pwm < 0 sentido anti-horario
{  
  if(pwm = 0)
  {
    // algo se colocar 0

  }
  else if(pwm > 0) // se pwm for positivo, ré no sentido horario
  {
    MotorR(-pwm);

    MotorL(0);
  }
  else if(pwm<0) // se pwm for negativo, ré no sentido Anti-horario
  {
    MotorL(pwm);

    MotorR(0);   
  }
}

//Função que movimenta o robô num trajeto com oponentes
void trajeto_com_inimigo(int pwm) 
{
  int linhaD = 0;
  int linhaE = 0;
  int iniE = 0;
  int iniD = 0;

  while(true)
  {
    estado_linha(&linhaD,&linhaE);
    estado_inimigos(&iniD, &iniE);

    //Verifica se o robô está na linha
    if( !linhaD && !linhaE)
    {
      //Leu na esquerda, mas não na direita
      if(iniE && !iniD)
      {
        //Mover o robô para a esquerda
        girar_Horario_eixo_robo(-pwm);
      }
      else if(!iniE && iniD)
      {
        //Mover o robô para a direita
        girar_Horario_eixo_robo(pwm);
      }
      else if(iniE && iniD)
      {
        //Os dois sensores detectam o oponente, ir para frente
        movimentacao(pwm);
      }
      else
      {
        //Não há sinal do sensor de linha e nem do oponente
        //Ficar girando? Ou ir para frente?
        girar_Horario_eixo_robo(pwm);
      }
      
      
    }
    else //Algum sensor de linha está ativado
    {
      
      if(linhaD && linhaE)
      {
        //Dar ré
        movimentacao(-pwm);  
      }
      else if(linhaD && !linhaE)
      {
        //Dar ré no anti-horario
        re_eixo_roda(-pwm);
        delay(25500/pwm);
      }
      else
      {
        //Dar ré no horario
        re_eixo_roda(pwm);
        delay(25500/pwm);
      }
    }
  }
} 

void loop() {

  int DIP;
  
  // Qual o sinal inicial do microST? O botão seta ele constantemente para HIGH? 
  while(digitalRead(microST))
  {
    DIP = readDIP();

    //Inserir as estratégias de acordo com número do DIP
    switch(DIP)
    {
      case 0: teste(125);
              break;
  
      case 1: teste(255);
              break;

      case 2: trajeto_simples(125);
              break;

      case 3: trajeto_simples(255);
              break;
      
      case 4: trajeto_com_inimigo(125);
              break;

      case 5: trajeto_com_inimigo(255);
              break;
              
    }
  }
 
}

// Testes das funções base
void teste (int pwm)
{
  /* Testes de movimentacao*/

  //Anda pra frente
  movimentacao(pwm);
  delay(130000/pwm);

  //Para o movimento
  movimentacao(0);
  delay(200);

  //Anda para trás
  movimentacao(-pwm);
  delay(130000/(2*pwm));

  //Para o movimento
  movimentacao(0);
  delay(200);

  /* Testes de rotação eixo-robo*/

  //Rotaciona horário no eixo do robo
  girar_Horario_eixo_robo(pwm);
  delay(130000/pwm);

  //Para o movimento
  movimentacao(0);
  delay(200);

  //Rotaciona anti-horário no eixo do robo
  girar_Horario_eixo_robo(-pwm);
  delay(130000/pwm);

  //Para o movimento 
  movimentacao(0);
  delay(200);

  /* Testes de rotação eixo-roda*/

  //Rotaciona horário no eixo da roda
  girar_eixo_roda(pwm);
  delay(130000/pwm);

  //Para o movimento
  movimentacao(0);
  delay(200);

  //Rotaciona anti-horário no eixo da roda
  girar_eixo_roda(-pwm);
  delay(130000/pwm);

  //Para o movimento
  movimentacao(0);
  delay(200);

  /* Testes de ré eixo-roda*/

  //Da ré rotacionando horário no eixo-roda
  re_eixo_roda(pwm);
  delay(130000/pwm);

  //Para o movimento
  movimentacao(0);
  delay(200);

  //Da ré rotacionando anti-horário no eixo-roda
  re_eixo_roda(-pwm);
  delay(130000/pwm);

  //Para o movimento
  movimentacao(0);
  delay(200);
}

void movimentacao(int pwm) // utiliza o pwm para escolher o sentido da movimenção
{  
  //movimenta para frente ou fica parado
  MotorR(pwm);
  MotorL(pwm);
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

  return n;
}
