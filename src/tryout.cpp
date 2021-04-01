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

#include "meanAngleFuncs.h"

using namespace std;

int main(int argc, char *argv[]) {

  float angles[] = { 0,45,90,135,180,225,270,315 };
  int n = 8;
  printArray(angles, n);

  translateAngle(angles, n, 0);

  printArray(angles, n);






}