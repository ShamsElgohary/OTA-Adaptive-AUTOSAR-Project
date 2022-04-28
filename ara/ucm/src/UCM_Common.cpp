#include "../includes/UCM_Common.hpp"

/* THESE PATHS ARE CONSTANTS AND THEY DO NOT CHANGE */


const string ProjectPath = GetProjectPath();
const string UCM_Path = ProjectPath + "/ucm/";
const string fileSystemPath = ProjectPath + "executables";
const string ZIP_PackagesPath = ProjectPath + "executables/ucm/ZIP_Packages";
const string fileBackupPath = ProjectPath + "executables/Backup";
const string ProcessListPath = ProjectPath + "executables/etc/system/";

string command;

static string GetProjectPath()
{
  string origin = GetCurrentDirectory();
  int pos = origin.find("executables");
  string Path {origin.substr(0, pos)};
  return Path;
}


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

string Convert2StringID(ara::ucm::TransferIdType TransferID)
{
    string StringID = "";   
    for (auto i : TransferID)
    {
      StringID += '-' + to_string(i);
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

