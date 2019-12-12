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

bool Dbg::LeakDbg::checking = false;
std::vector<Dbg::Pair> Dbg::LeakDbg::stack = std::vector<Dbg::Pair>();

Dbg::LeakDbg::LeakDbg()
{
    /// Constructor will only be called once,
    /// since it's a singlton class
    std::atexit( checkLeakStack );
}

Dbg::LeakDbg &Dbg::LeakDbg::instance()
{
    static LeakDbg INSTANCE;
    return INSTANCE;
}

void Dbg::LeakDbg::addRef(const std::string &nm, void *ptr)
{
    const Pair search = Pair{"",ptr};
    auto res = std::find(stack.begin(), stack.end(), Pair{"",ptr}) != stack.end();
    if( res ) {
        /// This will remove the base class ref, if it's a derived class
        stack.erase(std::remove(stack.begin(), stack.end(), search), stack.end());
    }
    stack.push_back(Pair{ nm, ptr });
}

void Dbg::LeakDbg::remRef(void *ptr)
{
    /// If it's not enabled, it means
    /// it's OK to remove a ref
    if( !LeakDbg::checking ){
        const Pair search = Pair{"",ptr};
        std::vector<Pair> vect;
        stack.erase(std::remove(stack.begin(), stack.end(), search), stack.end());
    }
}

void Dbg::checkLeakStack()
{
    /// Here the stack should be emoty
    /// you can print or assert if the stack is not empty
    for ( Pair pair : LeakDbg::stack) {
        const std::string msg = pair.name + " is leaked";
        std::cout << msg << std::endl;
    }
    std::cout  << "There are " << LeakDbg::stack.size() << " leaks ..." << std::endl;
    assert(LeakDbg::stack.empty());

}

#endif /// REL_MODE
#endif // LEAKDETECTOR_H
