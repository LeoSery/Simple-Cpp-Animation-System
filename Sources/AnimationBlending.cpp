#include "../Headers/AnimationBlending.h"

// Vector3 implementation
Vector3::Vector3() : x(0), y(0), z(0) {}
Vector3::Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

Vector3 Vector3::operator+(const Vector3& other) const
{
    return Vector3(x + other.x, y + other.y, z + other.z);
}

Vector3 Vector3::operator-(const Vector3& other) const
{
    return Vector3(x - other.x, y - other.y, z - other.z);
}

Vector3 Vector3::operator*(float scalar) const
{
    return Vector3(x * scalar, y * scalar, z * scalar);
}

// Quaternion implementation
Quaternion::Quaternion() : x(0), y(0), z(0), w(1) {}
Quaternion::Quaternion(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}

Quaternion Quaternion::operator+(const Quaternion& other) const
{
    return Quaternion(x + other.x, y + other.y, z + other.z, w + other.w);
}

Quaternion Quaternion::operator-(const Quaternion& other) const
{
    return Quaternion(x - other.x, y - other.y, z - other.z, w - other.w);
}

Quaternion Quaternion::operator*(float scalar) const
{
    return Quaternion(x * scalar, y * scalar, z * scalar, w * scalar);
}

// Transform implementation
Transform::Transform() : position(0, 0, 0), rotation(0, 0, 0, 1), scale(1, 1, 1) {}

Transform::Transform(const Vector3& pos, const Quaternion& rot, const Vector3& scl)  : position(pos), rotation(rot), scale(scl)
{

}

Transform Transform::operator+(const Transform& other) const
{
    return Transform(
        position + other.position,
        rotation + other.rotation,
        scale + other.scale
    );
}

Transform Transform::operator-(const Transform& other) const
{
    return Transform(
        position - other.position,
        rotation - other.rotation,
        scale - other.scale
    );
}

Transform Transform::operator*(float scalar) const
{
    return Transform(
        position * scalar,
        rotation * scalar,
        scale * scalar
    );
}

// Pose implementation
Pose::Pose()
{

}

Pose::Pose(int boneCount, const Transform& defaultTransform)
{
    boneTransforms.resize(boneCount, defaultTransform);
}

// Lerp functions
float lerp(float a, float b, float t)
{
    if (t <= 0.0f)
    {
        return a;
    }

    if (t >= 1.0f)
    {
        return b;
    }

    return a + (b - a) * t;
}

Vector3 lerp(const Vector3& a, const Vector3& b, float t)
{
    if (t <= 0.0f)
    {
        return a;
    }

    if (t >= 1.0f)
    {
        return b;
    }

    return a + (b - a) * t;
}

Quaternion lerp(const Quaternion& a, const Quaternion& b, float t)
{
    if (t <= 0.0f)
    {
        return a;
    }

    if (t >= 1.0f)
    {
        return b;
    }

    return a + (b - a) * t;
}

Transform lerp(const Transform& a, const Transform& b, float t)
{
    Transform result = Transform();

    result = {
        lerp(a.position, b.position, t),
        lerp(a.rotation, b.rotation, t),
        lerp(a.scale, b.scale, t),
    };

    return result;
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
