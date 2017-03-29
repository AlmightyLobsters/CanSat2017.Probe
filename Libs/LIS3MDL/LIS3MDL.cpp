#include <LIS3MDL/LIS3MDL.h>

LIS3MDL::LIS3MDL(PinName sda, PinName scl)
	: i2c_p(new I2C(sda, scl))
	, i2c(*i2c_p)
{
}

LIS3MDL::LIS3MDL(I2C& i2c)
	: i2c_p(nullptr)
	, i2c(i2c)
{
}

LIS3MDL::~LIS3MDL()
{
	if (i2c_p != nullptr)
		delete i2c_p;
}

bool LIS3MDL::init()
{
	char data = WHO_AM_I;
	i2c.write(address, &data, 1);

	i2c.read(address, &data, 1);

	if (data != this->whoAmI)
		return false;

	data = CTRL_REG1;
	i2c.write(address, &data, 1);
	data = 0b0100011;
	i2c.write(address, &data, 1);

	data = CTRL_REG2;
	i2c.write(address, &data, 1);
	data = 0b01100000;
	i2c.write(address, &data, 1);

	data = CTRL_REG3;
	i2c.write(address, &data, 1);
	data = 0b00000001;
	i2c.write(address, &data, 1);

	data = CTRL_REG4;
	i2c.write(address, &data, 1);
	data = 0b00001000;
	i2c.write(address, &data, 1);

	data = CTRL_REG5;
	i2c.write(address, &data, 1);
	data = 0x00000000;
	i2c.write(address, &data, 1);

	return true;
}

void LIS3MDL::read(int16_t* out)
{
	uint8_t cmd[6] = { OUT_X_L, 0, 0, 0, 0 };

	i2c.write(address, (char*)cmd, 1);
	cmd[0] = 0;

	i2c.read(address, (char*)cmd, 6);

	out[0] = (int16_t)((cmd[1] << 8) & 0xFF00) | cmd[0];
	out[1] = (int16_t)((cmd[3] << 8) & 0xFF00) | cmd[2];
	out[2] = (int16_t)((cmd[5] << 8) & 0xFF00) | cmd[4];
}
