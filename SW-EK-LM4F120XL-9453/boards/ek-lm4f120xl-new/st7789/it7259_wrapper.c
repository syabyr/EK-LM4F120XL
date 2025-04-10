#include <string.h>
#include "i2c_wrapper.h"
#include "spi_wrapper.h"
#include "st7789v.h"
#include "it7259_wrapper.h"


void GPIODIntHandler(void)
{
    // Clear the interrupt
    //ROM_GPIOIntClear(GPIO_PORTD_BASE, GPIO_PIN_7);
    printf("0\r\n");
    // Read the interrupt status

}

void tp_init()
{
    //TP_INT:PD7
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    ROM_GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_7);
    // 设置低电平触发
    ROM_GPIOIntTypeSet(GPIO_PORTD_BASE, GPIO_PIN_7, GPIO_FALLING_EDGE);
    ROM_IntMasterEnable();
    ROM_IntEnable(INT_GPIOD);
    ROM_IntPrioritySet(INT_GPIOD, 0x00);
}

void get_device_name(uint8_t *recv, uint8_t len)
{
    i2cWriteByte(IT7259_I2C_ADDR, IT7259_CMD_BUFFER, 0x00);
    i2cReadBytes(IT7259_I2C_ADDR, IT7259_CMD_RESP_BUFFER, recv, len);
    //0a 49 54 45 37 32 35 39 32 30
    //ITE725920
}

void get_firmware_version(uint8_t *recv, uint8_t len)
{
    uint8_t subCmd[]={0x01,0x00};
    i2cWriteBytes(IT7259_I2C_ADDR,IT7259_CMD_BUFFER,subCmd,2);
    i2cReadBytes(IT7259_I2C_ADDR,IT7259_CMD_RESP_BUFFER,recv,len);
    //09 03 00 00 03 03 0d 00 07 30

}

void get_2d_resolutions(uint8_t *recv, uint8_t len)
{
    uint8_t subCmd[]={0x01,0x02};
    i2cWriteBytes(IT7259_I2C_ADDR,IT7259_CMD_BUFFER,subCmd,2);
    i2cReadBytes(IT7259_I2C_ADDR,IT7259_CMD_RESP_BUFFER,recv,len);
    //0c 00 ef 00 ef 00 40 0d 03 04 00 00
    //240x240 分辨率
    // scale 40
    //connection type 0d
    // stage a b c d: 03 04 00 00
}

void get_flash_size(uint8_t *recv, uint8_t len)
{
    uint8_t subCmd[]={0x01,0x03};
    i2cWriteBytes(IT7259_I2C_ADDR,IT7259_CMD_BUFFER,subCmd,2);
    i2cReadBytes(IT7259_I2C_ADDR,IT7259_CMD_RESP_BUFFER,recv,len);
    //03 00 b4 00
    // 大小:0xb400,46080字节
}

void get_interrupt_status(uint8_t *recv, uint8_t len)
{
    uint8_t subCmd[]={0x01,0x04};
    i2cWriteBytes(IT7259_I2C_ADDR,IT7259_CMD_BUFFER,subCmd,2);
    i2cReadBytes(IT7259_I2C_ADDR,IT7259_CMD_RESP_BUFFER,recv,len);
    //01 00
    //01: trigger enabled
    //00: Low level trigger
}

void get_gesture_information(uint8_t *recv, uint8_t len)
{
    uint8_t subCmd[]={0x01,0x05};
    i2cWriteBytes(IT7259_I2C_ADDR,IT7259_CMD_BUFFER,subCmd,2);
    i2cReadBytes(IT7259_I2C_ADDR,IT7259_CMD_RESP_BUFFER,recv,len);
    //03 00 00 00
    // 手势:0x00
    // 手势状态:0x00
    // 手势坐标:0x00 0x00
    // 手势坐标:0x00 0x00
}

void get_configure_version(uint8_t *recv, uint8_t len)
{
    uint8_t subCmd[]={0x01,0x06};
    i2cWriteBytes(IT7259_I2C_ADDR,IT7259_CMD_BUFFER,subCmd,2);
    i2cReadBytes(IT7259_I2C_ADDR,IT7259_CMD_RESP_BUFFER,recv,len);
    //07 31 a7 15 16 00 00 
}

//Get Cap Sensor Information
void read_cap_sensor_info()
{
    printf("read_cap_sensor_info:");
    uint8_t pointinf[16]={0xff};
    i2cReadBytes(IT7259_I2C_ADDR,IT7259_POINT_INF_BUFFER,pointinf,16);
    for(int i=0;i<16;i++)
    {
        printf("%02x ",pointinf[i]);
    }
    printf("x:%03d y:%03d\r\n",pointinf[2],pointinf[4]);
    printf("\r\n");
}


// interrupt set
void set_interrupt_mode() {

}

void check_info() {
    //test tp
    uint8_t recv[32] = {0xff};

    //Get Device Name(0x00)
    get_device_name(recv, 0x0A);
    printf("get Device name:\r\n");
    for(int i=0;i<0x0A;i++)
    {
        printf("%02x ",recv[i]);
    }
    printf("\r\n");

    // Firmware Information(0x01,0x00)
    memset(recv, 0xff, 0x0A);
    get_firmware_version(recv, 0x0A);
    printf("get Firmware version:\r\n");
    for(int i=0;i<0x0A;i++)
    {
        printf("%02x ",recv[i]);
    }
    printf("\r\n");
    

    // 2D Resolutions(0x01,0x02)
    memset(recv, 0xff, 0x0C);
    get_2d_resolutions(recv, 0x0C);
    printf("get 2D resolutions:\r\n");
    for(int i=0;i<0x0C;i++)
    {
        printf("%02x ",recv[i]);
    }
    printf("\r\n");
    
    memset(recv, 0xff, 0x02);
    get_interrupt_status(recv, 0x02);
    printf("get interrupt status:\r\n");
    for(int i=0;i<0x02;i++)
    {
        printf("%02x ",recv[i]);
    }
    printf("\r\n");

    memset(recv, 0xff, 0x04);
    get_gesture_information(recv, 0x04);
    printf("get gesture information:\r\n");
    for(int i=0;i<0x04;i++)
    {
        printf("%02x ",recv[i]);
    }
    printf("\r\n");
     
    memset(recv, 0xff, 0x04);

    get_flash_size(recv, 0x04);
    printf("get Flash size:\r\n");
    for(int i=0;i<0x04;i++)
    {
        printf("%02x ",recv[i]);
    }
    printf("\r\n");
    

    memset(recv, 0xff, 0x07);

    get_configure_version(recv, 0x07);
    printf("get configure version:\r\n");
    for(int i=0;i<0x07;i++)
    {
        printf("%02x ",recv[i]);
    }
    printf("\r\n");

}