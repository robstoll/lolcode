#ifndef lolcode_h
#define lolcode_h

#include <map>
#include <tuple>
#include <boost\spirit\include\qi.hpp>
#include <boost\spirit\include\support_ascii.hpp>
#include <boost\spirit\include\phoenix_bind.hpp>
#include <boost\spirit\include\phoenix_core.hpp>
#include <boost\spirit\include\phoenix_object.hpp>
#include <boost\spirit\include\phoenix_operator.hpp>
#include "LolcodeTypes.h"

namespace lc {
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;
    namespace phx = boost::phoenix;
    namespace fusion = boost::fusion;
    typedef std::string::const_iterator string_itr;

    std::map<std::string, LolcodeType*> vars;
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
            vars.insert(std::pair<std::string, LolcodeType*>(id, &undefined));
        }
    };

    struct assign_var{

        void operator()(fusion::vector<std::string, std::string, LolcodeType*> const& v, qi::unused_type, qi::unused_type) const{
            std::string varId = fusion::at_c<0>(v);
            LolcodeType* value = fusion::at_c<2>(v);
            auto it = vars.find(varId);
            if (it != vars.end()) {
               it->second = value;
            } else {
                throw std::exception("cannot assign to an undeclared variable");
            }
        }
    };

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
        }
    };

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
                       >> identifier[new_var()]
                       ;
            
            assign = (identifier >> string("R") >> expr)[assign_var()]
                    ;

            expr    = bool_[qi::_r0 = phx::new_<TROOF>(qi::_1)]
                    | double_[qi::_r0 = phx::new_<NUMBAR>(qi::_1)]
                    | int_[qi::_r0 = phx::new_<NUMBR>(qi::_1)]
                    | quoted_string[qi::_r0 = phx::new_<YARN>(qi::_1)]
                    ;

            //addition = "SUM OF" >> mathexpr[qi::_r0 = qi::_1] >> "AN" >> mathexpr[qi::_r0 += qi::_1];
            //mathexpr = addition | double_;
        }

        qi::rule<Iterator, void(), commentAndBlankSkipper<Iterator>> start;
        qi::rule<Iterator, void(), commentAndBlankSkipper<Iterator>> end;
        qi::rule<Iterator, void(), commentAndBlankSkipper<Iterator>> statement;
        qi::rule<Iterator, void(), commentAndBlankSkipper<Iterator>> canHas;
        qi::rule<Iterator, void(), commentAndBlankSkipper<Iterator>> visible;
        qi::rule<Iterator, void(), commentAndBlankSkipper<Iterator>> varDecl;
        qi::rule<Iterator, void(), commentAndBlankSkipper<Iterator>> assign;
        qi::rule<Iterator, void(), commentAndBlankSkipper<Iterator>> addition;
        qi::rule<Iterator, LolcodeType*(), commentAndBlankSkipper<Iterator>> expr;
        qi::rule<Iterator, std::string(), commentAndBlankSkipper<Iterator>> quoted_string;
        qi::rule<Iterator, std::string(), commentAndBlankSkipper<Iterator>> identifier;
        qi::rule<Iterator, bool(), commentAndBlankSkipper<Iterator>> bool_;
    };
}

#endif
