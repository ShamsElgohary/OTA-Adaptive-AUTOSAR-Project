#include "cpr/cpr.h"
#include "iostream"
#include "fstream"
int main()
{
    auto responce=cpr::Get(cpr::Url{"http://127.0.0.1:8000/download/karim.zip"});
    std::cout<<responce.text<<std::endl;
    std::ofstream myfile;
    myfile.open("kemz2.zip");
    myfile<<responce.text;
    myfile.close();
}