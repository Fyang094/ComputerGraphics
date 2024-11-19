#include <vector>
#include <string>
#include <iostream>
#include "vec.h"

struct Triangle
{
    vec4 A,B,C;
};

std::vector<Triangle> Clip(const Triangle& tri)
{   
    std::vector<Triangle> result;

    int pointsOut = 0;
    // point A
    bool AisOut = false;
    for(int i = 0; i < 2; i++){
        if (tri.A[i] > tri.A[3]){
            AisOut = true;
        }
    }
    if(AisOut){
        pointsOut++;
    }

    // point B
    bool BisOut = false;
    for(int i = 0; i < 2; i++){
        if (tri.B[i] > tri.B[3]){
            BisOut = true;
        }
    }
    if(BisOut){
        pointsOut++;
    }
    
    // point C
    bool CisOut = false;
    for(int i = 0; i < 2; i++){
        if (tri.C[i] > tri.C[3]){
            CisOut = true;
        }
    }
    if(CisOut){
        pointsOut++;
    }

    if(pointsOut == 0){
        result = {};
    } 
    else if (pointsOut == 3){
        result.push_back(tri);
    } 
    else if (pointsOut == 2){
        if(!AisOut){
            tri.B = ;
        }
        if(!BisOut){

        }
        if(!CisOut){

        }
    } 
    else if (pointsOut == 1){

    }

    return result;
}
