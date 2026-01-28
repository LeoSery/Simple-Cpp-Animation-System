#pragma once

#include <vector>

struct Vector3
{
    float x, y, z;

    Vector3();
    Vector3(float _x, float _y, float _z);

    Vector3 operator+(const Vector3& other) const;
    Vector3 operator-(const Vector3& other) const;
    Vector3 operator*(float scalar) const;
};

struct Quaternion
{
    float x, y, z, w;

    Quaternion();
    Quaternion(float _x, float _y, float _z, float _w);

    Quaternion operator+(const Quaternion& other) const;
    Quaternion operator-(const Quaternion& other) const;
    Quaternion operator*(float scalar) const;
};

struct Transform
{
    Vector3 position;
    Quaternion rotation;
    Vector3 scale;

    Transform();
    Transform(const Vector3& pos, const Quaternion& rot, const Vector3& scl);

    Transform operator+(const Transform& other) const;
    Transform operator-(const Transform& other) const;
    Transform operator*(float scalar) const;
};

struct Pose
{
    std::vector<Transform> boneTransforms;

    Pose();
    Pose(int boneCount, const Transform& defaultTransform);
};

// Lerp functions
float lerp(float a, float b, float t);
Vector3 lerp(const Vector3& a, const Vector3& b, float t);
Quaternion lerp(const Quaternion& a, const Quaternion& b, float t);
Transform lerp(const Transform& a, const Transform& b, float t);

// Blend poses with weights
Pose BlendPoses(const std::vector<Pose>& poses, std::vector<float> weights);
