#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>
#include "../Include/tgaimage.h"
#include "../Include/Globals.h"
#include "../Include/Math.h"
#include "../Include/Model.h"
#include "../Include/Shader.h"

/// \TODO Clean up code to get rid of all the warnings
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
Vec3<float> LightPos(0.0f, 0.5f, 1.0f);
Vec3<float> LightColor(0.7f, 0.7f, 0.7f);
Vec3<float> LightDir(0, 0, 1);
Vec3<float> CameraPos(0, 0, 3);


struct Camera
{
    Vec3<float> Position;
    Vec3<float> Translation;

    Vec3<float> Up;
    Vec3<float> Forward;
    Vec3<float> Right;

    Camera()
    {
        this->Position = Vec3<float>(0.f, 0.f, 0.f);
        this->Translation = Vec3<float>(0.f, 0.f, 0.f);
        this->Up = Vec3<float>(0.f, 1.f, 0.f);
        this->Forward = Vec3<float>(0.f, 0.f, -1.f);
        this->Right = Vec3<float>(1.f, 0.f, 0.f);
    }

    Mat4x4<float> LookAt(Vec3<float> Point)
    {
        Mat4x4<float> ModelView;

        // Update Camera's position in world accoridng to its translation
        this->Position += Translation;

        // Assuming that world up direction is always (0, 1, 0)
        this->Forward = MathFunctionLibrary::Normalize(Point - Position);
        this->Right = MathFunctionLibrary::Normalize(MathFunctionLibrary::CrossProduct(
                    this->Forward, Vec3<float>(0.f, 1.f, 0.f)));
        this->Up = MathFunctionLibrary::Normalize(MathFunctionLibrary::CrossProduct(
                    this->Right, this->Forward));

        // Construct the matrix using these three axes as basis
        // --- Right ---
        // --- Up ------
        // --- Forward -
        ModelView.Mat[0][0] = Right.x;
        ModelView.Mat[0][1] = Right.y;
        ModelView.Mat[0][2] = Right.z;

        ModelView.Mat[1][0] = Up.x;
        ModelView.Mat[1][1] = Up.y;
        ModelView.Mat[1][2] = Up.z;

        ModelView.Mat[2][0] = Forward.x;
        ModelView.Mat[2][1] = Forward.y;
        ModelView.Mat[2][2] = Forward.z;

        ModelView.Mat[3][3] = 1;

        Mat4x4<float> TransMatrix;
        TransMatrix.Identity();
        TransMatrix.Mat[0][3] = -1 * this->Translation.x;
        TransMatrix.Mat[1][3] = -1 * this->Translation.y;
        TransMatrix.Mat[2][3] = -1 * this->Translation.z; 
        
        return ModelView * TransMatrix; 
    }
};

/// \Note More optimized version of DrawLine, Inspired by GitHub ssloy/tinyrenderer
void Line(Vec2<int> Start, Vec2<int> End, TGAImage& image, const TGAColor& color)
{
    bool Steep = false;
    int d = 1;
    if (Start.x > End.x) Start.Swap(End);
    if (Start.y > End.y) d = -1;
    // Slope < 1
    if (std::abs(Start.x - End.x) > std::abs(Start.y - End.y))
    {
        Steep = true;
        for (int i = Start.x; i <= End.x; i++)
        {
            float Ratio = (i - Start.x) / (End.x - Start.x);
            int y = Start.y + d * Ratio * (End.y - Start.y);
        }
    }
    else
    {
        
    }
}

/// \TODO: Simplify logic and optimization
void DrawLine(Vec2<int> Start, Vec2<int> End, TGAImage& image, const TGAColor& color)
{
    int d = 1;
    //**** Divide by 0
    if (Start.x == End.x)
    {
        if (Start.y > End.y) Start.Swap(End);
        for (int i = Start.y; i <= End.y; i++)
        {
            image.set(Start.x, i, color);
        }

        return;
    }
    // Always start from left marching toward right
    // Swap Start and End
    if (Start.x > End.x) Start.Swap(End);
    // Derive the line function
    float Slope = (float)(Start.y - End.y) / (float)(Start.x - End.x);
    float Intercept = (float)End.y - Slope * End.x;
    Vec2<int> Next(Start);
    Vec2<int> StepA(1, 0);
    Vec2<int> StepB(1, 1);

    if (Slope < 0)
    {
        d *= -1;
        StepA.Swap(StepB);
    }
    if (Slope > 1 || Slope < -1)
    {
        StepA.Transpose();
        StepB.Transpose();
    }

    StepA.y *= d;
    StepB.y *= d;

    while (Next.x < End.x || Next.y != End.y)
    {
        //**** Slope < 1
        if (Slope >= -1 && Slope <= 1)
        {
            // Eval F(x+1,y+0.5)
            if (Next.y + d * 0.5f - Slope * (Next.x + 1.0f) - Intercept >= 0)
                Next += StepA;
            else
                Next += StepB;
        }
        /// \Bug: When slope is greater than 0, can invert x, y
        else
        {
            // Eval F(x+0.5, y+1)
            if (Next.y + d * 1.0f - Slope * (Next.x + .5f) - Intercept >= 0)
                Next += StepB;
            else
                Next += StepA;
        }
        //**** Draw pixel to the buffer
        image.set(Next.x, Next.y, color);
    }
}

