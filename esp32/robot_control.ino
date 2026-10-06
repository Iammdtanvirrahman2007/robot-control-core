#include <Arduino.h>
#include <WiFi.h>

/*
  Robot Control Core ESP32 body firmware
  Protocol: one newline-terminated request per TCP connection on port 5000.
  Change WIFI_SSID/WIFI_PASSWORD and motor pins for your hardware.

  Motor wiring used here:
    Left PWM  GPIO25, Left DIR GPIO26
    Right PWM GPIO27, Right DIR GPIO14
    HC-SR04 TRIG GPIO5, ECHO GPIO18
*/
const char* WIFI_SSID = "YOUR_WIFI";
const char* WIFI_PASSWORD = "YOUR_PASSWORD";
const uint16_t PORT = 5000;

const int L_PWM=25, L_DIR=26, R_PWM=27, R_DIR=14;
const int TRIG=5, ECHO=18;

NetworkServer server(PORT);
int leftSpeed=0,rightSpeed=0;
unsigned long lastCommand=0;
const unsigned long COMMAND_TIMEOUT_MS=1500;

void motor(int lp,int rp){
  lp=constrain(lp,-100,100); rp=constrain(rp,-100,100);
  digitalWrite(L_DIR,lp<0?LOW:HIGH); digitalWrite(R_DIR,rp<0?LOW:HIGH);
  analogWrite(L_PWM,abs(lp)*255/100); analogWrite(R_PWM,abs(rp)*255/100);
  leftSpeed=lp; rightSpeed=rp;
}
void stopMotors(){motor(0,0);}
float distanceCm(){
  digitalWrite(TRIG,LOW); delayMicroseconds(2); digitalWrite(TRIG,HIGH); delayMicroseconds(10); digitalWrite(TRIG,LOW);
  long us=pulseIn(ECHO,HIGH,25000); return us?us*0.0343f/2.0f:999.0f;
}
void command(String c){
  c.trim(); c.toUpperCase();
  int v=70; int sp=c.indexOf(' '); if(sp>0)v=constrain(c.substring(sp+1).toInt(),0,100);
  String op=sp>0?c.substring(0,sp):c;
  if(op=="FORWARD"||op=="DRIVE_FORWARD"||op=="WALK_FORWARD"){motor(v,v);}
  else if(op=="BACKWARD"||op=="DRIVE_BACKWARD"||op=="WALK_BACKWARD"){motor(-v,-v);}
  else if(op=="LEFT"||op=="TURN_LEFT"){motor(-v,v);}
  else if(op=="RIGHT"||op=="TURN_RIGHT"){motor(v,-v);}
  else if(op=="STOP"||op=="SIT"||op=="LAND"||op=="E_STOP"||op=="EMERGENCY_STOP"){stopMotors();}
  else if(op=="SET_MOTOR_L"){motor(v,rightSpeed);}
  else if(op=="SET_MOTOR_R"){motor(leftSpeed,v);}
  lastCommand=millis();
}
String hardware(){
  return "{\"type\":\"wheeled\",\"label\":\"Wheeled Robot\",\"camera\":false,\"panel\":\"DRIVETRAIN\",\"sensors\":[{\"id\":\"front_distance\",\"name\":\"Front Distance\",\"unit\":\"cm\",\"min\":2,\"max\":400,\"kind\":\"n\"}],\"actuators\":[{\"id\":\"motor_l\",\"name\":\"Left Motor\",\"type\":\"motor\",\"min\":-100,\"max\":100,\"unit\":\"%\"},{\"id\":\"motor_r\",\"name\":\"Right Motor\",\"type\":\"motor\",\"min\":-100,\"max\":100,\"unit\":\"%\"}],\"controls\":[null,\"Forward\",null,\"Left\",\"Stop\",\"Right\",null,\"Back\",null],\"dpad\":1,\"api\":{\"Drive\":[\"drive_forward\",\"drive_backward\",\"turn_left\",\"turn_right\",\"stop\"],\"Sensors\":[\"front_distance\"],\"Motors\":[\"set_motor_l\",\"set_motor_r\"]},\"dec\":[\"front_distance\",25,\"TURN_RIGHT 50\",\"DRIVE_FORWARD 70\"],\"code\":\"while (robot.connected())\\n{\\n    if (robot.front_distance() < 25)\\n    {\\n        robot.stop();\\n        robot.turn_right(50);\\n    }\\n    else\\n    {\\n        robot.drive_forward(70);\\n    }\\n}\"}";
}
String telemetry(){
  float d=distanceCm();
  return "{\"sensors\":{\"front_distance\":"+String(d,1)+"},\"actuators\":{\"motor_l\":"+String(leftSpeed)+",\"motor_r\":"+String(rightSpeed)+"},\"battery\":100,\"temp\":"+String((float)temperatureRead(),1)+",\"latency\":0,\"camera\":null}";
}
void handle(NetworkClient& c){
  String req=c.readStringUntil('\n'); req.trim();
  if(req=="PING") c.println("PONG");
  else if(req=="GET_HARDWARE") c.println(hardware());
  else if(req=="GET_TELEMETRY") c.println(telemetry());
  else if(req.startsWith("COMMAND ")){command(req.substring(8));c.println("ACK");}
  else c.println("ERR");
  c.stop();
}
void setup(){
  Serial.begin(115200);
  pinMode(L_PWM,OUTPUT);pinMode(L_DIR,OUTPUT);pinMode(R_PWM,OUTPUT);pinMode(R_DIR,OUTPUT);
  pinMode(TRIG,OUTPUT);pinMode(ECHO,INPUT);stopMotors();
  WiFi.mode(WIFI_STA); WiFi.setHostname("robot-control-esp32"); WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
  while(WiFi.status()!=WL_CONNECTED){delay(400);Serial.print(".");}
  Serial.print("\nESP32 IP: ");Serial.println(WiFi.localIP());
  server.begin(); lastCommand=millis();
}
void loop(){
  if(millis()-lastCommand>COMMAND_TIMEOUT_MS) stopMotors();
  NetworkClient c=server.accept();
  if(c) handle(c);
}