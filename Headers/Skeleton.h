#pragma once

#include "MathsUtils.h"

#include <vector>
#include <string>
#include <iostream>

class Skeleton
{
public:
    int AddBone(const std::string& name, int parentIndex, const Matrix4x4& localTransform);
    int FindBone(const std::string& name);
    void UpdateWorldTransforms();
    Matrix4x4 GetWorldTransform(int boneIndex);
    void SetLocalTransform(int boneIndex, const Matrix4x4& newTransform);
    void ShowBonesTransform();

private:
    std::vector<std::string> bonesName;
    std::vector<int> bonesParentIndex;
    std::vector<Matrix4x4> bonesLocalTransform;
    std::vector<Matrix4x4> bonesWorldTransform;
};
