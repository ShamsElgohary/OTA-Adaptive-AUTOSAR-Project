/*****************************************************************************/
/* Functional Cluster : UCM
/*
/* File Name: UCM_Common.h
/*
/* Description: Header file that contains all libraries and functions that will be commonly used by the UCM files 
/*
/******************************************************************************/

#pragma once

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <map>
#include <iterator>
#include <string>
#include <vector>
#include <utility>
#include <unistd.h>
#include <iomanip>
#include <sys/stat.h>
#include "nlohmann/json.hpp"
#include "UCM_Types.hpp"

using json = nlohmann::json;

json jsonReadFromFile(string jsonName);

void jsonWriteInFile(string jsonName, json j);

/* Used to get the path of our current directory */
string GetCurrentDirectory();

bool IsPathExist(const std::string &s);
