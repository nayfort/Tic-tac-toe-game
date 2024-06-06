// Cross.h
#ifndef Cross_h
#define Cross_h
#include "shape.h"

namespace CrossGame

{
    class Cross : public Shape
    {
    public:
        Cross(float xCenter, float yCenter, float zCenter,
            float xSize, float ySize, float zSize,
            float* diffColor, float* ambiColor, float* specColor)
           
            : Shape(xCenter, yCenter, zCenter, xSize, ySize, zSize,
                specColor, diffColor, ambiColor) {}
        virtual void draw();
    };
}
#endif





