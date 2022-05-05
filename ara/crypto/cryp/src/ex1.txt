#include"iostream"
#include <unistd.h>

using namespace std;
int main ()
{
while(1)
{
std::cout<<"hello from executable"<<"\n";
usleep(50000);
}

return 0;
}