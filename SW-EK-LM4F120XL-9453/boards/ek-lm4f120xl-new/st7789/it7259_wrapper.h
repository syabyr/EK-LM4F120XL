#ifndef __IT7259_WRAPPER_H__
#define __IT7259_WRAPPER_H__

#define IT7259_I2C_ADDR 0x46

#define IT7259_CMD_BUFFER               0x20
#define IT7259_CMD_RESP_BUFFER          0xA0
#define IT7259_QUERY_BUFFER             0x80
#define IT7259_POINT_INF_BUFFER         0xE0

void tp_init();

void get_device_name(uint8_t *recv, uint8_t len);

void check_info();
#endif