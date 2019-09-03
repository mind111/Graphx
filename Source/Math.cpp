#include <iostream>
#include "../Include/Math.h"

void Mat4x4<float>::Print()
{
    std::cout << "-- Mat4x4 --" << std::endl;
    for (int Row = 0; Row < 4; Row++)
    {
        std::cout << "[ ";
        for (int Col = 0; Col < 4; Col++)
           std::cout << this->Mat[Row][Col] << " "; 
        std::cout << "]\n";
    }
}

Mat4x4<float> Mat4x4<float>::GenViewPort(float VP_Width, float VP_Height)
{
   // Map [-1, 1] to [0, VP_Width]
   //
   Mat4x4<float> Res;

   Res.Identity();
   Res.Mat[0][0] = .5f * VP_Width;
   Res.Mat[0][3] = .5f * VP_Width;
   Res.Mat[1][1] = .5f * VP_Height;
   Res.Mat[1][3] = .5f * VP_Height;

   return Res;
}

template <class T>
inline float MathFunctionLibrary::Length(const Vec3<T>& V0)
{
    return sqrt(V0.x * V0.x + V0.y * V0.y + V0.z * V0.z);
}

Vec3<float> MathFunctionLibrary::CrossProduct(const Vec3<float>& V0, const Vec3<float>& V1)
{
    /// \Note: |i     -j     k|
    ///        |v0.x v0.y v0.z|
    ///        |v1.x v1.y v1.z|
    Vec3<float> Result;

    Result.x = V0.y * V1.z - V0.z * V1.y;
    Result.y = V0.z * V1.x - V0.x * V1.z;
    Result.z = V0.x * V1.y - V0.y * V1.x;

    return Result;
}

Vec3<float> MathFunctionLibrary::Normalize(const Vec3<float>& v)
{
    float Len = Length(v);
    return Vec3<float>(v.x / Len, v.y / Len, v.z / Len);
}
