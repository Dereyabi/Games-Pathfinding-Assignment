#pragma once

#include <iostream>
#include <fstream>
using namespace std;

const int LoadError = -1;
const int MAX_DATA = 1000;

int LoadMap(string mapFile, int data[]);