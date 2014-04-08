#include "Lolcode.h"

void parse(std::string input){

    using boost::spirit::qi::phrase_parse;

    typedef std::string::const_iterator iterator_type;
    typedef lc::lolcode<iterator_type> lolcode_type;
    lolcode_type lolcode;

    iterator_type begin = input.begin();
    iterator_type end = input.end();

    lc::commentAndBlankSkipper<std::string::const_iterator> skipper;

    bool r = phrase_parse(begin, end, lolcode, skipper);

    if (r && begin == end){
        std::cout << "Succcess!" << std::endl << std::endl;
    }
    else {
        std::cout << "Fail!" << std::endl << std::endl;
    }

}

int main(int argc, char* args){
    using std::cout;
    cout << "as given in the task: ";
    parse("HAI \n I HAS A var \n var R 1.0 \n KTHXBYE");
    //parse("HAI \n I HAS A var \n var R 1 \n KTHXBYE");
    //parse("HAI \n I HAS A var \n var R \"hello\" \n KTHXBYE");
    //parse("HAI \n I HAS A var \n var R WIN \n KTHXBYE");
    //parse("HAI \n I HAS A var \n var R FAIL \n KTHXBYE");
    

    int s;
    std::cin >> s;

    return 0;
}