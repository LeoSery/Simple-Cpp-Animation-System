#pragma once

#include <string>
#include <vector>

struct AnimationClip
{
    std::string name;
    float duration;
};

class BlendTree1D
{
public:
    void addAnimation(float threshold, AnimationClip* clip);
    std::vector<float> calculateWeights(float parameter);
    void printWeights(float parameter);

private:
    std::vector<float> thresholds;
    std::vector<AnimationClip*> clips;
};
