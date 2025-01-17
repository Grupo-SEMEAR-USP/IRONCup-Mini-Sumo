
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
void MotorL(int pwm); // left motor / motor esquerdo / motor izquierdo
void MotorR(int pwm); // right motor / motor direito / motor derecho
int readDIP(); // read DIP switch / ler chave DIP / leer el interruptor DIP
/*******FUNCTIONS - END*******/
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
  pinMode(DIP1, INPUT_PULLUP);  // DO NOT CHANGE / NAO MUDAR / NO CAMBIAR
  pinMode(DIP2, INPUT_PULLUP);  // DO NOT CHANGE / NAO MUDAR / NO CAMBIAR
  pinMode(DIP3, INPUT_PULLUP);  // DO NOT CHANGE / NAO MUDAR / NO CAMBIAR
  pinMode(DIP4, INPUT_PULLUP);  // DO NOT CHANGE / NAO MUDAR / NO CAMBIAR
  
  // line sensor
  pinMode(lineL, INPUT); // DO NOT CHANGE / NAO MUDAR / NO CAMBIAR
  pinMode(lineR, INPUT); // DO NOT CHANGE / NAO MUDAR / NO CAMBIAR
 
  // distance sensor
  pinMode(distR, INPUT); // DO NOT CHANGE / NAO MUDAR / NO CAMBIAR
  pinMode(distL, INPUT); // DO NOT CHANGE / NAO MUDAR / NO CAMBIAR
 
  // micro-start
  pinMode(microST, INPUT); // DO NOT CHANGE / NAO MUDAR / NO CAMBIAR
  /****************PINOUT CONFIG - END***************/
 
  /***************INITIAL CONDITIONS*****************/
  digitalWrite(LED, LOW); // LED off / LED desligado / LED apagado 
  MotorL(0); // left motor stopped / motor esquerdo parado / motor izquierdo parado 
  MotorR(0); // right motor stopped / motor direito parado / motor derecho parado 
  /*************INITIAL CONDITIONS - END*************/
}

//Mudar a velocidade durante a investida
void loop() {

  int DIP;

  //Importante estar no loop da estratégia enquanto o microST estiver ativo
  while(digitalRead(microST))
  {    
    DIP = readDIP();

    //Inserir as estratégias de acordo com número do DIP
    switch(DIP)
    {
      case 0: e1_tornado(160);  //B3, A3/2/4 - 0000
              break;
  
      case 1: e1_tornado(255);  //B3, A3/2/4 - 0001
              break;

      case 2: e2paciencia(160); //A3 - 0010
              break;

      case 3: e2paciencia(255); //A3 - 0011
              break;
      
      case 4: e3_tempestade(160); // A3, B3 - 0100
              break;

      case 5: e3_tempestade(255); //A3, B3 - 0101
              break;
        
      case 6: e6comunzito(160); //A4 - 0110
              break;

      case 7: e6comunzito(255); //A4 - 0111
              break;
      
      case 8: e7_frontal(160);  //B3 - 1000
              break;

      case 9: e7_frontal(255); //B3 - 1001
              break;
			  
      case 10: e6comunzito_v2(160); //A4 - 1010
              break;

      case 11: e1_tornado_v2(160,1); //A1 - 1011
               break;

      case 12: e1_tornado_v2(160,-1); //A5 - 1100
               break;
    }
  }

  //Importante parar o robô
  MotorR(0);
  MotorL(0);
  
}


//Função que movimenta o robô num trajeto com oponentes
void e1_tornado(int pwm) 
{
  int linhaD = 0;
  int linhaE = 0;
  int iniE = 0;
  int iniD = 0;

  int sentido = 1;

  //Importante estar no loop da estratégia enquanto o microST estiver ativo
  while(digitalRead(microST))
  {
    estado_linha(&linhaD,&linhaE);
    estado_inimigos(&iniD, &iniE);
    
    //Verifica se o robô está na linha
    if( !linhaD && !linhaE)
    {
      //Preocupação aqui é o inimigo
      //Leu na esquerda, mas não na direita
      if(iniE && !iniD)
      {
        //Mover o robô para a esquerda
        girar_Horario_eixo_robo(-255);
      }
      else if(!iniE && iniD)
      {
        //Mover o robô para a direita
        girar_Horario_eixo_robo(255);
      }
      else if(iniE && iniD)
      {
        //Os dois sensores detectam o oponente, ir para frente
        movimentacao(255);
      }
      else
      {
        //Não há sinal do sensor de linha e nem do oponente
        //Ficar girando até encontrar o oponente
        girar_eixo_roda(pwm*sentido);
      }      
    }
    else //Algum sensor de linha está ativado
    {
      //Ré reta
      movimentacao(-255);
      delay(50);
      
      if(linhaD)
      {
        //Dar ré no anti-horario
        re_eixo_roda(-pwm);
        delay(51000/pwm);   //Cuidado: o robô pode cair ao fazer essa curva de 0,4 s

        //Alterar o giro frontal para o sentido anti horário
        sentido = -1;
      }
      else
      {
        //Dar ré no horario
        re_eixo_roda(pwm);
        delay(51000/pwm);

        //Alterar o giro frontal para o sentido horário
        sentido = 1;
      }
    }
  }
}

