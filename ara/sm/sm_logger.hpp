#pragma once
#include "../../utility/simulation/include/simulation.hpp"
#include <map>
#include "functional"
#include "thread"
struct sm_functions
{
    bool sm_StartUpdateSession;
    bool sm_StopUpdateSession;
    bool sm_PrepareUpdate;
    bool sm_VerifyUpdate;
};

class sm_logger
{
   private:
   sm_functions* functions_state;
   simulation* sim;
   static int fd;
   static std::thread gui_thread;
   public:
   sm_logger(int server_port);
   void update_logger(sm_functions functions,std::map<string,string>fg_states);
   void reset();
   static void gui_receive();
};