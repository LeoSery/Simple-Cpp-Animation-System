#include "../Headers/AnimationBlending.h"

Pose::Pose()
{

}

Pose::Pose(int boneCount, const Transform& defaultTransform)
{
    boneTransforms.resize(boneCount, defaultTransform);
}

// Blend poses with weights
Pose BlendPoses(const std::vector<Pose>& poses, std::vector<float> weights)
{
    if (poses.size() != weights.size())
    {
        return Pose();
    }

    float totalWeights = 0.0f;
    for (int i = 0; i < weights.size(); i++)
    {
        if (weights[i] < 0.0f)
        {
            weights[i] = 0.0f;
        }

        totalWeights += weights[i];
    }

    if (totalWeights == 0.0f)
    {
        return poses[0];
    }

    for (int i = 0; i < weights.size(); i++)
    {
        weights[i] = weights[i] / totalWeights;
    }

    Pose result = Pose();
    result.boneTransforms.resize(poses[0].boneTransforms.size(), Transform());

    for (int i = 0; i < result.boneTransforms.size(); i++)
    {
        for (int j = 0; j < weights.size(); j++)
        {
            result.boneTransforms[i] = result.boneTransforms[i] + (poses[j].boneTransforms[i] * weights[j]);
        }
    }

    return result;
}
