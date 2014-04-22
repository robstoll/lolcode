#ifndef lolcode_h
#define lolcode_h

#include <map>
#include <tuple>
#include <boost\spirit\include\qi.hpp>
#include <boost\spirit\include\support_ascii.hpp>
#include <boost\spirit\include\phoenix.hpp>
#include <boost\spirit\include\qi_numeric.hpp>
#include "LolcodeTypes.h"

namespace lc {
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;
    namespace phx = boost::phoenix;
    namespace fusion = boost::fusion;
    typedef std::string::const_iterator string_itr;

    std::map<std::string, LolcodeType*>* vars;
    NOOB undefined;

    template<class Iterator>
    struct commentAndBlankSkipper : public qi::grammar<Iterator> {
        qi::rule<Iterator> skip;
        commentAndBlankSkipper() : commentAndBlankSkipper::base_type(skip) {
            skip = ascii::blank
                 | ascii::string("BTW") >> *(ascii::char_ - qi::eol - qi::eoi)
                 ;
        }
    };

    struct new_var{
        void operator()(std::string const& id, qi::unused_type, qi::unused_type) const{
            vars->insert(std::pair<std::string, LolcodeType*>(id, &undefined));
        }
    };

    //lazy function
    struct assign_var_impl{
        template <class Arg1, class Arg2>
        struct result{
            typedef void type;
        };

        template<class Arg1, class Arg2>
        void operator()(Arg1 varId, Arg2 value) const{
            auto it = vars->find(varId);
            if (it != vars->end()) {
               it->second = value;
            } else {
                std::string msg = "cannot assign to an undeclared variable \"" 
                                      + varId + "\"";
                throw std::exception(msg.c_str());
            }
        }
    };
    phx::function<assign_var_impl> assign_var;


    //lazy function
    struct resolve_var_impl{
        template <class Arg>
        struct result{
            typedef LolcodeType* type;
        };

        template<class Arg>
        LolcodeType* operator()(Arg arg1) const{
            auto it = vars->find(arg1);
            if (it != vars->end()) {
                return it->second;
            }
            else {
                std::string msg = "could not resolve variable \"" + arg1 + "\"";
                throw std::exception(msg.c_str());
            }
        }
    };
    phx::function<resolve_var_impl> resolve_var;

    //lazy function
    struct minimum_impl{
        template <class Arg1, class Arg2>
        struct result{
            typedef NUMBER type;
        };

        template<class Arg1, class Arg2>
        NUMBER operator()(Arg1 number1, Arg2 number2) const{
            return number1 < number2 ? number1 : number2;
        }
    };
    phx::function<minimum_impl> minimum;

    //lazy function
    struct maximum_impl{
        template <class Arg1, class Arg2>
        struct result{
            typedef NUMBER type;
        };

        template<class Arg1, class Arg2>
        NUMBER operator()(Arg1 number1, Arg2 number2) const{
            return number1 > number2 ? number1 : number2;
        }
    };
    phx::function<maximum_impl> maximum;

    struct print_expr{
        void operator()(LolcodeType* const& expr, qi::unused_type, qi::unused_type) const{
            auto troof = dynamic_cast<TROOF*>(expr);
            if (troof){
                std::cout << troof->value;
                return;
            }
            auto numbr = dynamic_cast<NUMBR*>(expr);
            if (numbr){
                std::cout << numbr->value;
                return;
            }
            auto numbar = dynamic_cast<NUMBAR*>(expr);
            if (numbar){
                std::cout << numbar->value;
                return;
            }
            auto yarn = dynamic_cast<YARN*>(expr);
            if (yarn){
                std::cout << yarn->value;
                return;
            }

            auto number = dynamic_cast<NUMBER*>(expr);
            if (number){
                if (number->isInt){
                    std::cout << number->value.i;
                } else {
                    std::cout << number->value.d;
                }
                return;
            }

            auto noob = dynamic_cast<NOOB*>(expr);
            if (noob){
                std::cout << "undefined";
                return;
            }
        }
    };

    //Copied from http://stackoverflow.com/questions/3125582/boost-spirit-qi-numeric-parsing-of-integer-and-floating-points
    template <typename T>
    struct strict_real_policies : qi::real_policies<T>
    {
        static bool const expect_dot = true;
    };

