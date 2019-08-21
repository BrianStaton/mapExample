#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <map>
#include <unordered_map>
#include <string.h>

#define USE_CLASS_BAR   (1)

class CFoo
{
    public:
        CFoo(int _i);

        // friend bool operator< (const CFoo l, const CFoo r);
        bool operator< (const CFoo x) const;
        int getI(void) const {return i;}

    private:
        int i;
};

CFoo::CFoo(int _i)
    : i(_i)
{
}

#if 0
// This is the friend way
bool operator< (const CFoo l, const CFoo r)
{
    return l.i < r.i;
}
#else
bool CFoo::operator< (const CFoo x) const
{
    return this->i < x.getI();
}
#endif

#if USE_CLASS_BAR

// If CBar is to be used as a Key for an unordered map,
// you must overload the test for equality by using the
// friend function or another overload for ==
// and also provide a hash function.
class CBar
{
    public:
        CBar(int _i);

        // friend bool operator==(const CBar l, const CBar r);
        bool operator==(const CBar x) const;

        int getI(void) const {return i;}

    private:
        int i;
};

CBar::CBar(int _i)
    : i(_i)
{
}

#if 0
// This is using the friend method
bool operator==(CBar l, CBar r)
{
   return (l.i==r.i);
}
#else
bool CBar::operator==(const CBar x) const
{
    return (x.getI() == this->getI());
}
#endif

// This is how to add a hash function for user-defined types
// Overload the () operator and make it return a hash.
// The example multiplies two primes with the member.
// A good hash will compute a hash on each member and combine
// the hash values some way.  Shifting and XORing to do
// the combining may help too.
namespace std
{
    template <>
    struct hash<CBar>
    {
        size_t operator()( const CBar& k ) const
        {
            size_t res = 17;
            // res = res * 31 + hash<string>()( k.first );
            // res = res * 31 + hash<string>()( k.second );
            res = res * 31 + hash<int>()( k.getI() );
            return res;
        }
    };
}
#endif // USE_CLASS_BAR

int main(int argc, char *argv[])
{
    CFoo f1(1);
    CFoo f2(2);
    CFoo f3(3);
    std::map<CFoo, char> theFooMap;
    std::map<CFoo, char>::const_iterator itFoo;
    #if USE_CLASS_BAR
    CBar b1(10);
    CBar b2(20);
    CBar b3(30);
    std::unordered_map<CBar, char> theBarMap;
    std::unordered_map<CBar, char>::const_iterator itBar;
    #else
    std::unordered_map<std::string, char> theBarMap;
    std::unordered_map<std::string, char>::const_iterator itBar;
    #endif

    theFooMap[f1] = 'C';
    theFooMap[f2] = 'B';
    theFooMap[f3] = 'A';

    #if USE_CLASS_BAR
    theBarMap[b1] = 'C';
    theBarMap[b2] = 'B';
    theBarMap[b3] = 'A';
    #else
    theBarMap["AAA"] = 'C';
    theBarMap["BBB"] = 'B';
    theBarMap["CCC"] = 'A';
    #endif

    for(itFoo = theFooMap.begin(); itFoo != theFooMap.end(); ++itFoo)
    {
        printf("%c\n", itFoo->second);
    }
    itFoo = theFooMap.find(f2);
    printf("f2 = %c\n", itFoo->second);

    for(itBar = theBarMap.begin(); itBar != theBarMap.end(); ++itBar)
    {
        printf("%c\n", itBar->second);
    }
    #if USE_CLASS_BAR
    itBar = theBarMap.find(b2);
    printf("b2 = %c\n", itBar->second);
    #else
    itBar = theBarMap.find("BBB");
    printf("%s = %c\n", itBar->first.c_str(), itBar->second);
    #endif

}
