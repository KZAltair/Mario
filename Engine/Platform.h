#pragma once
#include <stdint.h>
#include "RCWndIncludes.h"
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int8_t sint8;
typedef int16_t sint16;
typedef int32_t sint32;
typedef int64_t sint64;

typedef float float32;
typedef double float64;

//Graphics features for game platform
struct game_bitmap_struct
{
	void* BitmapMemory;
	sint32 BitmapWidth;
	sint32 BitmapHeight;
	uint32* PixelPointer;

};

#pragma pack(push, 1) //pack bytes tightly to 1 bytes align, no automatic alignment allowed
struct bitmap_header
{
	uint16 FileType;
	uint32 FileSize;
	uint16 Reserved1;
	uint16 Reserved2;
	uint32 Size;
	sint32 Width;
	sint32 Height;
	uint16 Planes;
	uint16 BytesPerPixel;
};
#pragma pack(pop) //pop back all automatic packing of bytes back

#pragma pack(push, 1) //pack bytes tightly to 1 bytes align, no automatic alignment allowed
struct bitmap_header_24bit
{
	uint16 FileType;
	uint32 FileSize;
	uint16 Reserved1;
	uint16 Reserved2;
	uint32 FileOffsetToPixels;
	uint32 HeaderSize;
	sint32 Width;
	sint32 Height;
	uint16 Planes;
	uint16 BitesPerPixel;
};
#pragma pack(pop) //pop back all automatic packing of bytes back


//Math utilities
struct game_vector2
{
	float32 x;
	float32 y;
};

class GamePlatform
{
public:
	GamePlatform();
	~GamePlatform();

	static int ClampScreen(int minScreen, int maxScreen, int val);

	void MeasurePerformance();
	float32 GetTimeFPS();
public:

	//Time and CPU cycles measurement
	LARGE_INTEGER PerCountFrequencyResult = {};
	sint64 PerfCountFrequency = 0;
	LARGE_INTEGER LastCounter = {};
	uint64 LastCycleCount = 0;
	uint64 EndCycleCount = 0;
	LARGE_INTEGER EndCounter = {};

	uint64 CyclesElapsed = 0;
	sint64 CounterElapsed = 0;
	float32 MSPerFrame = 0.0f; //How many seconds the loop cycle took
	float32 FPS = 0.0f;
	float32 MCPerFrame = 0.0f;


};

