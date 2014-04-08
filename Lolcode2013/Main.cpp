#include "Lolcode.h"

void parse(std::string input){
    using boost::spirit::qi::phrase_parse;
    typedef std::string::const_iterator iterator_type;
    typedef lc::lolcode<iterator_type> lolcode_type;

    try{
        iterator_type begin = input.begin();
        iterator_type end = input.end();
        lolcode_type lolcode;
        lc::commentAndBlankSkipper<iterator_type> skipper;
        bool r = phrase_parse(begin, end, lolcode, skipper);

        if (r && begin == end){
            std::cout << "Succcess!" << std::endl << std::endl;
        }
        else {
            std::cout << "Fail!" << std::endl << std::endl;
        }
    }
    catch (std::exception& e){
        std::cout << "Fail! " << e.what() << std::endl << std::endl;
    }

}

int main(int argc, char* args){
    using std::cout;

    cout << "primitive types: ";
    parse("HAI \n VISIBLE WIN \" \" FAIL \" \" 2  \" \" 3.5 \" Hello\" \n KTHXBYE");
    cout << "variable TROOF - WIN: ";
    parse("HAI \n I HAS A var \n var R WIN \n VISIBLE var \n KTHXBYE");
    cout << "variable TROOF - FAIL: ";
    parse("HAI \n I HAS A var \n var R FAIL \n VISIBLE var \n KTHXBYE");
    cout << "variable NUMBR: ";
    parse("HAI \n I HAS A var \n var R 1 \n VISIBLE var \n KTHXBYE");
    cout << "variable NUMBAR: ";
    parse("HAI \n I HAS A var \n var R 3.5 \n VISIBLE var \n KTHXBYE");
    cout << "variable YARN: ";
    parse("HAI \n I HAS A var \n var R \"hello\" \n VISIBLE var \n KTHXBYE");    
    
    cout << "uninitialised variable: "; //kind of still ok, not really an error
    parse("HAI \n I HAS A var \n VISIBLE var \n KTHXBYE");
    cout << "assign to undefined variable: ";
    parse("HAI \n var R 1.2 \n KTHXBYE");
    cout << "unresolved variable: ";
    parse("HAI \n VISIBLE var \n KTHXBYE");

    int s;
    std::cin >> s;

    return 0;
}