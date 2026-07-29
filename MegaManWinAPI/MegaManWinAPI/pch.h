#pragma once

#define NOMINMAX 
#include <windows.h>	// min/max 유틸함수가 또 구현. 
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <functional>
#include <algorithm>
#include <windowsx.h>
#include <queue>

// C++ 20 std::format
#include <format>
#include <filesystem>
namespace fs = std::filesystem;

#include "Util.h"

// Json
#include <fstream>
#include "Json/nlohmann/json.hpp" // nlohmann/json 헤더 파일

// Direct2D 및 WIC 헤더
#include <d2d1.h>
#include <wincodec.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "windowscodecs.lib")
// 정수형 변수
using json = nlohmann::json;

using int8 = char;		// 1byte 정수형
using int16 = short;	// 2byte 정수형
using int32 = int;		// 4byte 정수형
using int64 = long long;// 8byte 정수형

using uint8 = unsigned char;		// 1byte 정수형
using uint16 = unsigned short;	// 2byte 정수형
using uint32 = unsigned int;		// 4byte 정수형
using uint64 = unsigned long long;// 8byte 정수형

using namespace std;

#define SMALL_NUMBER			(1.e-8f)

struct Vector
{
	float x = 0;
	float y = 0;

	Vector() {}
	Vector(float x, float y) : x(x), y(y) {}
	Vector(POINT pt) : x((float)pt.x), y((float)pt.y) {}

	Vector operator+(const Vector& other) const
	{
		Vector ret;
		ret.x = x + other.x;
		ret.y = y + other.y;
		return ret;
	}

	Vector operator-(const Vector& other) const
	{
		Vector ret;
		ret.x = x - other.x;
		ret.y = y - other.y;
		return ret;
	}

	Vector operator*(float value) const
	{
		Vector ret;
		ret.x = x * value;
		ret.y = y * value;
		return ret;
	}
	void operator+=(const Vector& other)
	{
		x += other.x;
		y += other.y;
	}

	void operator-=(const Vector& other)
	{
		x -= other.x;
		y -= other.y;
	}

	void operator*=(float ratio)
	{
		x *= ratio;
		y *= ratio;
	}

	bool operator==(const Vector& other) const
	{
		return (x == other.x && y == other.y);
	}

	bool operator==(Vector& other)
	{
		return (x == other.x && y == other.y);
	}


	// 내적
	// 외적
	// 정규화
	// 크기
	float LengthSquared()
	{
		return x * x + y * y;
	}
	float Length()
	{
		return ::sqrt(LengthSquared());
	}

	void Normalize()
	{
		float length = Length();
		if (length < SMALL_NUMBER)
			return;

		x /= length;
		y /= length;
	}

	// Dot 내적
	// float = cos(A)
	float Dot(Vector other) const
	{
		return x * other.x + y * other.y;
	}

	// Cross 외적
	// 결과가 3D 벡터여야하는데, 2D 좌표계라서 Z=0, float 하나를 리턴시킨다. (0,0,x)
	float Cross(Vector other)
	{
		return x * other.y - y * other.x;
	}

	Vector Rotate(float radian)
	{
		float cosA = cosf(radian);
		float sinA = sinf(radian);

		return Vector(x * cosA - y * sinA, x * sinA + y * cosA);
	}

	Vector Reflect(Vector normal) const
	{
		return *this - normal * (2.f * Dot(normal));
	}
};