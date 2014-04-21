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

union intOrDouble{
    int i;
    double d;
    intOrDouble() : i(0){}
};

struct NUMBER : LolcodeType{

    intOrDouble value;
    bool isInt;

    NUMBER() : value(intOrDouble()), isInt(true){}

    NUMBER(bool b){
        value.i = (int)b;
        isInt = true;
    }
    NUMBER(int i){
        value.i = i;
        isInt = true;
    }
    NUMBER(double d){
        value.d = d;
        isInt = false;
    }

    NUMBER& operator+=(NUMBER const& n){
        if (isInt && n.isInt){
            value.i += n.value.i;
        } else if (isInt){
            isInt = false;
            value.d = (double)value.i + n.value.d;
        } else {
            value.d += n.value.d;
        }
        return *this;
    }

    NUMBER& operator-=(NUMBER const& n){
        if (isInt && n.isInt){
            value.i -= n.value.i;
        } else if (isInt){
            isInt = false;
            value.d = (double)value.i - n.value.d;
        } else {
            value.d -= n.value.d;
        }
        return *this;
    }

    NUMBER& operator*=(NUMBER const& n){
        if (isInt && n.isInt){
            value.i *= n.value.i;
        } else if (isInt){
            isInt = false;
            value.d = (double)value.i * n.value.d;
        } else {
            value.d *= n.value.d;
        }
        return *this;
    }

    NUMBER& operator/=(NUMBER& n){
        if (isInt && n.isInt){
            value.i /= n.value.i;
        } else if (isInt){
            isInt = false;
            value.d = (double)value.i / n.value.d;
        } else {
            value.d /= n.value.d;
        }
        return *this;
    }

    NUMBER& operator%=(NUMBER& n){
        if (isInt && n.isInt){
            value.i %= n.value.i;
        }
        else if (isInt){
            isInt = false;
            value.d = std::fmod(value.i, n.value.d);
        }
        else {
            value.d = std::fmod(value.d, n.value.d);
        }
        return *this;
    }

    bool operator>(NUMBER& n){
        if (isInt && n.isInt){
            return value.i > n.value.i;
        } else if (isInt){
            return value.i > n.value.d;
        } else {
            return value.d > n.value.d;
        }
    }

    bool operator<(NUMBER& n){
        if (isInt && n.isInt){
            return value.i < n.value.i;
        } else if (isInt){
            return value.i < n.value.d;
        } else {
            return value.d < n.value.d;
        }
    }
};

#endif