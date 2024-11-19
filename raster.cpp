#include <vector>
#include <string>
#include <iostream>
#include "common.h"

void Rasterize(Pixel* pixels, int width, int height, const std::vector<Triangle>& tris)
{
    // initialize z-buffer
    double zBuff[width][height];
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            zBuff[i][j] = 1;
        }
    }

    for(auto triangle : tris){
        // homogenous cord to 3D
        vec3 vA(triangle.A[0] / triangle.A[3], triangle.A[1] / triangle.A[3], triangle.A[2] / triangle.A[3]);
        vec3 vB(triangle.B[0] / triangle.B[3], triangle.B[1] / triangle.B[3], triangle.B[2] / triangle.B[3]);
        vec3 vC(triangle.C[0] / triangle.C[3], triangle.C[1] / triangle.C[3], triangle.C[2] / triangle.C[3]);

        for(int y = 0; y < height; ++y){
            for(int x = 0; x < width; ++x){
                // normalized device coordinates
                vec3 position(((x + 0.5) / static_cast<double>(width)) * 2 - 1, ((y + 0.5) / static_cast<double>(height)) * 2 - 1, 0.0);
                
                // triangle sides 
                vec3 pA = position - vA;
                vec3 bA = vB - vA;
                vec3 cA = vC - vA;

                // barycentric coordinates
                double denom = bA[0] * cA[1] - bA[1] * cA[0];
                double beta = (pA[0] * cA[1] - pA[1] * cA[0]) / denom;
                double gamma = (bA[0] * pA[1] - bA[1] * pA[0]) / denom;
                double alpha = 1.0 - beta - gamma;

                if(!(alpha < 0 || beta < 0 || gamma < 0)){
                    // perspecitve correction
                    double iDepth = alpha * vA[2] + beta * vB[2] + gamma * vC[2];
                    double aCorrect = alpha / (iDepth * triangle.A[3]);
                    double bCorrect = beta / (iDepth * triangle.B[3]);
                    double gCorrect = gamma / (iDepth * triangle.C[3]);
                    double correctSum = aCorrect + bCorrect + gCorrect;

                    aCorrect /= correctSum;
                    bCorrect /= correctSum;
                    gCorrect /= correctSum;

                    // assigning color 
                    vec3 interColor = aCorrect * triangle.Ca + bCorrect * triangle.Cb + gCorrect * triangle.Cc;

                    // update z-buffer
                    if( iDepth < zBuff[x][y]) {
                        zBuff[x][y] = iDepth;
                        set_pixel(pixels, width, height, x, y, interColor);
                    }
                }
            }
        }
    }
}
