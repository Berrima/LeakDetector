#include "leakdetector.h"


#ifndef REL_MODE

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
        /// So that leaks are not conunted multiple times
        stack.erase(std::remove(stack.begin(), stack.end(), search), stack.end());
    }
    stack.push_back(Pair{ nm, ptr });
}

void Dbg::LeakDbg::remRef(void *ptr)
{
    /// If it's not remove, it means
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
