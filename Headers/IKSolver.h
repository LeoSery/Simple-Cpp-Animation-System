#pragma once

#include "MathsUtils.h"

#include <cmath>

struct IKChain
{
	float upperLength;
	float lowerLength;
};

struct IKResult
{
	float shoulderAngle;
	float elbowAngle;
	bool isReachable;
};

IKResult SolveTwoBoneIK(const Vector3& start, const Vector3& target, const IKChain& chain);
