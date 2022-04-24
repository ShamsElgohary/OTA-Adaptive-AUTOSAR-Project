#include "../includes/UCM_Common.hpp"

/* THESE PATHS ARE CONSTANTS AND THEY DO NOT CHANGE */
const string UCM_Path = GetUCMPath() ;
const string ZIP_PackagesPath = UCM_Path + "/ZIP_Packages";
const string fileSystemPath = GetFileSystemPath();
const string fileBackupPath = GetFileSystemPath() + "/Backup";
const string ProjectPath = GetProjectPath();
const string ProcessListPath = ProjectPath + "ara/etc/system/";

string command;

static string GetProjectPath()
{
  string origin = GetCurrentDirectory();
  int pos = origin.find("ara");
  string Path {origin.substr(0, pos)};
  return Path;
}


static string GetUCMPath()
{
  string origin = GetCurrentDirectory();
  chdir(  "../" );
  string UCM_Path { GetCurrentDirectory() };
  /* RETURN TO PROGRAM PATH */
  chdir( &origin[0] );
  return UCM_Path;
}



static string GetFileSystemPath()
{
  chdir(  "../../.." );
  string fileSystemPath { GetCurrentDirectory() + "/executables" };
  /* RETURN TO PROGRAM PATH */
  chdir( &UCM_Path[0] );
  return fileSystemPath;
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

