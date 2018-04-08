#include "MathHelpers.h"

bool GetRandMod(float value, float minPercent, float maxPercent, int& outMod)
{
	float minValue = value * minPercent;
	float maxValue = value * maxPercent;
	outMod = (maxValue - minValue) + 1;
	if (outMod <= 0)
		return false;
	return true;
}

float GetRandMin(float value, float minPercent)
{
	return value * minPercent;
}

float GetInscribedCircleRadius(float width, float height)
{
	return (width > height) ? height : width;
}