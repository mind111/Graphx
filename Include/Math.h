#pragma once
#include <cmath>
#include <algorithm>
#include <random>

template <class T>
struct Vec2
{
    T x;
    T y;

    //**** Constructor
    Vec2()
    {

    }

    Vec2(T p, T q)
    {
        x = p;
        y = q;
    }

    Vec2(const Vec2<T>& Another)
    {
        this->x = Another.x;
        this->y = Another.y;
    }

    //**** Operations
    Vec2<T> operator-(const Vec2<T>& Another)
    {
        return Vec2(this->x - Another.x, this->y - Another.y);
    }

    Vec2<T> operator+(const Vec2<T>& Another)
    {
        return Vec2<T>(this->x + Another.x, this->y + Another.y);
    }

    Vec2<T> operator=(const Vec2<T>& Another)
    {
        this->x = Another.x;
        this->y = Another.y;
        return *this;
    }
    
    void operator*=(const T Scalar)
    {
        this->x *= Scalar;
        this->y *= Scalar;
    }

    void operator/(const T Scalar)
    {
	return Vec2<T>(this.x / Scalar, this.y / Scalar);
    }

    Vec2<T>& operator+=(const Vec2<T>& Another)
    {
        this->x += Another.x;
        this->y += Another.y;

        return *this;
    }

    void Swap(Vec2<T>& Another)
    {
        Vec2<T> Holder;
        Holder = *this;
        *this = Another;
        Another = Holder;
    }

    void Transpose()
    {
        T Holder;
        Holder = this->x;
        this->x = this->y;
        this->y = Holder;
    }
};

template <class T>
struct Vec3
{
    T x;
    T y;
    T z;

    //**** Constructor
    Vec3()
    {
        
    }

    Vec3(T p, T q, T r)
    {
        x = p;
        y = q;
        z = r;
    }

    Vec3(const Vec3<T>& Another)
    {
        this->x = Another.x;
        this->y = Another.y;
        this->z = Another.z;
    }

    //**** Operations
    Vec3<T> operator-(const Vec3<T>& Another)
    {
        return Vec3(this->x - Another.x, this->y - Another.y, this->z - Another.z);
    }

    Vec3<T> operator+(const Vec3<T>& Another)
    {
        return Vec3<T>(this->x + Another.x, this->y + Another.y, this->z + Another.z);
    }
   
    Vec3<T> operator*(T Scalar)
    {
        return Vec3<T>(this->x * Scalar, this->y * Scalar, this->z * Scalar);        
    }

    Vec3<T> operator/(T Scalar)
    {
      return Vec3<T>(this->x / Scalar, this->y / Scalar, this->z / Scalar);
    }
  
    Vec3<T> operator=(const Vec3<T>& Another)
    {
        this->x = Another.x;
        this->y = Another.y;
        this->z = Another.z;
        return *this;
    }

    Vec3<T>& operator+=(const Vec3<T>& Another)
    {
        this->x += Another.x;
        this->y += Another.y;
        this->z += Another.z;
        return *this;
    }

    void Swap(Vec3<T>& Another)
    {
        Vec3<T> Holder;
        Holder = *this;
        *this = Another;
        Another = Holder;
    }
};

class MathFunctionLibrary
{
public:
    static Vec3<float> CrossProduct(const Vec3<float>& V0, const Vec3<float>& V1);
    static Vec3<float> Normalize(const Vec3<float>& v);
    template <class T>
    static float Length(const Vec3<T>& V0);
    template <class T>
    static T DotProduct(const Vec3<T>& V0, const Vec3<T>& V1)
    {
            return (V0.x * V1.x + V0.y * V1.y + V0.z * V1.z);
    }
};