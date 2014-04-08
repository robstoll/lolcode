#ifndef lolcode_h
#define lolcode_h

#include <map>
#include <tuple>
#include <boost\spirit\include\qi.hpp>
#include <boost\spirit\include\support_ascii.hpp>
#include <boost\spirit\include\phoenix.hpp>
#include <boost\spirit\include\phoenix_core.hpp>
#include <boost\spirit\include\phoenix_operator.hpp>
#include <boost\spirit\include\phoenix_stl.hpp>
#include <boost\bind.hpp>

namespace lc {
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;
    namespace phx = boost::phoenix;
    typedef std::string::const_iterator string_itr;

    struct NOOB{
        void* value = nullptr;
    };
    struct YARN{
        std::string value;
        YARN(std::string val) :value(val){}
    };
    struct NUMBR{
        int value;
        NUMBR(int val) :value(val){}
    };
    struct NUMBAR{
        double value;
        NUMBAR(double val) :value(val){}
    };
    struct TROOF{
        bool value;
        TROOF(bool val) :value(val){}
    };

    std::map<std::string, void*> vars;
    NOOB undefined;

    template<class Iterator>
    struct commentAndBlankSkipper : public qi::grammar<Iterator> {
        qi::rule<Iterator> skip;
        commentAndBlankSkipper() : commentAndBlankSkipper::base_type(skip, "PL/0") {
            skip = ascii::blank
                 | ascii::string("BTW") >> *(ascii::char_ - qi::eol - qi::eoi)
                 ;
        }
    };

    struct new_var{
        void operator()(std::string const& id, qi::unused_type, qi::unused_type) const{
            phx::insert(phx::ref(vars), std::pair<std::string, void*>(id, &undefined));
        }
    };

    struct assign_var{

        void operator()( first, Iterator last) const{
                
            int i = 0;
           /* if (vars.find(varId) != vars.end()){
                phx::insert(phx::ref(vars), std::pair<std::string, void*>(varId, value));
            }
            else{
                throw std::exception("cannot assign to an undeclared variable");
            }  */ 
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
                    | variable >> eol
                    | assign >> eol
                    | eol
                    ;
            
            canHas  = string("CAN") >> string("HAS") >> identifier >> char_('?')
                    ;

            visible = "VISIBLE" >> +quoted_string[phx::ref(std::cout) << qi::_1]
                       >>  (   char_('!')
                           |   qi::eps[phx::ref(std::cout) << std::endl]
                           )
                    ;
            
            variable
                    = string("I") >> string("HAS") >> string("A") 
                        >> identifier[new_var()]
                    ;

            assign = (identifier >> string("R") >> expr)[assign_var<Iterator>()]
                    ;

            expr    = double_[qi::_r0 = phx::new_<NUMBAR>(qi::_1)]
                    | int_[qi::_r0 = phx::new_<NUMBR>(qi::_1)]
                    | quoted_string[qi::_r0 = phx::new_<YARN>(qi::_1)]
                    | bool_[qi::_r0 = phx::new_<TROOF>(qi::_1)]
                    ;


            //addition = "SUM OF" >> mathexpr[qi::_r0 = qi::_1] >> "AN" >> mathexpr[qi::_r0 += qi::_1];
            //mathexpr = addition | double_;
        }

        qi::rule<Iterator, void(), commentAndBlankSkipper<Iterator>> start;
        qi::rule<Iterator, void(), commentAndBlankSkipper<Iterator>> end;
        qi::rule<Iterator, void(), commentAndBlankSkipper<Iterator>> statement;
        qi::rule<Iterator, void(), commentAndBlankSkipper<Iterator>> canHas;
        qi::rule<Iterator, void(), commentAndBlankSkipper<Iterator>> visible;
        qi::rule<Iterator, void(), commentAndBlankSkipper<Iterator>> variable;
        qi::rule<Iterator, void(), commentAndBlankSkipper<Iterator>> assign;
        qi::rule<Iterator, void(), commentAndBlankSkipper<Iterator>> addition;
        qi::rule<Iterator, void*(), commentAndBlankSkipper<Iterator>> expr;
        qi::rule<Iterator, std::string(), commentAndBlankSkipper<Iterator>> quoted_string;
        qi::rule<Iterator, std::string(), commentAndBlankSkipper<Iterator>> identifier;
        qi::rule<Iterator, bool(), commentAndBlankSkipper<Iterator>> bool_;
    };
}

#endif
