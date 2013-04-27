// Ŭnicode please
#include "stdafx.h"
#include "sora/math_helper.h"

namespace sora {;
float DegToRad(float degree)
{
	return static_cast<float>(degree * kPi / 180.0f);
}
float RadToDeg(float radian)
{
	return static_cast<float>(radian * 180.0f / kPi);
}

bool EqualAbsError(float a, float b, float maxError)
{
	if(a == b) {
		return true;
	}
	if(abs(a - b) <= maxError) {
		return true;
	}
	return false;
}

bool EqualRelError(float a, float b, float maxError) 
{
	if(fabs(a - b) < maxError) {
		return true;
	}
	float relativeError = 0;
	if(abs(b) > abs(a)) {
		relativeError = abs((a-b) / b);
	}	else {
		relativeError = abs((a-b) / a);
	}
	if(relativeError <= maxError) {
		return true;
	}
	return false;
}

bool EqualUlps(float a, float b, int maxUlps) 
{
	//http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm
	SR_ASSERT(sizeof(float) == sizeof(int));
	if (a == b) {
		return true;
	}
	int intDiff = abs(*(int*)&a - *(int*)&b);
	if (intDiff <= maxUlps) {
		return true;
	}
	return false;
}
}	//namespace sora
