#ifndef LEAKDETECTOR_H
#define LEAKDETECTOR_H

//#define REL_MODE


#ifdef REL_MODE
/// ## FOR RELEASE
#define CHK_LEAK()
#define TRACK_LEAK(Tp)


#else
/// ## FOR DEBUGING
#include <vector>
#include <string>
#include <assert.h>
#include <cstdlib>
#include <algorithm>
#include <iostream>


#define CHK_LEAK() Dbg::LeakDbg::checking = true;
#define TRACK_LEAK(Tp) Dbg::LeakDetector<Tp> leak_obj = Dbg::LeakDetector<Tp>(this);

namespace Dbg {
void checkLeakStack();

struct Pair
{
    std::string name;
    void* ref;
    bool operator==( const Pair &other ) const { return ref == other.ref; }

};


class LeakDbg
{
private:
    LeakDbg();

public:
    static LeakDbg& instance();
    static bool checking;
    static std::vector<Pair> stack;
    static void addRef(const std::string& nm, void* ptr);
    static void remRef(void* ptr);

};

void checkLeakStack();

template <typename Tp>
class LeakDetector {
private:
    Tp *objRef;
public:
    LeakDetector( Tp *t):objRef(t) {
        LeakDbg::instance(); // singlton
        LeakDbg::addRef( typeid(objRef).name(), objRef);
    }   
    LeakDetector(const LeakDetector<Tp> &)
    {
        LeakDbg::instance(); // singlton
        LeakDbg::addRef(typeid(Tp).name(), this);
    }
    ~LeakDetector() {
        LeakDbg::remRef(objRef);
    }

};
}

#endif /// REL_MODE
#endif // LEAKDETECTOR_H
