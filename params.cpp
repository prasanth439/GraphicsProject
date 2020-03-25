#include<string>
#include<vector>
#include<glm/glm.hpp>
#include<map>
union Value{
    glm::vec2 val2;
    glm::vec3 val3;
    glm::vec4 val4;
};
class Parameter{
    public:
        Parameter(const char* param_file=nullptr);
    private:
        std::string hello;
        std::map<std::string,Value>  mp;
};