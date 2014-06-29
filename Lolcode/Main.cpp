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

    cout << "BOTH OF (FAIL && FAIL): ";
    parse("HAI \n VISIBLE BOTH OF FAIL AN FAIL\n KTHXBYE");
    cout << "BOTH OF (WIN && FAIL): ";
    parse("HAI \n VISIBLE BOTH OF WIN AN FAIL\n KTHXBYE");
    cout << "BOTH OF (FAIL && WIN): ";
    parse("HAI \n VISIBLE BOTH OF FAIL AN WIN\n KTHXBYE");
    cout << "BOTH OF (WIN && WIN): ";
    parse("HAI \n VISIBLE BOTH OF WIN AN WIN\n KTHXBYE");
    
    cout << std::endl;
    cout << "EITHER OF (FAIL && FAIL): ";
    parse("HAI \n VISIBLE EITHER OF FAIL AN FAIL\n KTHXBYE");
    cout << "EITHER OF (WIN && FAIL): ";
    parse("HAI \n VISIBLE EITHER OF WIN AN FAIL\n KTHXBYE");
    cout << "EITHER OF (FAIL && WIN): ";
    parse("HAI \n VISIBLE EITHER OF FAIL AN WIN\n KTHXBYE");
    cout << "EITHER OF (WIN && WIN): ";
    parse("HAI \n VISIBLE EITHER OF WIN AN WIN\n KTHXBYE");

    cout << std::endl;
    cout << "WON OF (FAIL && FAIL): ";
    parse("HAI \n VISIBLE WON OF FAIL AN FAIL\n KTHXBYE");
    cout << "WON OF (WIN && FAIL): ";
    parse("HAI \n VISIBLE WON OF WIN AN FAIL\n KTHXBYE");
    cout << "WON OF (FAIL && WIN): ";
    parse("HAI \n VISIBLE WON OF FAIL AN WIN\n KTHXBYE");
    cout << "WON OF (WIN && WIN): ";
    parse("HAI \n VISIBLE WON OF WIN AN WIN\n KTHXBYE");

    cout << std::endl;
    cout << "NOT FAIL: ";
    parse("HAI \n VISIBLE NOT FAIL\n KTHXBYE");
    cout << "NOT WIN: ";
    parse("HAI \n VISIBLE NOT WIN\n KTHXBYE");
    
    cout << std::endl;
    cout << "ALL OF FAIL && FAIL && WIN: ";
    parse("HAI \n VISIBLE ALL OF FAIL AN FAIL AN WIN\n KTHXBYE");
    cout << "ALL OF WIN && FAIL && WIN: ";
    parse("HAI \n VISIBLE ALL OF WIN AN FAIL AN WIN\n KTHXBYE");
    cout << "ALL OF WIN && WIN && FAIL: ";
    parse("HAI \n VISIBLE ALL OF WIN AN WIN AN FAIL\n KTHXBYE");
    cout << "ALL OF WIN && WIN && WIN: ";
    parse("HAI \n VISIBLE ALL OF WIN AN WIN AN WIN\n KTHXBYE");
    
    cout << std::endl;
    cout << "ANY OF WIN && FAIL && WIN: ";
    parse("HAI \n VISIBLE ANY OF WIN AN FAIL AN WIN\n KTHXBYE");
    cout << "ANY OF FAIL && FAIL && WIN: ";
    parse("HAI \n VISIBLE ANY OF FAIL AN FAIL AN WIN\n KTHXBYE");
    cout << "ANY OF FAIL && WIN && FAIL: ";
    parse("HAI \n VISIBLE ANY OF FAIL AN WIN AN FAIL\n KTHXBYE");
    cout << "ANY OF FAIL && FAIL && WIN: ";
    parse("HAI \n VISIBLE ANY OF FAIL AN FAIL AN FAIL\n KTHXBYE");

    cout << std::endl;
    cout << "bool expression used in math expression: ";
    parse("HAI \n VISIBLE SUM OF EITHER OF WIN AN FAIL AN 2\n KTHXBYE");

    int s;
    std::cin >> s;

    return 0;
}