void DrawTriangle(Vec2<int> V0, Vec2<int> V1, Vec2<int> V2, TGAImage& image, const TGAColor& color)
{
    DrawLine(V0, V1, image, color);
    DrawLine(V1, V2, image, color);
    DrawLine(V2, V0, image, color);
}

/// \Note: Using naive scan-line method
void FillTriangle(Vec2<int>& V0, Vec2<int>& V1, Vec2<int>& V2, TGAImage& image, const TGAColor& color)
{
    // Sort the vertices according to their y value
    if (V0.y > V1.y) V0.Swap(V1);
    if (V1.y > V2.y) V1.Swap(V2);

    /// \Note: Compress code for rasterizing bottom half and upper half into one chunk
    for (int y = V0.y; y < V2.y; y++)
    {
         bool UpperHalf = (y >= V1.y);
        // Triangle similarity
        /// \Note: Speed-up: extract the constant part of the formula, 
        ///  the only variable in this calculation that is changing during
        ///  every iteration is y
        int Left = (V2.x - V0.x) * (y - V0.y) / (V2.y - V0.y) + V0.x;
        int Right = (UpperHalf ? 
            V2.x - (V2.x - V1.x) * (V2.y - y) / (V2.y - V1.y) : 
            V0.x + (V1.x - V0.x) * (y - V0.y) / (V1.y - V0.y));

        if (Left > Right) std::swap(Left, Right);

        for (int x = Left; x <= Right; x++)
          image.set(x, y, color);
    }
}

int main(int argc, char* argv[]) {
    // Testing Matrix multiplication
    Camera Camera;

    int ImageSize = ImageWidth * ImageHeight;    
    // Create an image for writing pixels
    TGAImage image(ImageWidth, ImageHeight, TGAImage::RGB);
    char ModelPath[64] = { "../Graphx/Assets/Model.obj" };
    char TexturePath[64] = { "../Graphx/Assets/Textures/african_head_diffuse.tga" };
    
    // Model
    Mat4x4<float> ModelToWorld;
    ModelToWorld.Identity();
    ModelToWorld.SetTranslation(Vec3<float>(0.f, 0.f, -2.f));
    // View
    Camera.Position = CameraPos;
    Camera.Translation = Vec3<float>(0.0, 0.0, 0.0);
    Mat4x4<float> View = Camera.LookAt(Vec3<float>(0.0f, 0.0f, -1.0f));
    // Projection
    Mat4x4<float> Perspective = Mat4x4<float>::Perspective(-1.f, -10.f, 0.f);
    
    Shader Shader;
    Shader.VS.MVP = Perspective * View * ModelToWorld;
    Shader.VS.Viewport = Mat4x4<float>::ViewPort(ImageWidth, ImageHeight);
    
    // Add a scope here to help trigger Model's destructor
    {
        Model Model;
        Model.Parse(ModelPath);
        TGAImage Sample;
        Model.LoadTexture(&Sample, TexturePath);
        Sample.flip_vertically();

        float* ZBuffer = new float[ImageWidth * ImageHeight];
        for (int i = 0; i < ImageSize; i++) ZBuffer[i] = -100.0f;

        Shader.FS.ZBuffer = ZBuffer;
        Shader.Draw(Model, image);
    }

    /// \TODO: Maybe instead of writing to an image,
    ///         can draw to a buffer, and display it using
    ///         a Win32 window
    // Draw the output to a file
    image.flip_vertically();
    image.write_tga_file("output.tga");

    return 0;
}   
