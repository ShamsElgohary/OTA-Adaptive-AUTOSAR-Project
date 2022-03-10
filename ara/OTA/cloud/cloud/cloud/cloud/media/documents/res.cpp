
#include <iostream>
#include <cpr.h>

int main(int argc, char** argv) {
    auto response = cpr::Get(cpr::Url{"http://127.0.0.1:8000/download/"});
    std::cout << response.text << std::endl;
}