#include "leakdetector.h"
#include <iostream>


class Test{
    TRACK_LEAK(Test)
public:
    Test(){}
    Test( int  ){}
    Test( Test* ){}
};


class Test_Dirv : public Test{
    TRACK_LEAK(Test_Dirv)
};

int main()
{
    /// These wont leak
    ///
    Test_Dirv *test0 = new Test_Dirv();
    Test *test1 = new Test( test0 );
    delete test0;
    delete test1;

    /// These are leaking (10 leaks)
    ///
    Test t0(3); // Parameterized constct
    Test t1{}; // Def constct
    Test t2(Test{0}); // Copy constct, but the compiler may omit the copy
    Test t5(t2); // Copy constct
    Test t3 = t2; // Copy constct
    Test t4;  // Def constct
    t4 = t3; // Assign
    Test t6 = std::move( t1 ); // Moved

    // This will give 3 leaks
    Test *t7 = new Test( new Test_Dirv );
    Test *t8 = new Test( t7 );

    /// Just a ref
    Test &t9 = t1;

    /// Any object that will be destroyed after this
    /// will be considered as a leak
    CHK_LEAK()

    /// Still considered a leak
    delete t8;

    return 0;
}
