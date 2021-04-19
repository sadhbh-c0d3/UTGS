#include "UselessPch.h"

#include "Useless/Util/LockObj.h"

#ifndef NDEBUG

namespace {

class Lockable
{
public:
    bool Lock() { return 0; }
    bool Lock(int) { return 0; }
    bool Lock(int*) { return 0; }
    bool Lock(int**,int*,int*) { return 0; }
    void Unlock() {}
};

void Test()
{
    int i,*pi;
    Lockable lockable;
    Useless::LockObj<Lockable> guard1(lockable);
    Useless::LockObj<Lockable> guard2(lockable,i);
    Useless::LockObj<Lockable> guard3(lockable,pi);
    Useless::LockObj<Lockable> guard4(lockable,&pi,pi,pi);
}

};

#endif
