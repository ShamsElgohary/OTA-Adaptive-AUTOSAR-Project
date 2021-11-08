// UCM.cpp : This file contains the 'main' function. Program execution begins and ends there.


#include <iostream>
#include<unistd.h> 


using namespace std;
int main()
{
	string g_ProjectPath = "home/desktop";
	chdir(g_ProjectPath.c_str());

	cout<< g_ProjectPath <<endl;
	cout << "Hello World!" << endl;
	return 0;
}
