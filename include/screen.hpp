#include <U8g2lib.h>
#include <Wire.h>
class Screen{
    public:
        Screen():u8g2(U8G2_R0, /* clock=*/ 9, /* data=*/ 10, /* cs=*/ 0, /* dc=*/ 12, /* reset=*/ 11){
        };
        void setup(){
            u8g2.begin();
            //u8g2.setBusClock(200000);
            //u8g2.enableUTF8Print();		// 启用UTF8打印函数支持
            //u8g2.setFont(u8g2_font_wqy13_t_gb2312b);  // 使用 u8g2_font_wqy13_t_gb2312 字体渲染
            u8g2.setFontDirection(0);
        };
        void update(){
            u8g2.sendBuffer();
        };
        void clear(){
            u8g2.clearBuffer();
            u8g2.sendBuffer();
        };
        void add_text(String text,int y,int x){
            u8g2.setCursor(x, y);
            u8g2.print(text);
        };
        U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2;
        //U8G2_SH1106_128X64_NONAME_F_4W_SW_SPI u8g2;
        bool is_in_menu;
        bool is_in_option;
    private:
        int now_option;
        int quantity;
        
        
};
