#include "../Headers/MathsUtils.h"

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

Transform::Transform(const Vector3& pos, const Quaternion& rot, const Vector3& scl) : position(pos), rotation(rot), scale(scl)
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

// Lerp functions
float Lerp(float a, float b, float t)
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

Vector3 Lerp(const Vector3& a, const Vector3& b, float t)
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

Quaternion Lerp(const Quaternion& a, const Quaternion& b, float t)
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

Transform Lerp(const Transform& a, const Transform& b, float t)
{
    Transform result = Transform();

    result = {
        Lerp(a.position, b.position, t),
        Lerp(a.rotation, b.rotation, t),
        Lerp(a.scale, b.scale, t),
    };

    return result;
}

float Clamp(float value, float min, float max)
{
    if (value < min)
    {
        return min;
    }

    if (value > max)
    {
        return max;
    }

    return value;
}
