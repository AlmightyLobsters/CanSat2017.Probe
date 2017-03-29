#pragma once
#include <mbed.h>

class LSM6DS33
{
public:
	LSM6DS33(PinName sda, PinName scl);
	LSM6DS33(I2C& i2c);
	virtual ~LSM6DS33();

	bool init();
	void readAcc(int16_t* out);
	void readGyro(int16_t* out);
	void read(int16_t* accOut, int16_t* gyroOut);

private:
	static const uint8_t address = 0b11010110;
	static const uint8_t whoAmI = 0b01101001;

	I2C* i2c_p;
	I2C& i2c;

	enum regAdds
	{
		WHO_AM_I = 0x0F,
		CTRL_ACC_REG1 = 0x10,
		CTRL_ACC_REG2 = 0x17,
		CTRL_ACC_REG3 = 0x18,
		CTRL_GYRO_REG1 = 0x11,
		CTRL_GYRO_REG2 = 0x16,
		CTRL_COM_REG1 = 0x12,
		CTRL_COM_REG2 = 0x13,
		CTRL_COM_REG3 = 0x14,
		CTRL_COM_REG4 = 0x15,
		CTRL_COM_REG5 = 0x19,
		OUT_X_L_G = 0x22,
		OUT_X_H_G = 0x23,
		OUT_Y_L_G = 0x24,
		OUT_Y_H_G = 0x25,
		OUT_Z_L_G = 0x26,
		OUT_Z_H_G = 0x27,
		OUT_X_L_A = 0x28,
		OUT_X_H_A = 0x29,
		OUT_Y_L_A = 0x2A,
		OUT_Y_H_A = 0x2B,
		OUT_Z_L_A = 0x2C,
		OUT_Z_H_A = 0x2D
	}
	;
};