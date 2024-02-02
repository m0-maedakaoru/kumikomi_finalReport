int latch = 10; // RCLK
int clock = 11; // SRCLK
int data = 9; // SER
int time_d = 5; //delay
int dig1 = 6;
int dig2 = 7;
int dig3 = 8;
int dig4 = 5;

const int LED_PIN_B = 2;
const int LED_PIN_Y = 3;
const int LED_PIN_G = 4;

int BUTTON_PIN = 12;
unsigned long start_time;
unsigned long run_time;
int mode_flg = 0;   //フラグ
float count_s;
unsigned long now_time;

int keta1;
int keta2;
int keta3;
int keta4;

byte seven_leds[27] = {
  B11111100, //0
  B01100000, //1
  B11011010, //2
  B11110010, //3
  B01100110, //4
  B10110110, //5
  B10111110, //6
  B11100100, //7
  B11111110, //8
  B11110110, //9
  B00000001, //点,10
  B00000000, //全消灯,11
  B11101100, //A,12
  B00111110, //b,13
  B10011100, //C,14
  B01111010, //d,15
  B10011110, //E,16
  B10001110, //F,17
  B01101110, //H,18
  B01111000, //J,19
  B00011100, //L,20
  B00101010, //n,21
  B00111010, //o,22
  B11001110, //P,23
  B00001010, //r,24
  B01111100, //U,25
  B01110110, //y,26
};

void Display(int x){
  digitalWrite(latch,LOW);
  shiftOut(data,clock,LSBFIRST,seven_leds[x]);
  digitalWrite(latch,HIGH);
}

void setup() {
  pinMode(latch, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(data, OUTPUT);
  pinMode(dig1, OUTPUT);
  pinMode(dig2, OUTPUT);
  pinMode(dig3, OUTPUT);
  pinMode(dig4, OUTPUT);
  digitalWrite(dig1, HIGH);
  digitalWrite(dig2, HIGH);
  digitalWrite(dig3, HIGH);
  digitalWrite(dig4, HIGH);
  Display(11);  
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN_B, OUTPUT);
  pinMode(LED_PIN_Y, OUTPUT);
  pinMode(LED_PIN_G, OUTPUT);
  digitalWrite(LED_PIN_B, LOW);
  digitalWrite(LED_PIN_Y, LOW);
  digitalWrite(LED_PIN_G, LOW);
  Serial.begin(9600);
}

void flash(int x1, int x2, int x3, int x4){
  
    //dig1の表示
    delay(time_d);
    digitalWrite(dig1, LOW);
    digitalWrite(dig2, HIGH);
    digitalWrite(dig3, HIGH);
    digitalWrite(dig4, HIGH);
    Display(x1);
    delay(time_d);
    Display(11);
    
    //dig2の表示
    delay(time_d);
    digitalWrite(dig1, HIGH);
    digitalWrite(dig2, LOW);
    digitalWrite(dig3, HIGH);
    digitalWrite(dig4, HIGH);
    Display(x2);
    delay(time_d);
    Display(10);
    delay(time_d);
    Display(11);
  
    //dig3の表示
    delay(time_d);
    digitalWrite(dig1, HIGH);
    digitalWrite(dig2, HIGH);
    digitalWrite(dig3, LOW);
    digitalWrite(dig4, HIGH);
    Display(x3);
    delay(time_d);
    Display(11);
  
    //dig4の表示
    delay(time_d);
    digitalWrite(dig1, HIGH);
    digitalWrite(dig2, HIGH);
    digitalWrite(dig3, HIGH);
    digitalWrite(dig4, LOW);
    Display(x4);
    delay(time_d);
    Display(11);
}

void loop() {
  run_time = millis();
  
  digitalWrite(LED_PIN_B, HIGH);
  digitalWrite(LED_PIN_Y, LOW);
  digitalWrite(LED_PIN_G, LOW);

  //最初にボタンが押されたらSTART
  if(digitalRead(BUTTON_PIN) == LOW  && mode_flg == 0){
    digitalWrite(LED_PIN_B, LOW);
    digitalWrite(LED_PIN_Y, HIGH);
    digitalWrite(LED_PIN_G, LOW);
    Serial.println("START");  
    start_time =millis(); //STARTの時刻を代入
    Serial.print(" START_millis = ");
    Serial.println(start_time);  
    mode_flg =1;//フラグを１にする
    Serial.print("mode_flg=");  
    Serial.println(mode_flg);  //フラグの確認 
    delay(2000);     
  }
  
  if(digitalRead(BUTTON_PIN) == HIGH  && mode_flg == 1){
    digitalWrite(LED_PIN_B, LOW);    
    digitalWrite(LED_PIN_Y, HIGH);
    digitalWrite(LED_PIN_G, LOW);
    for(int i = 0; i<=5; i++){
      flash(i, i+1, i+2, i+3);
      delay(30);
    }
  }
  
  //2回目はSTOP、フラグが１のときに実行
  if(digitalRead(BUTTON_PIN) == LOW  && mode_flg == 1){
    digitalWrite(LED_PIN_B, LOW);
    digitalWrite(LED_PIN_Y, LOW);
    digitalWrite(LED_PIN_G, HIGH);

    run_time =  millis()- start_time; //時間を計算
    
    Serial.print(" STOP_millis = "); 
    Serial.println(run_time);
    count_s = (float)run_time/1000; //秒に変換
    Serial.print(" TIME= "); 
    Serial.println(count_s);

    //得られた時間の数字を桁ごとに取り出す
    if(run_time>=10000){
      keta1 = run_time/10000;
      keta2 = (run_time-keta1*10000)/1000;
      keta3 = ((run_time-keta1*10000)-keta2*1000)/100;
      keta4 = (((run_time-keta1*10000)-keta2*1000)-keta3*100)/10;
    }
    else if(10000>=run_time && run_time>=1000){
      keta1 = 0;
      keta2 = run_time/1000;
      keta3 = (run_time-keta2*1000)/100;
      keta4 = ((run_time-keta2*1000)-keta3*100)/10;
    }
    else if(1000>=run_time && run_time>=100){
      keta1 = 0;
      keta2 = 0;
      keta3 = run_time/100;
      keta4 = (run_time-keta3*100)/10;
    }
    else if(100>=run_time && run_time>=10){
      keta1 = 0;
      keta2 = 0;
      keta3 = 0;
      keta4 = run_time/10;
    }
    else{
      keta1 = 16; //E
      keta2 = 24; //r
      keta3 = 24; //r
      keta4 = 22; //o
      Serial.println("ERROR");
    }
    
    Serial.print("keta="); 
    Serial.print(keta1);
    Serial.print(", ");
    Serial.print(keta2);
    Serial.print(", ");   
    Serial.print(keta3);
    Serial.print(", ");
    Serial.println(keta4);

    for (int i=0; i<100; i++){
     flash(keta1, keta2, keta3, keta4);
     delay(5);
     Serial.print("a, "); //for文が回っているかの確認
    }

    Serial.println(" ");
    mode_flg =0;  //フラグを戻す
    Serial.print("mode_flg=");  
    Serial.println(mode_flg);  //フラグの確認 
    delay(5000);
  } 
}
