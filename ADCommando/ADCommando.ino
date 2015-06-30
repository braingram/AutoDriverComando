#include <AutoDriver.h>
#include<comando.h>
//#define DEBUG 1
Comando com = Comando(Serial);
EchoProtocol echo = EchoProtocol(com);
CommandProtocol cmd =  CommandProtocol(com);

class AutoReel: public AutoDriver {
  private:
    int max_speed;
    int acc;
    int dec;
    int k;
    int ms;
    boolean low_speed;
    int _bp;
    
  public:
    AutoReel(int CSPin, int resetPin, int busyPin)
    : AutoDriver(CSPin, resetPin, busyPin),
      max_speed(500),
      acc(500),
      dec(500),
      k(41),
      ms(7),
      low_speed(false),
      _bp(busyPin)
      {}
    void configure () {
 
      resetDev();
      configSyncPin(_bp, 0);
      configStepMode(ms);
      setMaxSpeed(max_speed);
      //setFullSpeed(600);
      setLoSpdOpt(low_speed);
      setAcc(acc);
      setDec(dec);
      setSlewRate(SR_530V_us);
      setOCThreshold(OC_6000mA);//OC_750mA
      setPWMFreq(PWM_DIV_2, PWM_MUL_2);
      setOCShutdown(OC_SD_DISABLE);
      setVoltageComp(VS_COMP_DISABLE);
      setSwitchMode(SW_USER);
      setOscMode(INT_16MHZ_OSCOUT_16MHZ);
      setAccKVAL(k);
      setDecKVAL(k);
      setRunKVAL(k);
      setHoldKVAL(k);
      #ifdef DEBUG
        Serial.println(getDecKVAL());
        Serial.println(getRunKVAL());
        Serial.println(getHoldKVAL());
        Serial.println(getMaxSpeed());
        Serial.println(getFullSpeed());
        Serial.println(getAcc());
        Serial.println(getMinSpeed());
        Serial.println(getStepMode());
        Serial.println(getLoSpdOpt());
        Serial.println(getOCThreshold());
        Serial.println(getPWMFreqDivisor());
        Serial.println(getPWMFreqMultiplier());
        Serial.println(getVoltageComp());
        Serial.println(getSwitchMode());
      #endif
    }
    
    void set_max_speed(int v) {
      #ifdef DEBUG
        Serial.print("~set max_speed to ");
        Serial.println(v, DEC);
      #endif
      max_speed = v;
      //configure();
    }
    
    int get_max_speed() {
      return max_speed;
    }
    
    void set_acc(int v) {
      #ifdef DEBUG
        Serial.print("~set acc to ");
        Serial.println(v, DEC);
      #endif
      acc = v;
      //configure();
    }
    
    int get_acc() {
      return acc;
    }
    
    void set_dec(int v) {
      #ifdef DEBUG
        Serial.print("~set dec to ");
        Serial.println(v, DEC);
      #endif
      dec = v;
      //configure();
    }
    
    int get_dec() {
      return dec;
    }
    
    void set_k(int v) {
      #ifdef DEBUG
        Serial.print("~set k to ");
        Serial.println(v, DEC);
      #endif
      k = v;
      //configure();
    }
    
    int get_k() {
      return k;
    }
    
    void set_ms(int v) {
      #ifdef DEBUG
        Serial.print("~set ms to ");
        Serial.println(v, DEC);
      #endif
      ms = v;
      //configure();
    };
    
    int get_ms() {
      return ms;
    };
    
    void set_low_speed(boolean v) {
      #ifdef DEBUG
        Serial.print("~set low_speed to ");
        Serial.println(v, DEC);
      #endif
      low_speed = v;
      //configure();
    };
    
    boolean get_low_speed() {
      return low_speed;
    };
};

AutoReel boards[] = {
  AutoReel(10,6,5),
};

byte nboards = 0;
byte board_index = 0;

void conf(CommandProtocol* cmd){ //configure
  echo.send_message(com.get_bytes(), com.get_n_bytes());
  int board = cmd->get_arg<int>();
  boards[board].configure();
};//0

void soft_stop(CommandProtocol* cmd){ //stop
  echo.send_message(com.get_bytes(), com.get_n_bytes());
  int board = cmd->get_arg<int>();
  boards[board].softStop();
};//1

void hard_stop(CommandProtocol* cmd){ //hard stop
  echo.send_message(com.get_bytes(), com.get_n_bytes());
  int board = cmd->get_arg<int>();
  boards[board].hardStop();
};//2

