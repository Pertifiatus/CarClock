#ifndef DS3231_h
#define DS3231_h

#include "Arduino.h"
#include <SoftwareWire.h> 

#define DS3231_ADDR		0x68

#define FORMAT_SHORT	1
#define FORMAT_LONG		2

#define FORMAT_LITTLEENDIAN	1
#define FORMAT_BIGENDIAN	2
#define FORMAT_MIDDLEENDIAN	3

#define MONDAY		1
#define TUESDAY		2
#define WEDNESDAY	3
#define THURSDAY	4
#define FRIDAY		5
#define SATURDAY	6
#define SUNDAY		7

#define SQW_RATE_1		0
#define SQW_RATE_1K		1
#define SQW_RATE_4K		2
#define SQW_RATE_8K		3

#define OUTPUT_SQW		0
#define OUTPUT_INT		1

class Time
{
public:
	uint8_t		hour;
	uint8_t		min;
	uint8_t		sec;
	uint8_t		date;
	uint8_t		mon;
	uint16_t	year;
	uint8_t		dow;

	Time();
};

class DS3231
{
	public:
		DS3231(SoftwareWire *theWire);
		
        void	begin();
		Time	getTime();
		void	setTime(uint8_t hour, uint8_t min, uint8_t sec);
		void	setDate(uint8_t date, uint8_t mon, uint16_t year);
		void	setDOW();
		void	setDOW(uint8_t dow);

		char	*getTimeStr(uint8_t format=FORMAT_LONG);
		char	*getDateStr(uint8_t slformat=FORMAT_LONG, uint8_t eformat=FORMAT_LITTLEENDIAN, char divider='.');
		
        // ÄNDERUNG: const char* für feste Texte
		const char	*getDOWStr(uint8_t format=FORMAT_LONG);
		const char	*getMonthStr(uint8_t format=FORMAT_LONG);
		
        long	getUnixTime(Time t);

		void	enable32KHz(bool enable);
		void	setOutput(byte enable);
		void	setSQWRate(int rate);
		float	getTemp();

	private:
        SoftwareWire *_wire;
		uint8_t _burstArray[7];

		void	_burstRead();
		uint8_t	_readRegister(uint8_t reg);
		void 	_writeRegister(uint8_t reg, uint8_t value);
		uint8_t	_decode(uint8_t value);
		uint8_t	_decodeH(uint8_t value);
		uint8_t	_decodeY(uint8_t value);
		uint8_t	_encode(uint8_t vaule);
};
#endif