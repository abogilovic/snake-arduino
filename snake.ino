const int rowsSize=4; const int colsSize=4;
const int msDelay=1;

bool clickedLeft=false;
bool clickedRight=false;
int leftButtonPin=2;
int rightButtonPin=3;
int rowPins[rowsSize]={11,10,9,8};
int colPins[colsSize]={4,5,6,7};
bool matrix[4][4]={{false,false,false,false}, {false,false,false,false}, {false,false,false,false}, {false,false,false,false}};

const int updateGameEvery=300;
unsigned long nextGameUpdate=0;
struct BoardPos{
  int x;
  int y;
};
int snakeLength=1;
BoardPos snake[rowsSize*colsSize]={{2,1}};
int moveDirection[2]={1,0};
bool gameRunning=true;
BoardPos treat={2, 3};
int treatBlinkCycle=4;
int ClinkTreat=0;

void SnakeGameUpdate(){
  int next_x=snake[0].x+moveDirection[0];
  int next_y=snake[0].y+moveDirection[1];
  if(next_x>3) next_x=0; if(next_x<0) next_x=3;
  if(next_y>3) next_y=0; if(next_y<0) next_y=3;
  bool ate_treat=false;
  if(next_x==treat.x && next_y==treat.y){
    ate_treat=true;
    snakeLength+=1;
    BoardPos treat_spaces[15];
    int possible_spaces=0;
    for(int i=0; i<rowsSize; i++){
      for(int j=0; j<colsSize; j++){
        if(!matrix[i][j]) treat_spaces[possible_spaces++]={i, j};
      }
    }
    treat=treat_spaces[random(0,possible_spaces)];
  }else matrix[treat.x][treat.y]=true;
  
  for(int i=snakeLength-1; i>=0; i--){
    if(i==snakeLength-1 && !ate_treat) matrix[snake[i].x][snake[i].y]=false;
    if(i==0){
      snake[0].x=next_x; snake[0].y=next_y;
    }else{
      snake[i].x=snake[i-1].x;
      snake[i].y=snake[i-1].y;
    }
  }

  for(int j=1; j<snakeLength; j++)
    if(next_x==snake[j].x && next_y==snake[j].y) gameRunning=false;
  
  for(int i=0; i<snakeLength; i++)
    matrix[snake[i].x][snake[i].y]=true;
}

void setup() {
  randomSeed(analogRead(0));
  pinMode(leftButtonPin, INPUT);
  pinMode(rightButtonPin, INPUT);
  for(int i=0; i<rowsSize; i++){
    pinMode(rowPins[i], OUTPUT);
    pinMode(colPins[i], OUTPUT);
  }
  Serial.begin(9600);
}

void loop() {
  CheckInput();
  if(millis()>=nextGameUpdate && gameRunning){
    nextGameUpdate=millis()+updateGameEvery;
    SnakeGameUpdate();
  }
  WriteScreen();
}

void CheckInput(){
  if(digitalRead(leftButtonPin)==HIGH){
    if(!clickedLeft){
      if(moveDirection[0]!=0){
        moveDirection[1]=moveDirection[0];
        moveDirection[0]=0;
      }
      else if(moveDirection[1]!=0){
        moveDirection[0]=-1*moveDirection[1];
        moveDirection[1]=0;
      }
      clickedLeft=true;
    }
  }else clickedLeft=false;

  if(digitalRead(rightButtonPin)==HIGH){
    if(!clickedRight){
      if(moveDirection[0]!=0){
        moveDirection[1]=-1*moveDirection[0];
        moveDirection[0]=0;
      }
      else if(moveDirection[1]!=0){
        moveDirection[0]=moveDirection[1];
        moveDirection[1]=0;
      }
      clickedRight=true;
    }
  }else clickedRight=false;
}

void WriteScreen(){
  for(int i=0; i<rowsSize; i++){
    digitalWrite(rowPins[i], HIGH);
    for(int j=0; j<colsSize; j++){
      if(matrix[i][j]){
        if(i==treat.x && j==treat.y){
          if(ClinkTreat==treatBlinkCycle){
            digitalWrite(colPins[j], HIGH);
            delay(msDelay);
            digitalWrite(colPins[j], LOW);
            ClinkTreat=0;
          }else ClinkTreat+=1;
        }else{
          digitalWrite(colPins[j], HIGH);
          delay(msDelay);
          digitalWrite(colPins[j], LOW);
        }
      }else delay(msDelay);
    }
    digitalWrite(rowPins[i], LOW);
  }
}
