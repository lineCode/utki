//make sure all ting debugging facilities are turned on
#ifndef DEBUG
#	define DEBUG
#endif

#include "../../src/utki/debug.hpp"

#include "tests.hpp"

#include <memory>


namespace TestBasicDebugStuff{
	


bool DoSomethingAndReturnTrueOnSuccess(){
	return true;
}



class TestClass{
public:
	int a;
};



void Run(){
//	TRACE(<< "debug test" << std::endl)

	{
		int a = 13;
		ASSERT(a == 13)
		ASSERT(sizeof(int) == 4)
		ASSERT(sizeof(a) == 4 && sizeof(a) == sizeof(int))
	}

	{
		int a = 13;
		ASSERT_INFO(a == 13, "a is not 13, a is " << a)
	}

	{
		TestClass *c = new TestClass();
		
		//make sure, "c" is not 0 before accessing member a.
		c->a = 13;
		int b = c->a;
		ASSERT_ALWAYS(b == 13)
		delete c;
	}

	{
		std::unique_ptr<TestClass> pc(new TestClass());

		//make sure, "pc" is valid before accessing member a.
		pc->a = 13;
		int b = pc->a;
		ASSERT_ALWAYS(b == 13)
	}
}

}//~namespace