//Função que movimenta o robô num trajeto com oponentes
void e1_tornado_v2(int pwm, int sentido) 
{
  int linhaD = 0;
  int linhaE = 0;
  int iniE = 0;
  int iniD = 0;

  float z = 0.75; // raio de uns 40 cm para a curva

  //Importante estar no loop da estratégia enquanto o microST estiver ativo
  while(digitalRead(microST))
  {
    estado_linha(&linhaD,&linhaE);
    estado_inimigos(&iniD, &iniE);
    
    //Verifica se o robô está na linha
    if( !linhaD && !linhaE)
    {
      //Preocupação aqui é o inimigo
      //Leu na esquerda, mas não na direita
      if(iniE && !iniD)
      {
        //Mover o robô para a esquerda
        girar_eixo_roda(-255);
      }
      else if(!iniE && iniD)
      {
        //Mover o robô para a direita
        girar_eixo_roda(255);
      }
      else if(iniE && iniD)
      {
        //Os dois sensores detectam o oponente, ir para frente
        movimentacao(255);
      }
      else
      {
        //Não há sinal do sensor de linha e nem do oponente
        //Ficar girando até encontrar o oponente
        curva_arco(pwm*sentido, z);
      }      
    }
    else //Algum sensor de linha está ativado
    {
      //Ré reta
      movimentacao(-255);
      delay(40);
      
      if(linhaD)
      {
        //Dar ré no anti-horario
        re_eixo_roda(-pwm);
        delay(51000/pwm);   //Cuidado: o robô pode cair ao fazer essa curva de 0,4 s

        //Alterar o giro frontal para o sentido anti horário
        sentido = -1;
      }
      else
      {
        //Dar ré no horario
        re_eixo_roda(pwm);
        delay(51000/pwm);

        //Alterar o giro frontal para o sentido horário
        sentido = 1;
      }
    }
  }
}

//pwm > 0 Horário | pwm < 0 Anti-Horario 
// 0 < z < 1 - indica quão menos a outra roda irá andar
void curva_arco(int pwm, float z)
{
  if(pwm > 0)
  {
    MotorR((int) pwm*z);
    MotorL(pwm);
  }
  else
  {
    MotorR(-pwm);
    MotorL((int) pwm*(-z));  
  }
}

void e2paciencia(int pwm)
{
  int linhaD = 0;
  int linhaE = 0;
  int iniE = 0;
  int iniD = 0;
  int alt;

  //Importante estar no loop da estratégia enquanto o microST estiver ativo
  while(digitalRead(microST)){

    estado_linha(&linhaD,&linhaE);
    estado_inimigos(&iniD, &iniE);
    
    //Verifica se o robô está na linha
    if( !linhaD && !linhaE){//Procurando por inimigo
      if(iniE && !iniD){//Vendo inimigo no lado esquerdo, mas não lado direito
        //Mover o robô para a esquerda
        girar_Horario_eixo_robo(-pwm);
      }
      //Leu na direita, mas não na esquerda
      else if(!iniE && iniD){//Vendo inimigo no lado direito, mas não no lado esquerdo
        //Mover o robô para a direita
        girar_Horario_eixo_robo(pwm);
      }
 
      else if(iniE && iniD){
        //Os dois sensores detectam o oponente, ir para frente
        movimentacao(255);
      }
      
      else{
        //Não há sinal do sensor de linha e nem do oponente
        //Procurando de forma lenta o oponente e aumentando-a gradualmente
        for(alt = 75; alt <= 175; alt += 2.5){
        movimentacao(alt);
        delay(100);
        }
      }      
    }
    
    else{//Algum sensor de linha está ativado
      if(linhaD && linhaE){//ativou os 2 
        //Dar ré reta
        movimentacao(-pwm);
        delay(25500/pwm);

        //Ré girando
        girar_Horario_eixo_robo(pwm);
        delay(175);
      }
     
      //ativou o da direita
      else if(linhaD && !linhaE){
        //Dar ré no anti-horario
        re_eixo_roda(-pwm); //Verificar sinal
        delay(175);
      }
     
      //ativado da esquerda
      else{
        //Dar ré no horario
        re_eixo_roda(pwm);  //Verificar sinal
        delay(175);
      }
    }
  }
}