void rel(CommandProtocol* cmd){ //release
  echo.send_message(com.get_bytes(), com.get_n_bytes());
  int board = cmd->get_arg<int>();
  boards[board].softHiZ();
};//3

void max_sp(CommandProtocol* cmd){ //set max speed
  echo.send_message(com.get_bytes(), com.get_n_bytes());
  int board = cmd->get_arg<int>();
  int ex = cmd->get_arg<int>();
  int sp = cmd->get_arg<int>();
  boards[board].set_max_speed(sp);
};//4

void set_accel(CommandProtocol* cmd){ //set acc/dec
  echo.send_message(com.get_bytes(), com.get_n_bytes());
  int board = cmd->get_arg<int>();
  int ex = cmd->get_arg<int>();
  int ac = cmd->get_arg<int>();
  boards[board].set_acc(ac);
  boards[board].set_dec(ac);
};//5

void set_current(CommandProtocol* cmd){ //set k value [0-255]
  echo.send_message(com.get_bytes(), com.get_n_bytes());
  int board = cmd->get_arg<int>();
  int ex = cmd->get_arg<int>();
  int new_k = cmd->get_arg<int>();
  boards[board].set_k(new_k);
};//6

void set_micro(CommandProtocol* cmd){ //set microstepping vals [0-7]
  echo.send_message(com.get_bytes(), com.get_n_bytes());
  int board = cmd->get_arg<int>();
  int ex = cmd->get_arg<int>();
  int new_ms = cmd->get_arg<int>();
  boards[board].set_ms(new_ms);
};//7

void low_speed(CommandProtocol* cmd){ //set low speed mode true or false
  echo.send_message(com.get_bytes(), com.get_n_bytes());
  int board = cmd->get_arg<int>();
  int ex = cmd->get_arg<int>();
  bool is_on = cmd->get_arg<bool>();
  boards[board].set_low_speed(is_on);
  boards[board].configure();
};//8

void is_moving(CommandProtocol* cmd){ //returns a bool if the board is moving
  echo.send_message(com.get_bytes(), com.get_n_bytes());
  int board = cmd->get_arg<int>();
  bool is_moving = boards[board].busyCheck();
  //echo.send_message(is_moving);
};//9

void wait(CommandProtocol* cmd){ //holds until the board is done moving
  echo.send_message(com.get_bytes(), com.get_n_bytes());
  int board = cmd->get_arg<int>();
  while(boards[board].busyCheck());
};//10

void rot(CommandProtocol* cmd){ //rotates int dir int steps_per_sec
  //echo.send_message(com.get_bytes(), com.get_n_bytes());
  int board = cmd->get_arg<int>();
  int ex = cmd->get_arg<int>();
  int dir = cmd->get_arg<int>();
  int ex2 = cmd->get_arg<int>();
  int sps = cmd->get_arg<int>();
  
  
  Serial.println(board);
  Serial.println(ex);
  Serial.println(dir);
  Serial.println(ex2);
  Serial.println(sps);
  Serial.println(boards[board].get_ms());
  Serial.println(STEP_FS);
  boards[board].run(dir,sps);
};//11

void move_steps(CommandProtocol* cmd){ //move int dir int num_steps
  echo.send_message(com.get_bytes(), com.get_n_bytes());
  int board = cmd->get_arg<int>();
  int ex = cmd->get_arg<int>();
  int dir = cmd->get_arg<int>();
  ex = cmd->get_arg<int>();
  int num_steps = cmd->get_arg<int>();
  boards[board].move(dir,num_steps);
};//12

void query(CommandProtocol* cmd){//returns all vital values
  echo.send_message(com.get_bytes(),com.get_n_bytes());
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  nboards = sizeof(boards) / sizeof(AutoReel);
  for (int i=0; i < nboards; i++) {
    boards[i].configure();
  };
  // register protocols
  com.register_protocol(0,echo);
  com.register_protocol(1,cmd);
  // register command callbacks
  cmd.register_callback(0,conf);
  cmd.register_callback(1,soft_stop);
  cmd.register_callback(2,hard_stop);
  cmd.register_callback(3,rel);
  cmd.register_callback(4,max_sp);
  cmd.register_callback(5,set_accel);
  cmd.register_callback(6,set_current);
  cmd.register_callback(7,set_micro);
  cmd.register_callback(8,low_speed);
  cmd.register_callback(9,is_moving);
  cmd.register_callback(10,wait);
  cmd.register_callback(11,rot);
  cmd.register_callback(12,move_steps);
}

void loop() {
  // put your main code here, to run repeatedly:
  com.handle_stream();
}
