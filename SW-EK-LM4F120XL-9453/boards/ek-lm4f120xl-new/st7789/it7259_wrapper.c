#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "i2c_wrapper.h"
#include "it7259_wrapper.h"

// 触摸中断标志
volatile uint8_t g_touch_interrupt_flag = 0;

// GPIOD中断处理函数
void GPIODIntHandler(void)
{
    uint32_t ui32Status = GPIOPinIntStatus(GPIO_PORTD_BASE, true);

    // 检查是否是PD7的中断
    if(ui32Status & GPIO_PIN_7)
    {
        // 设置触摸中断标志
        g_touch_interrupt_flag = 1;

        // 清除中断标志
        GPIOPinIntClear(GPIO_PORTD_BASE, GPIO_PIN_7);
    }
}

// 检查芯片是否忙
static void CTP_CheckBusy(void)
{
    uint8_t query;
    uint16_t i = 0;

    do
    {
        i2cReadBytes(IT7269_ADDR, QUERY_BUFFER_INDEX, &query, 1);
        i++;
        if(i > 500)
            return;

    } while((query & QUERY_BUSY));
}

// 设置IT7269中断功能
// enable: 1=使能中断, 0=禁用中断
// type: 0=下降沿触发, 1=上升沿触发（根据芯片手册调整）
void IT7269_SetInterrupt(uint8_t enable, uint8_t type)
{
    uint8_t cmd[4] = {IT7269_CMD_SET_INTERRUPT, 0x04, enable, type};

    CTP_CheckBusy();
    i2cWriteBytes(IT7269_ADDR, COMMAND_BUFFER_INDEX, cmd, 4);

    // 等待命令执行完成
    ROM_SysCtlDelay(ROM_SysCtlClockGet() / 300); // 等待10ms
}

// 获取中断标志
uint8_t IT7269_GetInterruptFlag(void)
{
    return g_touch_interrupt_flag;
}

// 清除中断标志
void IT7269_ClearInterruptFlag(void)
{
    g_touch_interrupt_flag = 0;
}

// 检查是否有新的触摸点
uint8_t IT7269_CheckNewPoint(void)
{
    uint8_t query;

    i2cReadBytes(IT7269_ADDR, QUERY_BUFFER_INDEX, &query, 1);

    if (query & QUERY_POINT)
        return 1;

    return 0;
}

void tp_init()
{
    // TP_INT: PD7 先仅作为输入，不配置中断
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    // 配置PD7为输入，上拉
    ROM_GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_7);
    ROM_GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_7,
                         GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // 初始化完成，等待芯片稳定
    ROM_SysCtlDelay(ROM_SysCtlClockGet() / 3); // 等待1秒
}

void IT7269_ReadTouch(uint16_t *pX, uint16_t *pY, uint8_t *pPressed)
{
    uint8_t pointdata[14];

    *pPressed = 0;
    *pX = 0;
    *pY = 0;

    // 先检查是否有新点
    if(!IT7269_CheckNewPoint())
        return;

    // 读取14个字节的点数据
    i2cReadBytes(IT7269_ADDR, POINT_BUFFER_INDEX, pointdata, 14);

    // 检查是否是报点数据
    if(pointdata[0] & POINT_FLAG)
    {
        *pPressed = 1;
        // 解析坐标 (来自官方驱动)
        *pX = ((pointdata[3] & 0x0F) << 8) + pointdata[2];
        *pY = ((pointdata[3] & 0xF0) << 4) + pointdata[4];
    }
}

// 读取手势
IT7269_Gesture_t IT7269_ReadGesture(void)
{
    IT7269_Gesture_t gesture = {0, 0};
    uint8_t pointdata[14];

    // 读取14个字节的点数据（先读后判断，避免重复读取）
    i2cReadBytes(IT7269_ADDR, POINT_BUFFER_INDEX, pointdata, 14);

    // 检查是否是手势事件
    if(pointdata[0] & GESTURES_FLAG)
    {
        gesture.type = pointdata[1];
        // 如果是滑动手势，读取方向
        if(gesture.type == GESTURE_FLICK)
        {
            gesture.direction = pointdata[10];
        }
    }

    return gesture;
}