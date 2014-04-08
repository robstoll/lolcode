#include "Lolcode.h"
#include "LolcodeTypes.h"

void parse(std::string input){

    using boost::spirit::qi::phrase_parse;

    typedef std::string::const_iterator iterator_type;
    typedef lc::lolcode<iterator_type> lolcode_type;
    lolcode_type lolcode;

    iterator_type begin = input.begin();
    iterator_type end = input.end();

    lc::commentAndBlankSkipper<iterator_type> skipper;

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
    //cout << "as given in the task: ";
    //parse("HAI \n VISIBLE \"HAI WORLD!\" BTW this is a comment \n KTHXBYE");
    //cout << "before HAI: ";
    //parse("BTW the following is lolocode: \n HAI \n KTHXBYE");
    //cout << "on the same line as HAI: ";
    //parse("HAI BTW the following is lolocode: \n KTHXBYE");
    //cout << "on the same line as KTHXBYE: ";
    //parse("HAI \n KTHXBYE BTW this was lolocode");
    //cout << "after KTHXBYE: ";
    //parse("HAI \n KTHXBYE \n BTW this was lolocode");
    //cout << "a comment on its own: ";
    //parse("HAI \n BTW comment on an own line \n KTHXBYE");

    //cout << "a comment without newline: ";
    //parse("HAI \n BTW misses a new line KTHXBYE");

    cout << "testing primitive types: ";
    parse("HAI \n VISIBLE WIN \" \" FAIL \" \" 2  \" \" 3.5 \" Hello\" \n KTHXBYE");
    parse("HAI \n I HAS A var \n var R 1.0 \n KTHXBYE");
    //parse("HAI \n I HAS A var \n var R 1 \n KTHXBYE");
    //parse("HAI \n I HAS A var \n var R \"hello\" \n KTHXBYE");
    //parse("HAI \n I HAS A var \n var R WIN \n KTHXBYE");
    //parse("HAI \n I HAS A var \n var R FAIL \n KTHXBYE");
    

    int s;
    std::cin >> s;

    return 0;
}