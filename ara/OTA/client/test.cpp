#include "cpr/cpr.h"
#include "iostream"
#include "fstream"
int main()
{
    auto responce=cpr::Get(cpr::Url{"http://127.0.0.1:8000/meta_data_send"});
    std::cout<<responce.text<<std::endl;
    std::ofstream myfile;
    myfile.open("all.txt");
    myfile<<responce.text;
    myfile.close();
}