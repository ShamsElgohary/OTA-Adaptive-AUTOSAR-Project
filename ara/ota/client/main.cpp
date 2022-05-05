#include "cpr/cpr.h"
#include "iostream"
#include <filesystem>
#include "../../crypto/cryp/include/crypto_provider.hpp"
#include "PackageManagerProxy.hpp"
#include "execution_client.hpp"
#include "fstream"
#include <string>
#include <vector>
using std::filesystem::current_path;
using namespace std;
using namespace ara::ucm::pkgmgr::proxy;
using namespace ara::com::proxy;
using namespace ara::crypto;


class CLIENT_OTA
{
    
private:
    vector<SwClusterInfoType> trail;
    vector<SwClusterInfoType> UCM_trail;
    PackageManagerProxy *RecService = nullptr;
    

public:
    
    CLIENT_OTA()
    {
        ara::com::ServiceHandleContainer<ProxyBase::HandleType> handles = PackageManagerProxy::FindService();
        if (handles.size())
        {
            this->RecService = new PackageManagerProxy(handles[0]);
        }
        else
        {
            cout<<"NO UCM";
        }
    }
    
    string hash(string packagename)
    {
        //string filename = packagename + ".zip";
        string url_final = "http://127.0.0.1:8000/hash/" + packagename + ".zip";
        auto responce = cpr::Get(cpr::Url{url_final});
        // std::cout<<responce.text<<std::endl;
        return responce.text;
    }

    string crypto_get_hash(string packagename)
    {
        std::string path =  "/home/yasser/Documents/GitHub/OTA-Adaptive-AUTOSAR-Project/ara/ota/client/ex1.zip";
    
        ifstream ifs(path, ios::binary | ios::ate);
        ifstream::pos_type pos = ifs.tellg();
        std::vector<char> result(pos);
        ifs.seekg(0, ios::beg);
        ifs.read(&result[0], pos);
        
        char* result_arr=nullptr;
        result_arr=&result[0];

        std::vector <unsigned char> Digest;

        cryp::CryptoProvider cryp_provider;
        cryp::HashFunctionCtx::Uptr hash = cryp_provider.CreateHashFunctionCtx(cryp::HashCtx_AlgID::SHA1_ID);
        hash->Start();
        hash->Update(ReadOnlyMemRegion(result_arr), result.size());
        
        //std::cout << "Message: " << msg << std::endl;

        Digest=hash->Finish();
        
        std::stringstream ss;
        
        for (int i = 0; i < Digest.size(); i++)
        {
            //std::cout << static_cast<unsigned>(Digest[i]) << "    ";
            if(Digest[i]>=0 && Digest[i] <= 15)
            {
                ss<<"0"<< std::hex<<int(Digest[i]);
            }
            else
            {
                ss<< std::hex<<int(Digest[i]);
            }
            
            
            
        }

        std::string res(ss.str());
        
        return res;


    }
    bool compare_hash(string packagename)
    {
        std::string hash_server= hash(packagename);
        std::string hash_cryp=crypto_get_hash(packagename);
        //cout << hash_server<< endl;
        //cout << hash_cryp << endl;
        if(hash_server == hash_cryp)
        {return 1;}
        else{
            return 0;
        }
    }

    void parse_meta_data()
    {

        ifstream myfile;
        myfile.open("meta_data.txt");
        string myline;

        while (myfile)
        {
            SwClusterInfoType x;
            getline(myfile, myline);
            if (myline == "")
            {
            }
            else
            {
                int it = myline.find("#");
                string file_name = myline.substr(0, it);
                string number = myline.substr(it + 1);
                int num = stoi(number);
                x.Name = file_name;
                x.Version = num;
                trail.push_back(x);
            }
        }
    }
    

    void get_meta_data()
    {
        string url_final = "https://coretestsystem.herokuapp.com/meta_data_send";
        auto responce = cpr::Get(cpr::Url{url_final});
        // std::cout<<responce.text<<std::endl;
        std::ofstream myfile;
        myfile.open("meta_data.txt");
        myfile << responce.text;
        myfile.close();
    }

    void download(string packagename)
    {
        string filename = packagename + ".zip";
        string url_final = "https://coretestsystem.herokuapp.com/download/" + packagename + ".zip";
        auto responce = cpr::Get(cpr::Url{url_final});
        // std::cout<<responce.text<<std::endl;
        std::ofstream myfile;
        myfile.open(filename);
        myfile << responce.text;
        myfile.close();
    }

    void Transfer2UCM(string Package_Name)
    {
        string Final_Package_Name = Package_Name + ".zip";
        ifstream ifs(Final_Package_Name, ios::binary | ios::ate);
        ifstream::pos_type pos = ifs.tellg();
        vector<char> result(pos);
        ifs.seekg(0, ios::beg);
        ifs.read(&result[0], pos);
        ByteVectorType ByteData;
        TransferStartOutput transfer_start = RecService->TransferStart(result.size());
        uint32_t Number_of_BLocks = ceil((float)result.size() / (float)transfer_start.BlockSize);
        uint32_t BlockCounter = 0;
        uint64_t BlockSize = transfer_start.BlockSize;
        int i = 0;

        while (BlockCounter < Number_of_BLocks)
        {
            for (uint32_t j = 0; (j < BlockSize) && ((i + j) < result.size()); j++)
            {
                ByteData.push_back(result[i + j]);
            }

            RecService->TransferData(transfer_start.id, ByteData, BlockCounter);
            BlockCounter++;
            ByteData.clear();
            i += BlockSize;
        }

        RecService->TransferExit(transfer_start.id);
        RecService->ProcessSwPackage(transfer_start.id);
        RecService->Activate();
        // RecService.Rollback();
        RecService->Finish();
    }
    void Compare()
    {
        for (auto cluster : trail)
        {
            for (auto UCM_Cluster : UCM_trail)
            {
                if (cluster.Name == UCM_Cluster.Name)
                {
                    if (stoi(cluster.Version) <= stoi(UCM_Cluster.Version))
                    {
                        continue;
                    }
                    else
                    {
                        
                        string pckg = cluster.Name + "#" + cluster.Version;
                        if(compare_hash(pckg))
                        {
                        download(pckg);
                        this->Transfer2UCM(pckg);
                        }
                        else
                        {
                            cout << "Error: Different hash "
                        }
                    }
                }
            }
        }
    }
    void Get_UCM_Clusters()
    {
        GetSwClusterInfoOutput out = RecService->GetSwClusterInfo();
        UCM_trail = out.vectorOfClusterInfo;
    }
    ~CLIENT_OTA() {}

    void run()
    {
        get_meta_data();
        cout << "1" << endl;
        parse_meta_data();
        cout << "2" << endl;
        Get_UCM_Clusters();
        cout << "3" << endl;
        Compare();
    }
    
    
};

int main()
{
    CLIENT_OTA x;
    x.run();
    

    
    
    return 0;
}
