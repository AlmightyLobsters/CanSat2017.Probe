#include <mbed.h>
#include <BME280/BME280.h>
#include <RadioHead/RH_RF69.h>

// Serial
Serial pc(USBTX, USBRX);
//Serial gps(PA_9, PA_10);

// I2C
//I2C sensorI2C(PB_9, PB_8);
//BME280 bme(sensorI2C);

// SPI

RH_RF69 radio(PA_8, PA_11);

struct
{
	uint8_t lat_o;
	uint8_t lon_o;
	uint16_t temp;
	uint16_t pres;
	uint16_t hmdt;
	float time;
	float lat;
	float lon;
	float alt;
	float batlvl;
	float velocity;
	int16_t acc[3];
	int16_t gyro[3];
	int16_t mag[3];
} data;

float temp, press, hmdt;

bool setup();
int main()
{
	if (!setup()) return 1;
	for (;;)
	{
		radio.send((uint8_t*)&data, sizeof(data));
		radio.waitPacketSent();
		pc.printf("Sent data\n");
		wait_ms(500);
	}
}

bool setup()
{
	if (!radio.init())
	{
		pc.printf("Radio not initialized");
		return false;
	}
	if (!radio.setFrequency(433.0))
	{
		pc.printf("Frequency set failed");
		return false;
	}
	radio.setTxPower(14);
	radio.setEncryptionKey(NULL);

	data = { 0 };
	data.temp = 1;
	data.pres = 2;
	data.hmdt = 3;
	data.time = 4;
	data.lat = 5;
	data.lat_o = 6;
	data.lon = 7;
	data.lon_o = 8;
	data.alt = 9;
	data.batlvl = 10;
	data.velocity = 11;
	data.acc[0] = 12;
	data.acc[1] = 13;
	data.acc[2] = 14;
	data.gyro[0] = 15;
	data.gyro[1] = 16;
	data.gyro[2] = 17;
	data.mag[0] = 18;
	data.mag[1] = 19;
	data.mag[2] = 20;
	return true;
}