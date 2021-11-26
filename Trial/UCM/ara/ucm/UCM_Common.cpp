#include "includes/UCM_Common.hpp"

/* THESE PATHS ARE CONSTANTS AND THEY DO NOT CHANGE */
const string UCM_Path = GetCurrentDirectory() ;
const string ZIP_PackagesPath = UCM_Path + "/ZIP_Packages/";
const string fileSystemPath = "/home/shams/VS_Workspace/Latest/UCM/executables";
string command;

/*
string GetFileSystemPath()
{
  string fileSystemPath { GetCurrentDirectory() };
  command = "cd ../..";
  system(&command[0]);
  fileSystemPath = fileSystemPath + "\\executables";
  return fileSystemPath;
}
*/

/* Used to get the path of our current directory */
string GetCurrentDirectory()
{
    char directoryPath[FILENAME_MAX];
    getcwd(directoryPath,FILENAME_MAX);
    return directoryPath;
}

bool IsPathExist(const std::string &s)
{
  struct stat buffer;
  return (stat (s.c_str(), &buffer) == 0);
}

string Convert2StringID(ara::ucm::TransferIdType &TransferID)
{
    string StringID = to_string(TransferID[0]);   
    for (uint8_t i =1 ; i < 16 ; i++)
    {
      StringID += '-' + to_string(TransferID[i]);
    }
    return StringID;
}

json jsonReadFromFile(string jsonName)
{
    ifstream i(jsonName);
    json j;
    i >> j;
    return j;
}
void jsonWriteInFile(string jsonName, json j)
{
    ofstream o(jsonName);
    o << setw(4) << j << endl;
    o.close();
}
