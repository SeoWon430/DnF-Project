#pragma once
class Math
{
public:
	static const float PI;

	static float Modulo(float val1, float val2);

	static float ToRadian(float degree);
	static float ToDegree(float radian);

	static int Random(int r1, int r2);
	static float Random(float r1, float r2);

	static float Length(Vector3 a, Vector3 b);
	static float LengthXZ(Vector3 a, Vector3 b);

	template<typename T>
	static T Clamp(T value, T min, T max)
	{
		value = value > max ? max : value;
		value = value < min ? min : value;

		return value;
	}
	// T를 알수 없기에 cpp에 정의하지 않고 인라인으로 바로 사용
};
