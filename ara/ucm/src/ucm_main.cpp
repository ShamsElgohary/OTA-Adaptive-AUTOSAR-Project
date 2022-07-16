#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include "PackageManager.hpp"
#include "../../exec/include/execution_client.hpp"
#include <unistd.h>

using namespace std;
using namespace ara::ucm;
using namespace ara::exec;
void handle_sigTerm(int sig)
{
    ExecutionClient exec;
    exec.ReportExecutionStaste(ExecutionState::Kterminate);
    exit(1);
}
int main()
{
    struct sigaction sa;
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = handle_sigTerm;
    sigaction(SIGTERM, &sa, NULL);
    sleep(1);
    ara::ucm::pkgmgr::PackageManagerImpl PackageManagerInstance(1, ara::com::MethodCallProcessingMode::kEvent);
    string path = ZIP_PackagesPath + "/PackageTest.zip";
    ara::exec::ExecutionClient exec;
    exec.ReportExecutionStaste(ara::exec::ExecutionState::Krunning);
    PackageManagerInstance.OfferService();
    return 0;
}
