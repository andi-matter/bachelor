//C, C++
#include <stdio.h>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <sstream>
#include <numeric>
#include <tuple>
#include <map>

// root
#include <TMath.h>

using namespace std;

float translateAngle(float inputAngle, float zeroAngle); 
void translateAngle(float *inputAngles ,int sizeAngles, float zeroAngle);
void cartesianToPolar(int numberEntries, float* inputX, float* inputY, float* outputAngles);
float cartesianToPolar(float inputX, float inputY);

/**
 * @brief Convert cartesian coordinates to polar angle [0, 360)
 * 
 * @param inputX 
 * @param inputY 
 * @return float 
 */
float cartesianToPolar(float inputX, float inputY) {
    float pi = TMath::Pi();
    float outputAngle;
    if (inputX > 0 && inputY >= 0) {
        outputAngle = (TMath::ATan(inputY / inputX) * 180.0 / pi);
    } 
    else if (inputX < 0) {
        outputAngle = (TMath::ATan(inputY / inputX)  + pi) * 180.0 / pi;
    } 
    else if (inputX > 0 && inputY < 0) {
        outputAngle = (TMath::ATan(inputY / inputX)  + 2*pi) * 180.0 / pi;
    } 
    else if (inputX == 0 && inputY > 0) {
        outputAngle = 0.5 * 180.0;
    }
    else if (inputX == 0 && inputY < 0) {
        outputAngle = -1.5* 180.0;
    }
    return outputAngle;
}

/**
 * @brief Convert list of cartesian coordinates to corresponding polar angle [0, 360)
 * 
 * @param numberEntries 
 * @param inputX 
 * @param inputY 
 * @param outputAngles 
 */
void cartesianToPolar(int numberEntries, float* inputX, float* inputY, float* outputAngles) {
    float pi = TMath::Pi();
    for (int i=0; i<numberEntries; i++) {
        if (inputX[i] > 0 && inputY[i] >= 0) {
          outputAngles[i] = (TMath::ATan(inputY[i] / inputX[i]) * 180.0 / pi);
        } 
        else if (inputX < 0) {
          outputAngles[i] = (TMath::ATan(inputY[i] / inputX[i])  + pi) * 180.0 / pi;
        } 
        else if (inputX > 0 && inputY < 0) {
          outputAngles[i] = (TMath::ATan(inputY[i] / inputX[i])  + 2*pi) * 180.0 / pi;
        } 
        else if (inputX[i] == 0 && inputY[i] > 0) {
          outputAngles[i] = 0.5 * 180.0;
        }
        else if (inputX[i] == 0 && inputY[i] < 0) {
          outputAngles[i] = -1.5* 180.0;
        }
    }
}


void printArray(float *array, int arraySize);
/**
 * @brief print array, one line no commas
 * 
 * @param array 
 * @param arraySize 
 */
void printArray(float *array, int arraySize) {
    for (int i=0; i<arraySize; i++) {
        cout << array[i] << " ";
    }
    cout << endl;
}

/**
 * @brief projects angle into (-180, 180] range, where the zeroAngle is the new 0
 * 
 * @param inputAngle in degrees
 * @param zeroAngle new zero
 * @return float    translated angle
 */
float translateAngle(float inputAngle, float zeroAngle) {
    float newAngle = inputAngle - zeroAngle;
    if (newAngle <= -180.0) newAngle += 360.0;
    else if (newAngle > 180.0) newAngle -= 360;
    return newAngle;
}

/**
 * @brief projects angles into (-180, 180] range, where the zeroAngle is the new 0
 * 
 * @param inputAngles in degrees
 * @param sizeAngles size of angle array
 * @param zeroAngle new zero
 */
void translateAngle(float *inputAngles, int sizeAngles, float zeroAngle) {
    for (int i=0; i<sizeAngles; i++) {
        inputAngles[i] -= zeroAngle;
        if (inputAngles[i] <= -180.0) inputAngles[i] += 360.0;
        else if (inputAngles[i] > 180.0) inputAngles[i] -= 360.0;
    }
}