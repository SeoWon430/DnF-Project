#include "stdafx.h"
#include "Math.h"

const float Math::PI = 3.14159265f;

float Math::Modulo(float val1, float val2)
{
	while (val1 - val2 >= 0)
		val1 -= val2;

	return val1;
}

float Math::ToRadian(float degree)
{
	return degree * PI / 180.0f;
}

float Math::ToDegree(float radian)
{
	return radian * 180.0f / PI;
}

float Math::Random(float r1, float r2)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = r2 - r1;
	float val = random * diff;

	return r1 + val;
}

float Math::Length(Vector3 a, Vector3 b)
{
	float result = (a.x - b.x) * (a.x - b.x);
	result += (a.y - b.y) * (a.y - b.y);
	result += (a.z - b.z) * (a.z - b.z);
	return result;
}

float Math::LengthXZ(Vector3 a, Vector3 b)
{
	float result = (a.x - b.x) * (a.x - b.x);
	result += (a.z - b.z) * (a.z - b.z);
	return result;
}

int Math::Random(int r1, int r2)
{
	return (int)(rand() % (r2 - r1 + 1)) + r1;
}