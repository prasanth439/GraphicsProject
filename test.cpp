#include<iostream>
#include<map>
#include<vector>
using namespace std;

int main(int argc,const char* argv[])
{
    string temp;
    vector<string> s;
    map<string,int> t;
    int count = 0;
    while(getline(cin,temp))
    {
        s.push_back(temp);
        if(t.find(temp)==t.end()){
            t.insert({temp,count++});
        }
    }
    for(auto a:t){
        cout<<a.first<<" "<<a.second<<endl;
    }
    cout<<"###########"<<endl;
    int i= 0;
    for(auto b:s)
    {
        cout<<t[b]<<",";
        i++;
        if(i%3==0)
        {
            cout<<endl;
        }
    }
    return 0;
}