#include <mbed.h>
#include <BME280/BME280.h>
#include <LIS3MDL/LIS3MDL.h>
#include <RadioHead/RH_RF69.h>

// Serial
Serial pc(USBTX, USBRX);
Serial gps(PA_9, PA_10);

// I2C
I2C sensorI2C(PB_9, PB_8);
BME280 bme(sensorI2C);
LIS3MDL lis(sensorI2C);

// SPI

RH_RF69 radio(PA_8, PA_11);

struct
{
	uint8_t lat_o;
	uint8_t lon_o;
	int16_t temp;
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

char gpsMsg[80];

bool setup();
bool waitSerialWithTimeout(Serial*, uint16_t);
bool checkSum(char*, uint16_t, uint8_t*);
int main()
{
	if (!setup()) return 1;
	for (;;)
	{
		memset(&data, 0, sizeof(data));
		if (waitSerialWithTimeout(&gps, 700))
		{
			gps.gets(gpsMsg, 79);
			uint8_t padding;
			if (checkSum(gpsMsg, 79, &padding) && strncmp(&gpsMsg[padding], "GPGGA", 5) == 0)
			{
				char buffer[15]; // Max length of a GGA datafield should be 11, so this is fine
				memset(buffer, 0, sizeof(buffer));
				for (uint8_t i = 0; i < sizeof(buffer); i++) buffer[i] = 0;
				uint8_t startIndex = 0, endIndex;
				while (gpsMsg[startIndex] != ',') startIndex++; // Go after $GPGGA
				endIndex = ++startIndex; // Move endIndex after startIndex
				while (gpsMsg[endIndex] != ',') endIndex++; // Move endIndex to next comma
				if (endIndex != startIndex) // If there are characters between the commas, interpret time
				{
					memcpy(buffer, &gpsMsg[startIndex], endIndex - startIndex); // Copy characters from 1 char after comma to 1 char before comma
					data.time = strtof(buffer, nullptr);
					memset(buffer, 0, sizeof(buffer)); // Clear buffer
				}
				startIndex = ++endIndex; // Set startIndex to next comma, increment endIndex
				while (gpsMsg[endIndex] != ',') endIndex++; // Move endIndex to next comma
				if (endIndex != startIndex) // If there are characters between commas, interpret latitude
				{
					memcpy(buffer, &gpsMsg[startIndex + 1], endIndex - startIndex); // Copy characters between commas
					data.lat = strtof(buffer, nullptr);
					memset(buffer, 0, sizeof(buffer)); // Clear buffer
				}
				startIndex = ++endIndex; // Set startIndex to next comma, increment endIndex
				while (gpsMsg[endIndex] != ',') endIndex++; // Move endIndex to next comma
				if (endIndex != startIndex)
				{
					memcpy(buffer, &gpsMsg[startIndex], endIndex - startIndex); // Copy characters between commas
					data.lat_o = buffer[0] == 'S';
					memset(buffer, 0, sizeof(buffer)); // Clear buffer
				}
				startIndex = ++endIndex;
				while (gpsMsg[endIndex] != ',') endIndex++; // Move endIndex to next comma
				if (endIndex != startIndex)
				{
					memcpy(buffer, &gpsMsg[startIndex], endIndex - startIndex);
					data.lon = strtof(buffer, nullptr);
					memset(buffer, 0, sizeof(buffer));
				}
				startIndex = ++endIndex;
				while (gpsMsg[endIndex] != ',') endIndex++;
				if (endIndex != startIndex)
				{
					memcpy(buffer, &gpsMsg[startIndex], endIndex - startIndex);
					data.lon_o = buffer[0] == 'E';
					memset(buffer, 0, sizeof(buffer));
				}
				startIndex = ++endIndex;
				for (uint8_t i = 0; i < 3; i++) // Skip 3 fields
				{
					while (gpsMsg[startIndex] != ',')
						startIndex++;
					startIndex++;
				}
				endIndex = startIndex;
				while (gpsMsg[endIndex] != ',') endIndex++;
				if (endIndex != startIndex)
				{
					memcpy(buffer, &gpsMsg[startIndex], endIndex - startIndex);
					data.alt = strtof(buffer, nullptr);
					memset(buffer, 0, sizeof(buffer));
				}
			}
			memset(gpsMsg, 0, 100);
		}

		data.temp = bme.getTemperature() * 100;
		data.pres = bme.getPressure() * 10;
		data.hmdt = bme.getHumidity() * 100;
		lis.read(data.mag);

		radio.send((uint8_t*)&data, sizeof(data));
		radio.waitPacketSent();
		pc.printf("Sent data\n");
	}
}

bool setup()
{
	Timer t;
#pragma region Radio
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
#pragma endregion

#pragma region I2C
	bme.initialize();

	if (!lis.init())
	{
		pc.printf("LIS3MDL not initialized");
		return false;
	}
#pragma endregion

#pragma region SPI

#pragma endregion

#pragma region GPS
	// Baudrate 115200
	gps.printf("$PMTK251,115200*1F\r\n");
	if (!waitSerialWithTimeout(&gps, 1000))
	{
		pc.printf("GPS baudrate set failed");
		return false;
	}
	gps.baud(115200);

	// Fix interval 500ms
	gps.printf("$PMTK300,500,0,0,0,0*28\r\n");
	// Enable GGA protocol
	gps.printf("$PMTK314,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n");
#pragma endregion

	return true;
}

bool waitSerialWithTimeout(Serial* serial, uint16_t timeout)
{
	Timer t;
	t.start();
	while (!serial->readable() && t.read_ms() < timeout)
		;
	t.stop();
	return t.read_ms() < timeout;
}

bool checkSum(char* msg, uint16_t length, uint8_t* padding)
{
	uint16_t realLength = strnlen(msg, length);
	if (!(realLength > 6 &&
		msg[0] == '$' &&
		msg[realLength - 1] == '\n' &&
		msg[realLength - 2] == '\r' &&
		msg[realLength - 5] == '*'))
		return false;
	*padding = 1;
	while (msg[*padding] == '$') (*padding)++;
	char expChecksum[2] = { msg[realLength - 4], msg[realLength - 3] };
	char checksum[2] = { 0, 0 };
	uint8_t interChecksum = 0;
	for (uint16_t i = *padding; i < realLength - 5; i++)
		interChecksum ^= msg[i];
	sprintf(checksum, "%02X", interChecksum);
	return strncmp(checksum, expChecksum, 2) == 0;
}
