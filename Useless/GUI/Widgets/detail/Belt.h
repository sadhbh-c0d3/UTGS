#ifndef __INCLUDED_USELESS_BELT_H__
#define __INCLUDED_USELESS_BELT_H__

namespace Useless
{

struct Belt
{
public:
    Belt( int size=0, bool fixed=false): _size(size), _fixed(fixed) {}

    void SetSize( int size, bool fixed) { _size=size; _fixed=fixed; }
    bool IsFixed() { return _fixed; }

    int     _size;
    bool    _fixed;
    int     _position;
    
};

};//namespace Useless

#endif//__INCLUDED_USELESS_BELT_H__