void e3_tempestade(int pwm)
{

  int linhaD = 0;
  int linhaE = 0;
  int iniE = 0;
  int iniD = 0;
  int estado = 0;
  int contador = 0;

  //Importante estar no loop da estratégia enquanto o microST estiver ativo
  while(digitalRead(microST))
  {
    estado_linha(&linhaD,&linhaE);
    estado_inimigos(&iniD, &iniE);

    //Verifica se o robô está na linha
    if( !linhaD && !linhaE)//robô não está na linha
    {
      //Tem inimigo na esquerda, mas não na direita
      if(iniE && !iniD)
      {
        //Mover o robô para a esquerda
        girar_Horario_eixo_robo(-pwm);
      }
      else if(!iniE && iniD)//Tem inimigo na direita mas não na esquerda
      {
        //Mover o robô para a direita
        girar_Horario_eixo_robo(pwm);
      }
      else if(iniE && iniD)
      {
        //Os dois sensores detectam o oponente, ir para frente
        movimentacao(255);
      }
      else
      {
        //Não há sinal do sensor de linha e nem do oponente
        //Dar dois piques frontais e girar
        if(estado == 0){
          movimentacao(pwm);//dá o primeiro pique
          delay(25500/pwm);
        }
        else if(estado == 1){
          movimentacao(0);//para
          delay(25500/pwm);
        }
         else if(estado == 2){
           movimentacao(pwm);//dá o segundo pique
           delay(25500/pwm);
        }
         else if(estado == 3){
          movimentacao(0);//para
          delay(25500/pwm);
         }
         else if(estado == 4){
           if(contador % 2 == 0){//alternar entre giro horário e anti horário
             contador = 0;
             girar_Horario_eixo_robo(pwm);//rotacionar um pouco no sentido horário pra tentar achar o inimigo
             delay(100);
           }
          else{
            contador = 1;
            girar_Horario_eixo_robo(-pwm);//rotacionar um pouco no sentido anti-horário pra tentar achar o inimigo
            delay(100);
          }
          contador++;
          }
         else{
          estado = -1; //botar o estado em -1 caso seja maior que 4, pq na próxima linha (estado++) volta ele pra 0
         }
          estado ++;
          
        
      }
      
      
    }
    else //Algum sensor de linha está ativado
    {
      
      if(linhaD && linhaE)
      {
        //Dar ré
        movimentacao(-pwm);
        delay(25500/pwm);  
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

void e6comunzito (int pwm){
  int linhaD = 0;
  int linhaE = 0;
  int iniE = 0;
  int iniD = 0;
  int sonin;

  //Importante estar no loop da estratégia enquanto o microST estiver ativo
  while(digitalRead(microST)){

    estado_linha(&linhaD,&linhaE);
    estado_inimigos(&iniD, &iniE);
    
    //Verifica se o robô está na linha
    if( !linhaD && !linhaE){
      //Preocupação aqui é o inimigo
      //Leu na esquerda, mas não na direita
      
      if(iniE && !iniD){
        //Mover o robô para a esquerda
        girar_Horario_eixo_robo(-pwm);
      }
      //Leu na direita, mas não na esquerda
      
      else if(!iniE && iniD){
        //Mover o robô para a direita
        girar_Horario_eixo_robo(pwm);
      }
 
      else if(iniE && iniD){
        //Os dois sensores detectam o oponente, ir para frente
        movimentacao(255);
      }
      
      else{
        //Não há sinal do sensor de linha e nem do oponente
        movimentacao(pwm);
      }      
    }
    
    else /*Algum sensor de linha está ativado*/{
      sonin = random(190,225);
      
      //Tranco para o carrinho n sair da arena
      movimentacao(-255);
      delay(10);
      
     //ativou os 2 
      if(linhaD && linhaE){
        //Dar ré reta
        movimentacao(-255);
        delay(80);

        //Ré girando
        re_eixo_roda(pwm);
        delay(sonin); 
      }
     
      //ativou o da direita
      else if(linhaD && !linhaE){
        //Dar ré no anti-horario
        re_eixo_roda(-pwm); //Verificar sinal
        delay(sonin);  
      }
     
      //ativado da esquerda
      else{
        //Dar ré no horario
        re_eixo_roda(pwm);  //Verificar sinal
        delay(sonin);
      }
    }
  }
}

void e6comunzito_v2 (int pwm){
  int linhaD = 0;
  int linhaE = 0;
  int iniE = 0;
  int iniD = 0;
  int sonin;

  //Importante estar no loop da estratégia enquanto o microST estiver ativo
  while(digitalRead(microST)){

    estado_linha(&linhaD,&linhaE);
    estado_inimigos(&iniD, &iniE);
    
    //Verifica se o robô está na linha
    if( !linhaD && !linhaE){
      //Preocupação aqui é o inimigo
      //Leu na esquerda, mas não na direita
      
      if(iniE && !iniD){
        //Mover o robô para a esquerda
        girar_eixo_roda(-255);
      }
      //Leu na direita, mas não na esquerda
      
      else if(!iniE && iniD){
        //Mover o robô para a direita
        girar_eixo_roda(255);
      }
 
      else if(iniE && iniD){
        //Os dois sensores detectam o oponente, ir para frente
        movimentacao(255);
      }
      
      else{
        //Não há sinal do sensor de linha e nem do oponente
        movimentacao(pwm);
      }      
    }
    
    else /*Algum sensor de linha está ativado*/{
      sonin = random(190,225);
      
      //Tranco para o carrinho n sair da arena
      movimentacao(-255);
      delay(10);
      
     //ativou os 2 
      if(linhaD && linhaE){
        //Dar ré reta
        movimentacao(-255);
        delay(80);

        //Ré girando
        re_eixo_roda(pwm);
        delay(sonin); 
      }
     
      //ativou o da direita
      else if(linhaD && !linhaE){
        //Dar ré no anti-horario
        re_eixo_roda(-pwm); //Verificar sinal
        delay(sonin);  
      }
     
      //ativado da esquerda
      else{
        //Dar ré no horario
        re_eixo_roda(pwm);  //Verificar sinal
        delay(sonin);
      }
    }
  }
}


void e7_frontal(int pwm){
 
  unsigned long previousMillisConfronto = 0;
  int linhaD = 0;
  int linhaE = 0;
  int iniE = 0;
  int iniD = 0;

  //Importante estar no loop da estratégia enquanto o microST estiver ativo
  while(digitalRead(microST))
  {
    estado_linha(&linhaD,&linhaE);
    estado_inimigos(&iniD, &iniE);
    
    //Verifica se o robô está na linha
    if( !linhaD && !linhaE)
    {
      //Preocupação aqui é o inimigo
      //Leu na esquerda, mas não na direita
      if(iniE && !iniD)
      {
        //Mover o robô para a esquerda
        girar_Horario_eixo_robo(-255);
      }
      else if(!iniE && iniD)
      {
        //Mover o robô para a direita
        girar_Horario_eixo_robo(255);
      }
      else if(iniE && iniD)
      {//os dois sensores detectam o oponente
        
        
        previousMillisConfronto = millis();
        while(millis() - previousMillisConfronto < 2000){//Quando tiver vendo o inimigo por mais de 2 segundos, provavelmente vai dar empate
          movimentacao(255);
          
        }
          re_eixo_roda(pwm);//dar uma ré
          delay(100);//Se for menos de 2s, ir pra cima com tudo

      }
      else
      {
        //Não há sinal do sensor de linha e nem do oponente
        movimentacao(pwm);
        
      }      
    }
    else //Algum sensor de linha está ativado
    {
      movimentacao(-255);
      delay(100);
      if(linhaD)
      {
        //Dar ré no anti-horario
        re_eixo_roda(-pwm);
        delay(51000/pwm);   //Cuidado: o robô pode cair ao fazer essa curva de 0,4 s
      }
      else
      {
        //Dar ré no horario
        re_eixo_roda(pwm);
        delay(51000/pwm);
      }
    }
  }
  
  }

void movimentacao(int pwm) // utiliza o pwm para escolher o sentido da movimenção
{  
  //movimenta para frente ou fica parado
  MotorR(pwm);
  MotorL(pwm);
}


//Verifica os sensores de distancia
void estado_inimigos(int *direita, int *esquerda)
{
    *esquerda = digitalRead(distL);
 
    *direita = digitalRead(distR);
}


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

//Gira no sentido horario 
void girar_Horario_eixo_robo(int pwm) // pwm > 0 Horário | pwm < 0 Anti Horario
{  
 if(pwm > 0) // se pwm for positivo, roda horario
  {
    MotorR(-pwm);
    MotorL(pwm);
  }
  else if(pwm < 0) // se pwm for negativo, roda anti-horario
  {
    MotorR(-pwm);
    MotorL(pwm);   
  }
}


//Gira para esquerda ou direira com eixo da roda
void girar_eixo_roda(int pwm) // pwm > 0 direita | pwm < 0 esquerda
{  
  if(pwm > 0) // se pwm for positivo, vai para direita com motorR fixo
  {
    MotorR(0);
    MotorL(pwm);
  }
  else if(pwm < 0) // se pwm for negativo, vai para esquerda com motorL fixo
  {
    MotorR(-pwm);
    MotorL(0);   
  }
}


//Gira para esquerda ou direira com eixo da roda
void re_eixo_roda(int pwm) // pwm > 0 sentido horario | pwm < 0 sentido anti-horario
{  
  if(pwm == 0)
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
