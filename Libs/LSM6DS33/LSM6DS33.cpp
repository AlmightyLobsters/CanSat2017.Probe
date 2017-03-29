#include <LSM6DS33/LSM6DS33.h>

LSM6DS33::LSM6DS33(PinName sda, PinName scl)
	: i2c_p(new I2C(sda, scl))
	, i2c(*i2c_p)
{
}

LSM6DS33::LSM6DS33(I2C& i2c)
	: i2c_p(nullptr)
	, i2c(i2c)
{
}

LSM6DS33::~LSM6DS33()
{
	if (i2c_p != nullptr)
		delete i2c_p;
}

bool LSM6DS33::init()
{
	char data = WHO_AM_I;
	i2c.write(address, &data, 1);

	i2c.read(address, &data, 1);

	if (data != this->whoAmI)
		return false;

	data = CTRL_ACC_REG1;
	i2c.write(address, &data, 1);
	data = 0b00011100;
	i2c.write(address, &data, 1);

	data = CTRL_ACC_REG2;
	i2c.write(address, &data, 1);
	data = 0b00000000;
	i2c.write(address, &data, 1);

	data = CTRL_ACC_REG3;
	i2c.write(address, &data, 1);
	data = 0b00111000;
	i2c.write(address, &data, 1);

	data = CTRL_GYRO_REG1;
	i2c.write(address, &data, 1);
	data = 0b0001000;
	i2c.write(address, &data, 1);

	data = CTRL_GYRO_REG2;
	i2c.write(address, &data, 1);
	data = 0b10000000;
	i2c.write(address, &data, 1);

	data = CTRL_COM_REG1;
	i2c.write(address, &data, 1);
	data = 0b00000100;
	i2c.write(address, &data, 1);

	data = CTRL_COM_REG2;
	i2c.write(address, &data, 1);
	data = 0b1000000;
	i2c.write(address, &data, 1);

	data = CTRL_COM_REG3;
	i2c.write(address, &data, 1);
	data = 0b0000000;
	i2c.write(address, &data, 1);

	data = CTRL_COM_REG4;
	i2c.write(address, &data, 1);
	data = 0b00000000;
	i2c.write(address, &data, 1);

	data = CTRL_COM_REG5;
	i2c.write(address, &data, 1);
	data = 0b00111000;
	i2c.write(address, &data, 1);

	return true;
}

void LSM6DS33::readAcc(int16_t* out)
{
	char cmd[6] = { OUT_X_L_A, 0, 0, 0, 0 };

	i2c.write(address, cmd, 1);
	cmd[0] = 0;

	i2c.read(address, cmd, 6);

	out[0] = (int16_t)((cmd[1] << 8) & 0xFF00) | cmd[0];
	out[1] = (int16_t)((cmd[3] << 8) & 0xFF00) | cmd[2];
	out[2] = (int16_t)((cmd[5] << 8) & 0xFF00) | cmd[4];
}

void LSM6DS33::readGyro(int16_t* out)
{
	char cmd[6] = { OUT_X_L_G, 0, 0, 0, 0 };

	i2c.write(address, cmd, 1);
	cmd[0] = 0;

	i2c.read(address, cmd, 6);

	out[0] = (int16_t)((cmd[1] << 8) & 0xFF00) | cmd[0];
	out[1] = (int16_t)((cmd[3] << 8) & 0xFF00) | cmd[2];
	out[2] = (int16_t)((cmd[5] << 8) & 0xFF00) | cmd[4];
}

void LSM6DS33::read(int16_t* accOut, int16_t* gyroOut)
{
	readAcc(accOut);
	readGyro(gyroOut);
}
