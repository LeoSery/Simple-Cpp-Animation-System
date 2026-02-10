#pragma once

#include "MathsUtils.h"

#include <vector>

struct Pose
{
    std::vector<Transform> boneTransforms;

    Pose();
    Pose(int boneCount, const Transform& defaultTransform);
};

// Blend poses with weights
Pose BlendPoses(const std::vector<Pose>& poses, std::vector<float> weights);
