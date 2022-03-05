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
#include "UCM_Types.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

/* VARIABLES THAT WILL BE USED THROUGHTOUT MOST OF THE UCM FILES */
extern const string UCM_Path;
extern const string ZIP_PackagesPath;
extern const string fileSystemPath;
extern const string fileBackupPath;
extern string command;

/* USED TO GET FILE SYSTEM PATH (ONLY IN THIS FILE) */
static string GetFileSystemPath();

/* Used to get the path of our current directory */
string GetCurrentDirectory();

/* CHECKS IF PATH EXISTS */
bool IsPathExist(const std::string &s);

/* CONVERTS ID INTO A STRING FORMAT */
string Convert2StringID(ara::ucm::TransferIdType &TransferID);

json jsonReadFromFile(string jsonName);

void jsonWriteInFile(string jsonName, json j);

