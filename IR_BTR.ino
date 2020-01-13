#include <IRremote.h>

//#define SERIAL

#define RECV_PIN  2

#define MOTOR_1_N_1 5 //left
#define MOTOR_1_N_2 6
#define MOTOR_2_N_1 10 //rigth
#define MOTOR_2_N_2 11
#define MOTOR_PWM_MAX_VALUE 255


IRrecv irrecv(RECV_PIN);

decode_results results;
int p,val;

char sign(int n){
  if(n<0){
    return -1;
  }
  return 1;
}
int MOTOR_MAX(int n){
  if(abs(n)>MOTOR_PWM_MAX_VALUE){
    n=MOTOR_PWM_MAX_VALUE * sign(n);
  }
  return n;
}

void MOTOR_1(int n){
  n= MOTOR_MAX(n);
  if(n>=0){
    analogWrite(MOTOR_1_N_1, n);
    analogWrite(MOTOR_1_N_2, 0);
    return ;
  }
  else{
    analogWrite(MOTOR_1_N_1, 0);
    analogWrite(MOTOR_1_N_2, -n);
    return ;
  }
}
void MOTOR_2(int n){
  n= MOTOR_MAX(n);
  if(n>=0){
    analogWrite(MOTOR_2_N_1, n);
    analogWrite(MOTOR_2_N_2, 0);
    return ;
  }
  else{
    analogWrite(MOTOR_2_N_1, 0);
    analogWrite(MOTOR_2_N_2, -n);
    return ;
  }
}

void MOTOR_Rote(int forth, int left){
  left= left*sign(forth);
  MOTOR_1(forth -left);
  MOTOR_2(forth +left);
  
}


void setup()
{
#ifdef SERIAL
  Serial.begin(9600);
#endif
  // In case the interrupt driver crashes on setup, give a clue
  // to the user what's going on.
//  Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the receiver
//  Serial.println("Enabled IRin");
  p=0;
}

void loop() {
  if (irrecv.decode(&results)) {
    val=results.value;
//    Serial.print(results.value, HEX);
//    Serial.print("  ");
//    Serial.println(p, DEC);
    p=0;
    switch (val){
      case 0x20DF00FF : //forth
        MOTOR_Rote(255, 0);
#ifdef SERIAL
//        Serial.println("forth");
#endif
        break;

      case 0x20DFE01F : //forth left
        MOTOR_Rote(255, 255);
#ifdef SERIAL
//        Serial.println("forth left");
#endif
        break;
      case 0x20DF609F : //forth right
        MOTOR_Rote(255, -255);
#ifdef SERIAL
  //      Serial.println("forth right");
#endif
        break;

      case 0x20DFC03F : //left
        MOTOR_Rote(0, 255);
#ifdef SERIAL
    //    Serial.println("left");
#endif
        break;
      case 0x20DF40BF : //right
        MOTOR_Rote(0, -255);
#ifdef SERIAL
//  /      Serial.println("right");
#endif
        break;
 
      case 0x20DF807F : //back
        MOTOR_Rote(-255, 0);
#ifdef SERIAL
//     /   Serial.println("back");
#endif
        break;

      case 0x20DF02FD : //back left
        MOTOR_Rote(-255, 255);
#ifdef SERIAL
//   /     Serial.println("back left");
#endif
        break;
      case 0x20DF827D : //back right
        MOTOR_Rote(-255, -255);
#ifdef SERIAL
// /       Serial.println("back right");
#endif
        break;

      case 0xFFFFFFFF : //repet
#ifdef SERIAL
//    /    Serial.println("repet");
#endif
        break;
      default:
        MOTOR_Rote(0, 0);
#ifdef SERIAL
// /       Serial.println("nod use");
#endif
        break;
    }
    irrecv.resume(); // Receive the next value
  }
  else{
    p++;
    if (p>2){
      p=4;
      MOTOR_Rote(0, 0);
    }
  }
  delay(100);
}
