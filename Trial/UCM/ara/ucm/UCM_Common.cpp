#include "includes/UCM_Common.hpp"


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
