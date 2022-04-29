#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include "PackageManager.hpp"
#include "../../exec/include/execution_client.hpp"

using namespace std;
using namespace ara::ucm;

int main (void)
{
    // ara::exec::ExecutionClient exec;
    // exec.ReportExecutionStaste(ara::exec::ExecutionState::Krunning);
    
    ara::ucm::pkgmgr::PackageManagerImpl PackageManagerInstance(1, ara::com::MethodCallProcessingMode::kEvent);
    string path =  ZIP_PackagesPath + "/PackageTest.zip";
    PackageManagerInstance.OfferService();
    return 0;
<<<<<<< Updated upstream
} 




=======
}
>>>>>>> Stashed changes
