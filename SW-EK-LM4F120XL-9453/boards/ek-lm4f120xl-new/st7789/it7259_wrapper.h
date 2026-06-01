#ifndef __IT7259_WRAPPER_H__
#define __IT7259_WRAPPER_H__

#include <stdint.h>

#define IT7269_ADDR 0x46  // 7位I2C地址

// IT7269 Buffer Definitions (来自官方驱动)
#define COMMAND_BUFFER_INDEX          0x20
#define QUERY_BUFFER_INDEX            0x80
#define COMMAND_RESPONSE_BUFFER_INDEX 0xA0
#define POINT_BUFFER_INDEX            0xE0

#define QUERY_SUCCESS                 0x00
#define QUERY_BUSY                    0x01
#define QUERY_ERROR                   0x02
#define QUERY_POINT                   0x80

#define POINT_FLAG                    0x08
#define GESTURES_FLAG                 0x80

// 中断配置命令
#define IT7269_CMD_SET_INTERRUPT      0x02

// 手势类型定义
#define GESTURE_TAP                   0x20    // 单击
#define GESTURE_FLICK                 0x22    // 快速滑动
#define GESTURE_DOUBLE_TAP            0x23    // 双击

// 滑动方向定义 (仅FLICK手势有效)
#define DIR_UP                        0x08    // 上
#define DIR_UPPER_RIGHT               0x09    // 右上
#define DIR_RIGHT                     0x0A    // 右
#define DIR_LOWER_RIGHT               0x0B    // 右下
#define DIR_DOWN                      0x0C    // 下
#define DIR_LOWER_LEFT                0x0D    // 左下
#define DIR_LEFT                      0x0E    // 左
#define DIR_UPPER_LEFT                0x0F    // 左上

// 手势返回结构体
typedef struct {
    uint8_t type;         // 手势类型：GESTURE_TAP/FLICK/DOUBLE_TAP，0表示无手势
    uint8_t direction;    // 滑动方向，仅FLICK手势有效
} IT7269_Gesture_t;

// 触摸中断标志
extern volatile uint8_t g_touch_interrupt_flag;

void tp_init();
uint8_t IT7269_CheckNewPoint(void);
void IT7269_ReadTouch(uint16_t *pX, uint16_t *pY, uint8_t *pPressed);
void IT7269_SetInterrupt(uint8_t enable, uint8_t type);
uint8_t IT7269_GetInterruptFlag(void);
void IT7269_ClearInterruptFlag(void);

// 手势读取函数
IT7269_Gesture_t IT7269_ReadGesture(void);

#endif