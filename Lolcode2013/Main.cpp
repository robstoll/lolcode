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

    cout << "SUM OF two int (4 + 5): ";
    parse("HAI \n VISIBLE SUM OF 4 AN 5 \n KTHXBYE");
    cout << "SUM OF two double (1.0 + 2.3): ";
    parse("HAI \n VISIBLE SUM OF 1.0 AN 2.3 \n KTHXBYE");
    cout << "SUM OF int (1) and double 2.3: ";
    parse("HAI \n VISIBLE SUM OF 1 AN 2.3 \n KTHXBYE");
    cout << "SUM OF WIN (true=1) and int 5: ";
    parse("HAI \n VISIBLE SUM OF WIN AN 5 \n KTHXBYE");
    cout << "SUM OF FAIL (false=0) and double 2.3: ";
    parse("HAI \n VISIBLE SUM OF FAIL AN 2.3 \n KTHXBYE");
    
    cout << std::endl;
    cout << "SUM OF int 1 as string and int 3: ";
    parse("HAI \n VISIBLE SUM OF \"1\" AN 3 \n KTHXBYE");
    cout << "SUM OF int 1 as string and double 2.3: ";
    parse("HAI \n VISIBLE SUM OF \"1\" AN 2.3 \n KTHXBYE");
    cout << "SUM OF double 1.3 as string and double 2.3: ";
    parse("HAI \n VISIBLE SUM OF \"1.3\" AN 2.3 \n KTHXBYE");
    cout << "SUM OF int 1 as string and double 2.3 as string: ";
    parse("HAI \n VISIBLE SUM OF \"1\" AN \"2.3\" \n KTHXBYE");
    cout << std::endl;
    cout << "SUM OF multiple numbers (WIN+FAIL+4+2.3+\"2.3\"+\"7\"): ";
    parse("HAI \n VISIBLE SUM OF WIN AN FAIL AN 4 AN 2.3 AN \"2.3\" AN \"7\" \n KTHXBYE");
    
    cout << std::endl;
    cout << "DIFF OF two ints (4-5)";
    parse("HAI \n VISIBLE DIFF OF 4 AN 5\n KTHXBYE");
    cout << "PRODUKT OF two doubles (1.3*2.5)";
    parse("HAI \n VISIBLE PRODUKT OF 1.3 AN 2.5\n KTHXBYE");
    cout << "QUOSHUNT OF two int as string (\"5\" / \"2\")";
    parse("HAI \n VISIBLE QUOSHUNT OF \"5\" AN \"2\" \n KTHXBYE");
    cout << "QUOSHUNT OF int 5 and double 2.5";
    parse("HAI \n VISIBLE QUOSHUNT OF 5 AN 2.5 \n KTHXBYE");
    cout << "MOD OF two int (5 % 3): ";
    parse("HAI \n VISIBLE MOD OF 5 AN 3\n KTHXBYE");
    cout << "MOD OF int 7 and double 2.5: ";
    parse("HAI \n VISIBLE MOD OF 7 AN 2.5\n KTHXBYE");
    cout << "BIGGR OF numbers (2, 4.5, WIN, FAIL, 3, 1.0): ";
    parse("HAI \n VISIBLE BIGGR OF 2 AN 4.5 AN WIN AN FAIL AN 3 AN 1.0\n KTHXBYE");
    cout << "SMALLR OF numbers (2, 4.5, WIN, FAIL, 3, 1.0): ";
    parse("HAI \n VISIBLE SMALLR OF 2 AN 4.5 AN WIN AN FAIL AN 3 AN 1.0\n KTHXBYE");
    
    cout << std::endl;
    cout << "NESTED addition and multiplication: ";
    parse("HAI \n VISIBLE SMALLR OF 2 AN 4.5 AN WIN AN FAIL AN 3 AN 1.0\n KTHXBYE");

    

    int s;
    std::cin >> s;

    return 0;
}