    qi::real_parser< double, strict_real_policies<double> > real;

    template<class Iterator>
    struct lolcode : qi::grammar<Iterator, void(), commentAndBlankSkipper<Iterator>>
    {

        lolcode() : lolcode::base_type(start){
            using qi::lexeme;
            using ascii::char_;
            using ascii::string;
            using ascii::alpha;
            using ascii::blank;
            using qi::double_;
            using qi::int_;
            using qi::eol;
            using qi::eoi;

            quoted_string = lexeme['"' >> +(char_ - '"') >> '"'];
            quoted_double = lexeme['"' >> double_[qi::_r0 = qi::_1]  >> '"'];
            quoted_int = lexeme['"' >> int_[qi::_r0 = qi::_1] >> '"'];

            identifier 
                    = lexeme[
                            (alpha || char_('_')) >> *(alpha || char_('_') || qi::digit)
                      ]
                    ;

            bool_   = string("WIN")[qi::_r0=true] 
                    | string("FAIL")[qi::_r0=false]
                    ;

            start   = *eol >> string("HAI") >> eol >> *statement >> end
                    ;

            end     = "KTHXBYE" >> *eol >> eoi
                    ;

            statement  
                    = canHas >> eol
                    | visible >> eol
                    | varDecl >> eol
                    | assign >> eol
                    | mathStat >> eol
                    | eol
                    ;
            
            canHas  = string("CAN") >> string("HAS") >> identifier >> char_('?')
                    ;

            visible = "VISIBLE" >> +expr[print_expr()]
                       >>  (   char_('!')
                           |   qi::eps[phx::ref(std::cout) << std::endl]
                           )
                    ;
            
            varDecl
                    = string("I") >> string("HAS") >> string("A") 
                       >> (
                                identifier[new_var()] >> 
                                -(string("ITZ") >> expr[qi::_a = qi::_1])
                          )
                          [if_(qi::_2)[assign_var(qi::_1, qi::_a)]]
                    ;
            
            assign  = (identifier >> string("R") >> expr)[assign_var(qi::_1, qi::_3)]
                    ;

            mathStat 
                    = addition[qi::_r0 = phx::new_<NUMBER>(qi::_1)]
                    | substraction[qi::_r0 = phx::new_<NUMBER>(qi::_1)]
                    | multiplication[qi::_r0 = phx::new_<NUMBER>(qi::_1)]
                    | division[qi::_r0 = phx::new_<NUMBER>(qi::_1)]
                    | mod[qi::_r0 = phx::new_<NUMBER>(qi::_1)]
                    | min[qi::_r0 = phx::new_<NUMBER>(qi::_1)]
                    | max[qi::_r0 = phx::new_<NUMBER>(qi::_1)]
                    ;

            addition    
                    = string("SUM") >> string("OF") >> mathExpr[qi::_r0 = qi::_1] 
                        >> +(string("AN") >> mathExpr[qi::_r0 += qi::_1]) 
                        >> -string("MKAY")
                    ;

            substraction
                    = string("DIFF") >> string("OF") >> mathExpr[qi::_r0 = qi::_1]
                        >> +(string("AN") >> mathExpr[qi::_r0 -= qi::_1]) 
                        >> -string("MKAY")
                    ;

            multiplication
                    = string("PRODUKT") >> string("OF") >> mathExpr[qi::_r0 = qi::_1]
                        >> +(string("AN") >> mathExpr[qi::_r0 *= qi::_1]) 
                        >> -string("MKAY")
                    ;

            division
                    = string("QUOSHUNT") >> string("OF") >> mathExpr[qi::_r0 = qi::_1]
                        >> +(string("AN") >> mathExpr[qi::_r0 /= qi::_1]) 
                        >> -string("MKAY")
                    ;
            
            mod
                    = string("MOD") >> string("OF") >> mathExpr[qi::_r0 = qi::_1]
                        >> +(string("AN") >> mathExpr[qi::_r0 %= qi::_1]) 
                        >> -string("MKAY")
                    ;

            min
                    = string("SMALLR") >> string("OF") >> mathExpr[qi::_r0 = qi::_1]
                     >> +(string("AN") >> mathExpr[qi::_r0 = minimum(qi::_r0, qi::_1)]) 
                     >> -string("MKAY")
                    ;

            max
                    = string("BIGGR") >> string("OF") >> mathExpr[qi::_r0 = qi::_1]
                        >> +(string("AN") >> mathExpr[qi::_r0 = maximum(qi::_r0, qi::_1)]) 
                        >> -string("MKAY")
                    ;
            
            mathExpr
                    = addition[qi::_r0 = qi::_1]
                    | substraction[qi::_r0 = qi::_1]
                    | multiplication[qi::_r0 = qi::_1]
                    | division[qi::_r0 = qi::_1]
                    | mod[qi::_r0 = qi::_1]
                    | min[qi::_r0 = qi::_1]
                    | max[qi::_r0 = qi::_1]
                    | bool_[qi::_r0 = phx::construct<NUMBER>(qi::_1)]
                    | real[qi::_r0 = phx::construct<NUMBER>(qi::_1)]
                    | int_[qi::_r0 = phx::construct<NUMBER>(qi::_1)]
                    | quoted_double[qi::_r0 = phx::construct<NUMBER>(qi::_1)]
                    | quoted_int[qi::_r0 = phx::construct<NUMBER>(qi::_1)]
                    ;

            expr    = bool_[qi::_r0 = phx::new_<TROOF>(qi::_1)]
                    | real[qi::_r0 = phx::new_<NUMBAR>(qi::_1)]
                    | int_[qi::_r0 = phx::new_<NUMBR>(qi::_1)]
                    | quoted_string[qi::_r0 = phx::new_<YARN>(qi::_1)]
                    | mathStat[qi::_r0 = qi::_1]
                    | identifier[qi::_r0 = resolve_var(qi::_1)]
                    ;

            vars = new std::map<std::string, LolcodeType*>();
        }

