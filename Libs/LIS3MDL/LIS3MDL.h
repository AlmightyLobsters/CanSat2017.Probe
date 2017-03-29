#pragma once
#include <mbed.h>

class LIS3MDL
{
public:
	LIS3MDL(PinName sda, PinName scl);
	LIS3MDL(I2C& i2c);
	virtual ~LIS3MDL();

	bool init();
	void read(int16_t* out);

private:
	static const uint8_t address = 0b00111100;
	static const uint8_t whoAmI = 0b00111101;

	I2C* i2c_p;
	I2C& i2c;

	enum regAdds
	{
		WHO_AM_I = 0x0F,
		CTRL_REG1 = 0x20,
		CTRL_REG2 = 0x21,
		CTRL_REG3 = 0x22,
		CTRL_REG4 = 0x23,
		CTRL_REG5 = 0x24,
		OUT_X_L = 0x28,
		OUT_X_H = 0x29,
		OUT_Y_L = 0x2A,
		OUT_Y_H = 0x2B,
		OUT_Z_L = 0x2C,
		OUT_Z_H = 0x2D
	}
	;
};