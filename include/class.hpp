#include <Arduino.h>
#define analogprecision 12
class Rocker{
    public:
        //X轴引脚,Y轴引脚,X轴方向(1,-1),Y轴方向(1,-1),死区(0-1)
        Rocker(uint8_t x_pin,uint8_t y_pin ,int8_t X_dir=1,int8_t Y_dir=1,float Dead_zone=0.02){
            X_pin = x_pin;
            Y_pin = y_pin;
            x_dir = X_dir;
            y_dir = Y_dir;
            dead_zone = Dead_zone;
        }
        //初始化
        void setup(){
            pinMode(X_pin,INPUT);
            pinMode(Y_pin,INPUT);
            //获得轴零点
            int temp_X,temp_Y;
            for (int i = 0; i < 10; i++)
            {
                temp_X += analogRead(X_pin);
                temp_Y += analogRead(Y_pin);
                delay(10);
            }
            x_zero_point = temp_X/10;
            y_zero_point = temp_Y/10;
            
        }
        //获取x引脚
        uint8_t get_x_pin(){
            return X_pin;
        }
        //获取y引脚
        uint8_t get_y_pin(){
            return Y_pin;
        }
        //更新和获取X轴值
        double get_x(){
            double temp = x_dir*double(analogRead(X_pin)-x_zero_point) / double(1<<analogprecision);
            if (abs(temp)>dead_zone){
            X= temp;
            }else{
                X=0;
            }
            return X;
        }
        //更新和获取Y轴值
        double get_y(){
            double temp = y_dir*double(analogRead(Y_pin)-y_zero_point)/double(1<<analogprecision);
            if (abs(temp)>dead_zone){
            Y= temp;
            }else{
                Y=0;
            }
            return Y;
        }
        ~Rocker(){

        };
        double X,Y;
    private:
        uint16_t x_zero_point,y_zero_point;
        float dead_zone;
        uint8_t X_pin,Y_pin;
        int8_t x_dir,y_dir;
};
//按键
class Button{
    public:
    Button(uint8_t Pin){
        pin = Pin;
    }
    ~Button(){

    };
    bool get_state(){
        return state;
    }
        
    void setup(){
        pinMode(pin,INPUT_PULLDOWN);
        state = digitalRead(pin);
    }
    void callback(){
        state = digitalRead(pin);
    }
    private:
        uint8_t pin;
        bool state;
};
//三档拨动开关
class Toggle_Switch{
    public:
        Toggle_Switch(uint8_t PinA,uint8_t PinB){
            pinA = PinA;
            pinB = PinB;
        }
        void setup(){
            pinMode(pinA,INPUT_PULLDOWN);
            pinMode(pinB,INPUT_PULLDOWN);
        }
        void callback(){
            state_A = digitalRead(pinA);
            state_B = digitalRead(pinB);
            if(state_A || state_B){
                state = -1*state_B+state_A;
            }else{
                state = 0;
            }
        }
        int get_state(){
            return state;
        }
    private:
        uint8_t pinA,pinB;
        bool state_A,state_B;
        int8_t state=0;
};
//旋钮
class Knob{
    public:
        Knob(uint8_t Pin){
            pin = Pin;
        }
        void setup(){
            pinMode(pin,INPUT);
        }
        double callback(){
            value = analogRead(pin);
            value = (1<<analogprecision)-1-value;
            value = value/(1<<analogprecision);
            return value;
        }
        double get_value(){
            return value;
        }

    
    private:
        double value;
        uint8_t pin;
        
};
//编码器旋钮
class Encoder_Konb{
    public:
        Encoder_Konb(uint8_t Sw_pin,uint8_t Dt_pin,uint8_t Clk_pin,int8_t Dir){
            sw_pin = Sw_pin;
            dt_pin = Dt_pin;
            clk_pin = Clk_pin;
            dir = Dir;
        }
        void setup(){
            //pinMode(sw_pin,INPUT_PULLUP);
            pinMode(dt_pin,INPUT_PULLDOWN);
            pinMode(clk_pin,INPUT_PULLDOWN);

        }
        void sw_callback(){
            sw_state = digitalRead(sw_pin);
        }
       void dt_callback(){

            if(digitalRead(dt_pin) == 0 && digitalRead(clk_pin) == 1){
                value += dir;
            }else if(digitalRead(dt_pin) == 1 && digitalRead(clk_pin) == 0){
                value -= dir;
            }

        }
        void clear(){
            value = 0;
        }
        int64_t get_value(){
            return value;
        }
    private:

        uint8_t sw_pin,dt_pin,clk_pin;
        int8_t dir;
        bool sw_state;
        int64_t value;
};

void pins_setup(){
    
}