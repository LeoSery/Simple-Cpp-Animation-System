#include "../Headers/Skeleton.h"

Matrix4x4::Matrix4x4()
{
    for (int i = 0; i < 16; i++)
    {
        data[i] = 0;
    }

    data[0] = 1;
    data[5] = 1;
    data[10] = 1;
    data[15] = 1;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& other) const
{
    Matrix4x4 result;
    for (int i = 0; i < 16; i++)
    {
        result.data[i] = this->data[i] + other.data[i];
    }
    return result;
}

int Skeleton::AddBone(const std::string& name, int parentIndex, const Matrix4x4& localTransform)
{
    bonesName.push_back(name);
    bonesParentIndex.push_back(parentIndex);
    bonesLocalTransform.push_back(localTransform);
    return bonesName.size() - 1;
}

int Skeleton::FindBone(const std::string& name)
{
    if (name.empty())
    {
        return -1;
    }

    for (int i = 0; i <= bonesName.size() - 1; i++)
    {
        if (bonesName[i] == name)
        {
            return i;
        }
    }

    return -1;
}

void Skeleton::UpdateWorldTransforms()
{
    bonesWorldTransform.resize(bonesName.size());

    for (int i = 0; i < bonesName.size(); i++)
    {
        if (i == 0)
        {
            bonesWorldTransform[i] = bonesLocalTransform[i];
            continue;
        }

        Matrix4x4 ParentWorldTransform = GetWorldTransform(bonesParentIndex[i]);
        Matrix4x4 WorldTransform = ParentWorldTransform * bonesLocalTransform[i];
        bonesWorldTransform[i] = WorldTransform;
    }
}

Matrix4x4 Skeleton::GetWorldTransform(int boneIndex)
{
    if (boneIndex < 0 || boneIndex >= bonesName.size())
    {
        return Matrix4x4();
    }

    return bonesWorldTransform[boneIndex];
}

void Skeleton::SetLocalTransform(int boneIndex, const Matrix4x4& newTransform)
{
    if (boneIndex < 0 || boneIndex >= bonesName.size())
    {
        return;
    }

    bonesLocalTransform[boneIndex] = newTransform;
}
