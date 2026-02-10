#pragma once

#include <cmath>

struct Matrix4x4
{
    float data[16];

    Matrix4x4();
    static Matrix4x4 RotationZ(float angleRadians);
    Matrix4x4 operator*(const Matrix4x4& other) const;
    void Print();
};

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

// Lerp functions
float Lerp(float a, float b, float t);
Vector3 Lerp(const Vector3& a, const Vector3& b, float t);
Quaternion Lerp(const Quaternion& a, const Quaternion& b, float t);
Transform Lerp(const Transform& a, const Transform& b, float t);

// Clamp functions
float Clamp(float value, float min, float max);