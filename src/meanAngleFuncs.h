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

using namespace std;

float translateAngle(float inputAngle, float zeroAngle); 
void translateAngle(float *inputAngles, float zeroAngle);

void printArray(float *array, int arraySize);

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