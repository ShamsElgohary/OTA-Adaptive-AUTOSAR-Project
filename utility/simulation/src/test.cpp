#include "../include/sm_logger.hpp"
int main()
{
    /*sm_logger sm(8088);
    sm_functions functions={1,0,0,0};
    std::map<string,string>m;
    m["f1"]="startup";
    m["f2"]="off";
    sm.update_logger(functions,m);*/
    simulation s(8088);
    s.creat_socket();
    s.listen_l();
}