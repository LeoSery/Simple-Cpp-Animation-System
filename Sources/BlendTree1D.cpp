#include "../Headers/BlendTree1D.h"

#include <iostream>

void BlendTree1D::addAnimation(float threshold, AnimationClip* clip)
{
    thresholds.push_back(threshold);
    clips.push_back(clip);
}

std::vector<float> BlendTree1D::calculateWeights(float parameter)
{
    std::vector<float> weights(clips.size(), 0.0f);

    if (parameter <= thresholds[0])
    {
        weights[0] = 1.0f;
        return weights;
    }

    if (parameter >= thresholds[thresholds.size() - 1])
    {
        weights[thresholds.size() - 1] = 1.0f;
        return weights;
    }

    int intervalIndex = 0;
    float threshold1 = 0.0f;
    float threshold2 = 0.0f;

    for (int i = 0; i <= thresholds.size() - 2; i++)
    {
        if (thresholds[i] <= parameter && parameter < thresholds[i + 1])
        {
            intervalIndex = i;

            threshold1 = thresholds[intervalIndex];
            threshold2 = thresholds[intervalIndex + 1];

            weights[i] = (threshold2 - parameter) / (threshold2 - threshold1);
            weights[i + 1] = (parameter - threshold1) / (threshold2 - threshold1);
        }
    }

    return weights;
}

void BlendTree1D::printWeights(float parameter)
{
    std::vector<float> NewWeights = calculateWeights(parameter);

    std::cout << "[";
    for (float weight : NewWeights)
    {
        std::cout << weight << ",";
    }
    std::cout << "]" << std::endl;
}
