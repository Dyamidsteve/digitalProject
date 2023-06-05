#pragma once
#include <vector>
#include <iostream>
using namespace std;

vector<unsigned char> meanShift(const vector<unsigned char>& image, int width, int height, double spatialRadius, double colorRadius);