        qi::rule<Iterator, void(), commentAndBlankSkipper<Iterator>> start;
        qi::rule<Iterator, void(), commentAndBlankSkipper<Iterator>> end;
        qi::rule<Iterator, void(), commentAndBlankSkipper<Iterator>> statement;
        qi::rule<Iterator, void(), commentAndBlankSkipper<Iterator>> canHas;
        qi::rule<Iterator, void(), commentAndBlankSkipper<Iterator>> visible;
        qi::rule<Iterator, void(), commentAndBlankSkipper<Iterator>, boost::spirit::locals<LolcodeType*>> varDecl;
        qi::rule<Iterator, void(), commentAndBlankSkipper<Iterator>> assign;
        qi::rule<Iterator, LolcodeType*(), commentAndBlankSkipper<Iterator>> expr;
        qi::rule<Iterator, std::string(), commentAndBlankSkipper<Iterator>> quoted_string;
        qi::rule<Iterator, double(), commentAndBlankSkipper<Iterator>> quoted_double;
        qi::rule<Iterator, int(), commentAndBlankSkipper<Iterator>> quoted_int;
        qi::rule<Iterator, std::string(), commentAndBlankSkipper<Iterator>> identifier;
        qi::rule<Iterator, bool(), commentAndBlankSkipper<Iterator>> bool_;
        qi::rule<Iterator, NUMBER(), commentAndBlankSkipper<Iterator>> addition;
        qi::rule<Iterator, NUMBER(), commentAndBlankSkipper<Iterator>> substraction;
        qi::rule<Iterator, NUMBER(), commentAndBlankSkipper<Iterator>> multiplication;
        qi::rule<Iterator, NUMBER(), commentAndBlankSkipper<Iterator>> division;
        qi::rule<Iterator, NUMBER(), commentAndBlankSkipper<Iterator>> mod;
        qi::rule<Iterator, NUMBER(), commentAndBlankSkipper<Iterator>> min;
        qi::rule<Iterator, NUMBER(), commentAndBlankSkipper<Iterator>> max;
        qi::rule<Iterator, LolcodeType*(), commentAndBlankSkipper<Iterator>> mathStat;
        qi::rule<Iterator, NUMBER(), commentAndBlankSkipper<Iterator>> mathExpr;
    };
}

#endif
