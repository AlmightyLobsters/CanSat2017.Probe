#include <mbed.h>
#include <math.h>
#include <BME280.h>
#include <LSM6.h>
#include <LIS3MDL.h>
#include <RFM69.h>

Serial pc(USBTX, USBRX);

I2C sensorI2C(I2C_SDA, I2C_SCL);

BME280 bme(sensorI2C);
LSM6 lsm6(sensorI2C);
LIS3MDL lis3mdl(sensorI2C);

struct _data
{
	struct _prim
	{
		uint16_t temp; // offset 0
		uint16_t pres; // offset 2
		uint16_t hmdt; // offset 4
	} primary;
	struct _gps
	{
		float time; // offset 8
		float lat; // offset 12
		uint8_t lat_o; // true ~ S, false ~ N offset 16
		float lon; // offset 20
		uint8_t lon_o; // true ~ E, false ~ W offset 24
		float alt; // offset 28
	} gps;
	struct _stats
	{
		float batlvl; // offset 32
		float proprots; // offset 36
	} stats;
	struct _imu
	{
		int16_t acc[3]; // offset 40, 42, 44
		int16_t gyro[3]; // offset 46, 48, 50
		int16_t mag[3]; // offset 52, 54, 56
	} imu;	
} dataToSend;

int main() 
{
	size_t size = sizeof(dataToSend);
	for (;;)
	{
		lsm6.read();
		lis3mdl.read();
		dataToSend = { 0 };
		dataToSend.primary.temp = bme.getTemperature() * 100;
		dataToSend.primary.pres = bme.getPressure() * 10;
		dataToSend.primary.hmdt = bme.getHumidity() * 100;
		dataToSend.gps.time = 0;
		dataToSend.gps.lat = 0;
		dataToSend.gps.lat_o = 0;
		dataToSend.gps.lon = 0;
		dataToSend.gps.lon_o = 0;
		dataToSend.gps.alt = 0;
		dataToSend.stats.batlvl = 0;
		dataToSend.stats.proprots = 0;
		dataToSend.imu.acc[0] = lsm6.acc.x;
		dataToSend.imu.acc[1] = lsm6.acc.y;
		dataToSend.imu.acc[2] = lsm6.acc.z;
		dataToSend.imu.gyro[0] = lsm6.gyro.x;
		dataToSend.imu.gyro[1] = lsm6.gyro.y;
		dataToSend.imu.gyro[2] = lsm6.gyro.z;
		dataToSend.imu.mag[0] = lis3mdl.m.x;
		dataToSend.imu.mag[1] = lis3mdl.m.y;
		dataToSend.imu.mag[2] = lis3mdl.m.z;
		
		wait_ms(500);
	}
}