#include "../Headers/IKSolver.h"

IKResult SolveTwoBoneIK(const Vector3& start, const Vector3& target, const IKChain& chain)
{
	IKResult result = IKResult();
	
	float distance = std::sqrt(std::pow(start.x - target.x, 2) + std::pow(start.y - target.y, 2) + std::pow(start.z - target.z, 2));

	float chainLength = chain.lowerLength + chain.upperLength;
	float maxReach = chainLength;
	float minReach = std::abs(chain.lowerLength - chain.upperLength);

	float EPSILON = 0.000001f;
	if (distance < minReach || distance > maxReach || distance < EPSILON)
	{
		result.shoulderAngle = 0.0f;
		result.elbowAngle = 0.0f;
		result.isReachable = false;
		return result;
	}
	else
	{
		result.isReachable = true;
	}

	float elbowAngle = 0.0f;
	elbowAngle = std::acos(Clamp((std::pow(chain.lowerLength, 2) + std::pow(chain.upperLength, 2) - std::pow(distance, 2)) / (2 * chain.lowerLength * chain.upperLength), -1 , 1));
	result.elbowAngle = elbowAngle;

	float shoulderAngle = 0.0f;
	float angleToTarget = 0.0f;
	float internalAngle = 0.0f;

	angleToTarget = std::atan2(target.y - start.y, target.x - start.x);
	internalAngle = std::acos(Clamp((std::pow(chain.lowerLength, 2) + std::pow(distance, 2) - std::pow(chain.upperLength, 2)) / (2 * chain.lowerLength * distance), -1, 1));
	shoulderAngle = angleToTarget + internalAngle;
	result.shoulderAngle = shoulderAngle;

	return result;
}
