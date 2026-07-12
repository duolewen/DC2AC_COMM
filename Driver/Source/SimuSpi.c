// 假设已定义GPIO操作宏
// #define LCD_CS_HIGH()    gpio_set(LCD_CS_PIN, 1)  // CS引脚置高
// #define LCD_CS_LOW()     gpio_set(LCD_CS_PIN, 0)
// #define LCD_RS_CMD()     gpio_set(LCD_DC_PIN, 0)  // DCX=0 -> 命令模式
// #define LCD_RS_DATA()    gpio_set(LCD_DC_PIN, 1)  // DCX=1 -> 数据模式
// #define LCD_WR_PULSE()   // WR引脚先高后低再高（参考后续描述）
// #define LCD_RD_PULSE()   // RD引脚先高后低再高

// 写命令（8-bit）
void lcd_write_cmd(uint8_t cmd) {
    LCD_CS_LOW();           // CS使能
    LCD_RS_CMD();           // DCX=0，进入命令模式
    LCD_WR_PULSE();         // WR上升沿锁存数据
    DATA_PORT = cmd;        // 将8位指令放到数据总线上
    LCD_WR_SET();
    LCD_WR_CLR();
    LCD_WR_SET();           // 产生WR上升沿
    LCD_CS_HIGH();          // CS失能
}

// 写数据（8-bit）
void lcd_write_data(uint8_t data) {
    LCD_CS_LOW();
    LCD_RS_DATA();          // DCX=1，进入数据模式
    DATA_PORT = data;       // 将8位参数放到数据总线上
    LCD_WR_PULSE();         // 产生WR上升沿锁存
    LCD_CS_HIGH();
}

// 读数据（用于读ID或GRAM回读）
uint8_t lcd_read_data(void) {
    uint8_t data;
    LCD_CS_LOW();
    LCD_RS_DATA();          // 数据模式
    LCD_RD_PULSE();         // RD低脉冲读取数据
    data = DATA_PORT_READ();// 读取8位数据线
    LCD_CS_HIGH();
    return data;
}



void SimuLcd_Write(uint8_t Flag,uint8_t Value)
{
	
}

HW_Reset();                

Delay(120);                

Write(Command , 0x11);     //Sleep Out

Delay(120);                

Write(Command , 0x36);     
Write(Parameter , 0x48);   

Write(Command , 0x3A);     //Interface Pixel Format
Write(Parameter , 0x77);   

Write(Command , 0xF0);     // Command Set Control
Write(Parameter , 0xC3);   

Write(Command , 0xF0);     // 
Write(Parameter , 0x96);   

Write(Command , 0xB4);     
Write(Parameter , 0x01);   //1-Dot INV

Write(Command , 0xE8);     
Write(Parameter , 0x40);
Write(Parameter , 0x8A);
Write(Parameter , 0x00);
Write(Parameter , 0x00);
Write(Parameter , 0x29);
Write(Parameter , 0x19);
Write(Parameter , 0xA5);
Write(Parameter , 0x33);

Write(Command , 0xB9);     
Write(Parameter , 0x02);
Write(Parameter , 0xE0);

Write(Command , 0xC0);     
Write(Parameter , 0x80);
Write(Parameter , 0x65);   //VGH=15V, VGL=-10V

Write(Command , 0xC1);     
Write(Parameter , 0x13);   //4.5V

Write(Command , 0xC2);     
Write(Parameter , 0xA7);

Write(Command , 0xC5);     //VCOM Control  
Write(Parameter , 0x22);   //1.15V

Write(Command , 0xE0);     //Positive Voltage Gamma Control
Write(Parameter , 0xF0);   
Write(Parameter , 0x0E);   
Write(Parameter , 0x14);   
Write(Parameter , 0x0E);   
Write(Parameter , 0x0F);   
Write(Parameter , 0x1A);   
Write(Parameter , 0x37);   
Write(Parameter , 0x44);   
Write(Parameter , 0x48);   
Write(Parameter , 0x29);   
Write(Parameter , 0x15);   
Write(Parameter , 0x13);   
Write(Parameter , 0x18);   
Write(Parameter , 0x1B);   

Write(Command , 0xE1);     //Negative Voltage Gamma Control
Write(Parameter , 0xF0);   
Write(Parameter , 0x0A);   
Write(Parameter , 0x10);   
Write(Parameter , 0x0A);   
Write(Parameter , 0x0B);   
Write(Parameter , 0x17);   
Write(Parameter , 0x37);   
Write(Parameter , 0x44);   
Write(Parameter , 0x48);   
Write(Parameter , 0x2C);   
Write(Parameter , 0x16);   
Write(Parameter , 0x15);   
Write(Parameter , 0x1B);   
Write(Parameter , 0x1F);   

Write(Command , 0xF0);     
Write(Parameter , 0x3C);   

Write(Command , 0xF0);     
Write(Parameter , 0x69);   

Delay(120);                // Delay 120ms

Write(Command , 0x29);     // Display ON
