// SETUP DA PONTE H (PINAGEM)
// R: TRAÇÃO
// L: DESENROLA
int Renable = 2;
int Lenable = 3;
int Rpwm = 11;  
int Lpwm = 12;  

// BOTÕES (PINAGEM)
int Rbt = 4;  // TRAÇÃO
int Lbt = 5;  // DESENROLA
int Sbt = 6;  // PARADA

// VARIÁVEIS DE PROCESSO
int flag = 0;
int motorVal = 0;
int countTracao = 0;
int countDesenrola = 0;


/*
  Para os pinos INPUT, para definir como pull down (ler 5V ao pressionar), precisa definir como input pinMode(Rbt, INPUT) e na linha seguinte escrever digitalWrite(Rbt, LOW);
  Exemplo:
    Ao invés de usar:
      pinMode(botao, INPUT_PULL_DOWN); (antigo)
    Agora é assim:
      pinMode(botao, INPUT);
      digitalWrite(botao, LOW);
  
  Se quiser definir como PULL UP e ler 0V ao pressionar, precisa de um digitalWrite(Rbt, HIGH);
*/

void setup() 
{
  Serial.begin(9600);
  pinMode(Rpwm, OUTPUT);
  pinMode(Lpwm, OUTPUT);
  pinMode(Renable, OUTPUT);
  pinMode(Lenable, OUTPUT);
  pinMode(Rbt, INPUT);
  pinMode(Lbt, INPUT);
  pinMode(Sbt, INPUT);
  digitalWrite(Rbt, LOW);
  digitalWrite(Lbt, LOW);
  digitalWrite(Sbt, LOW);
  digitalWrite(Lenable, HIGH);
  digitalWrite(Renable, HIGH);
  flag = 0;  // inicialmente máquina parada
  delay(5000);  // delay de 10s para inicializar
}

// flag = 0 máquina parada
// flag = 1 sistema recebeu comando para tracionar
// flag = 2 sistema recebeu comando para desenrolar
// flag = 3 máquina em operação (estado não diretamente acionado)

void loop() 
{

  // botão de tração pressionado
  if(digitalRead(Rbt)){
    flag = 1;
  }

  // botão de desenrolar pressionado
  if(digitalRead(Lbt)){
    flag = 2;
  }

  if(digitalRead(Sbt)){
    flag = 0;
  }

  // máquina parada
  if(flag == 0){
    if(countTracao > 0){
      countTracao -= 1;
      delay(25);
      motorVal = int(countTracao*255/100);
      analogWrite(Rpwm, motorVal);
    }

    if(countDesenrola > 0){
      countDesenrola -= 1;
      delay(25);
      
      motorVal = int(countDesenrola*255/100);
      analogWrite(Lpwm, motorVal);
    }
  }

  // iniciando rampa de tração (countTracao < 50 evita que o botão seja pressionado consecutivamente)
  if(flag == 1 && countTracao < 50){
    if(countDesenrola > 0){
      countDesenrola -= 1;
      delay(25);
      motorVal = int(countDesenrola*255/100);
      analogWrite(Lpwm, motorVal);
    }
    countTracao += 1;
    delay(25);
    motorVal = int(countTracao*255/100);
    analogWrite(Rpwm, motorVal);
    Serial.println(countTracao);
    Serial.println(countDesenrola);
    if(countTracao == 50){
      flag = 3;
    }
  }

  // iniciando rampa de desenrolamento
  if(flag == 2 && countDesenrola < 50){
    if(countTracao > 0) {
      countTracao -= 1;
      delay(25);
      motorVal = int(countTracao*255/100);
      analogWrite(Rpwm, motorVal);
    }
    countDesenrola += 1;
    delay(25);
    motorVal = int(countDesenrola*255/100);
    analogWrite(Lpwm, motorVal);
    Serial.println(countTracao);
    Serial.println(countDesenrola);
    if(countDesenrola == 50){
      flag = 3;
    }
  }

  // a máquina estar em estado de flag 3 significa que não alteraremos a 
  // escrita PWM. 

}