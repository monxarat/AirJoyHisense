#include "Plist.hpp"
#include "TheBoost.hpp"

int main(void)
{

    std::string filename("f:/1.plist");

    try
    {
        std::map<std::string, boost::any> dict; 
        Plist::readPlist(filename, dict);

        const std::string & contentLocation = boost::any_cast<const std::string&>(dict.find("Content-Location")->second);
        double startPosition = boost::any_cast<const double&>(dict.find("Start-Position")->second);


        std::cout << "contentLocation: " << contentLocation << std::endl;
        std::cout << "startPosition: " << startPosition << std::endl;
    }
    catch (std::exception &e)
    {
        std::cout << "read Plist error : " << e.what() << std::endl;
    }


    return 0;
}
