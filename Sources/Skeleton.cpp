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

Matrix4x4 Matrix4x4::RotationZ(float angleRadians)
{
    Matrix4x4 result = Matrix4x4();

    result.data[0] = std::cos(angleRadians);
    result.data[1] = -(std::sin(angleRadians));
    result.data[4] = std::sin(angleRadians);
    result.data[5] = std::cos(angleRadians);

    return result;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& other) const
{
    Matrix4x4 result;

    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            float sum = 0.0f;

            for (int k = 0; k < 4; k++)
            {
                sum += this->data[row * 4 + k] * other.data[k * 4 + col];
            }

            result.data[row * 4 + col] = sum;
        }
    }

    return result;
}

void Matrix4x4::Print()
{
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            printf("%8.3f ", data[row * 4 + col]);
        }
        printf("\n");
    }
    printf("\n");
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

void Skeleton::ShowBonesTransform()
{
    if (bonesName.empty())
    {
        return;
    }

    std::cout << "Bones transform :" << std::endl;

    for (int i = 0; i < bonesName.size(); i++)
    {
        std::cout << bonesName[i] << std::endl;
        std::cout << "[" << std::endl;
        bonesWorldTransform[i].Print();
        std::cout << "]" << std::endl;
        std::cout << std::endl;
    }
}
