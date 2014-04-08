#ifndef lolcodetypes_h
#define lolcodetypes_h

struct LolcodeType{
    virtual ~LolcodeType(){}
};

struct NOOB : LolcodeType{
    void* value = nullptr;
};
struct YARN : LolcodeType{
    std::string value;
    YARN(std::string val) : value(val){}
};
struct NUMBR : LolcodeType{
    int value;
    NUMBR(int val) : value(val){}
};
struct NUMBAR : LolcodeType{
    double value;
    NUMBAR(double val) : value(val){}
};
struct TROOF : LolcodeType{
    bool value;
    TROOF(bool val) : value(val){}
};

#endif