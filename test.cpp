#include<iostream>
#include<map>
#include<vector>
#include<glm/glm.hpp>
using namespace std;

int main(int argc,const char* argv[])
{
    cout<<glm::tan(45.0f)<<endl;
    cout<<glm::tan(glm::radians(45.0f))<<endl;
    return 0;
}