#include "i2c_wrapper.h"


uint8_t i2cReadByte(uint8_t device_address, uint8_t device_register)
{
	//specify that we want to communicate to device address with an intended write to bus
	ROM_I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, device_address, false);

	//the register to be read
	ROM_I2CMasterDataPut(I2C0_MASTER_BASE, device_register);

	//send control byte and register address byte to slave device
	ROM_I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_SINGLE_SEND);

	//wait for MCU to complete send transaction
	while(ROM_I2CMasterBusy(I2C0_MASTER_BASE));

	//read from the specified slave device
	ROM_I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, device_address, true);

	//send control byte and read from the register from the MCU
	ROM_I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);

	//wait while checking for MCU to complete the transaction
	while(ROM_I2CMasterBusy(I2C0_MASTER_BASE));

	//Get the data from the MCU register and return to caller
	return( ROM_I2CMasterDataGet(I2C0_MASTER_BASE));
}

void i2cWriteByte(uint8_t device_address, uint8_t device_register, uint8_t device_data)
{
    //specify that we want to communicate to device address with an intended write to bus
    ROM_I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, device_address, false);

    //register to be read
    ROM_I2CMasterDataPut(I2C0_MASTER_BASE, device_register);

    //send control byte and register address byte to slave device
    ROM_I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_START);

    //wait for MCU to complete send transaction
    while(ROM_I2CMasterBusy(I2C0_MASTER_BASE));

    //specify data to be written to the above mentioned device_register
    ROM_I2CMasterDataPut(I2C0_MASTER_BASE, device_data);

    //send data that was specified above
    ROM_I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);

    //wait for MCU & device to complete transaction
    while(ROM_I2CMasterBusy(I2C0_MASTER_BASE));
}

void i2cWriteTwoBytes(uint8_t device_address, uint8_t device_register, uint8_t data1,uint8_t data2)
{
    //specify that we want to communicate to device address with an intended write to bus
    ROM_I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, device_address, false);

    //register to be read
    ROM_I2CMasterDataPut(I2C0_MASTER_BASE, device_register);

    //send control byte and register address byte to slave device
    ROM_I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_START);

    //wait for MCU to complete send transaction
    while(ROM_I2CMasterBusy(I2C0_MASTER_BASE));

    //specify data to be written to the above mentioned device_register
    ROM_I2CMasterDataPut(I2C0_MASTER_BASE, data1);

    //send data that was specified above
    ROM_I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
    while(ROM_I2CMasterBusy(I2C0_MASTER_BASE));

    //specify data to be written to the above mentioned device_register
    ROM_I2CMasterDataPut(I2C0_MASTER_BASE, data2);

    //send data that was specified above
    ROM_I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);

    //wait for MCU & device to complete transaction
    while(ROM_I2CMasterBusy(I2C0_MASTER_BASE));
}

void i2cWriteBytes(uint8_t device_address,uint8_t device_register, uint8_t *pData, size_t size)
{
    //specify that we want to communicate to device address with an intended write to bus
    ROM_I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, device_address, false);

    //register to be read
    ROM_I2CMasterDataPut(I2C0_MASTER_BASE, device_register);

    //send control byte and register address byte to slave device
    ROM_I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_START);

    //wait for MCU to complete send transaction
    while(ROM_I2CMasterBusy(I2C0_MASTER_BASE));

    for(size_t i = 0; i < size; i++)
    {
        //specify data to be written to the above mentioned device_register
        ROM_I2CMasterDataPut(I2C0_MASTER_BASE, *pData++);

        //send data that was specified above
        if(i == (size - 1))
            ROM_I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
        else
            ROM_I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);

        //wait for MCU & device to complete transaction
        while(ROM_I2CMasterBusy(I2C0_MASTER_BASE));
    }
}

void i2cReadBytes(uint8_t device_address,uint8_t device_register, uint8_t *pData, size_t size)
 {
        ROM_I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, device_address, false);
        ROM_I2CMasterDataPut(I2C0_MASTER_BASE, device_register);
        ROM_I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_SINGLE_SEND);
        while(ROM_I2CMasterBusy(I2C0_MASTER_BASE));
        ROM_I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, device_address, true);
        ROM_I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
        while(ROM_I2CMasterBusy(I2C0_MASTER_BASE));
        *pData++ = ROM_I2CMasterDataGet(I2C0_MASTER_BASE);
        for(size_t i = 1; i < size; i++)
        {
            ROM_I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
            while(ROM_I2CMasterBusy(I2C0_MASTER_BASE));
            *pData++ = ROM_I2CMasterDataGet(I2C0_MASTER_BASE);
        }
        ROM_I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
        while(ROM_I2CMasterBusy(I2C0_MASTER_BASE));
        *pData = ROM_I2CMasterDataGet(I2C0_MASTER_BASE);
 }



void i2cDetect()
{
    uint8_t address;
    printf("     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\r\n");
    for (int i = 0; i < 128; i += 16) {
        printf("%02x: ", i);
        for (int j = 0; j < 16; j++) {
            address = i + j;
            ROM_I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, address, true);
            ROM_I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_SINGLE_SEND);
            while (ROM_I2CMasterBusy(I2C0_MASTER_BASE));
            if (ROM_I2CMasterErr(I2C0_MASTER_BASE) == I2C_MASTER_ERR_NONE) {
                printf("%02x ", address);
            } else {
                printf("-- ");
            }
        }
        printf("\r\n");
    }
}



void i2c_init(void)
{
	//This function is for eewiki and is to be updated to handle any port

	//enable I2C module
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);

	//reset I2C module
	SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);

	//enable GPIO peripheral that contains I2C
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

	// Configure the pin muxing for I2C0 functions on port B2 and B3.
	GPIOPinConfigure(GPIO_PB2_I2C0SCL);
	GPIOPinConfigure(GPIO_PB3_I2C0SDA);

	// Select the I2C function for these pins.
	GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
	GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);


	// Enable and initialize the I2C0 master module.  Use the system clock for
	// the I2C0 module.  The last parameter sets the I2C data transfer rate.
	// If false the data rate is set to 100kbps and if true the data rate will
	// be set to 400kbps.
	I2CMasterInitExpClk(I2C0_MASTER_BASE, SysCtlClockGet(), false);
    ROM_I2CMasterEnable(I2C0_MASTER_BASE);
	//clear I2C FIFOs
	//HWREG(I2C0_MASTER_BASE + I2C_O_FIFOCTL) = 80008000;
}




 




