#include "cpr/cpr.h"
#include "iostream"
#include "fstream"
#include <string>
#include <vector>
using namespace std;
struct meta_data
{
    string name;
    int version;
};


void print(meta_data x){
    cout<<"name= "<<x.name<<'\n';
    cout<<"version= "<<x.version<<'\n';
}

void parse_meta_data(){
    vector <meta_data> trail;
    ifstream myfile; myfile.open("meta_data.txt");
    string myline;

    while ( myfile ) { 
            meta_data x;
            getline (myfile, myline);
            if (myline == "")
            {
            }
            else{
            int it=myline.find("#");
            string file_name=myline.substr(0,it);
            string number=myline.substr(it+1);
            int num=stoi(number);
            x.name=file_name;
            x.version=num;
            trail.push_back(x);
            }
        } 

    for(int i=0; i < trail.size(); i++)
    {
    print(trail[i]);
    }

}


int main()
{
    auto responce=cpr::Get(cpr::Url{"http://127.0.0.1:8000/meta_data_send"});
    std::cout<<responce.text<<std::endl;
    std::ofstream myfile;
    myfile.open("meta_data.txt");
    myfile<<responce.text;
    myfile.close();
    parse_meta_data();
}