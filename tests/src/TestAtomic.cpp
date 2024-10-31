#include "TestAtomic.hpp"

CTestAtomic::CTestAtomic(std::string name)
	:evsim::CAtomic(name)
{
	REGISTER_INPUT_PORT(one);
	REGISTER_INPUT_PORT(two);
	REGISTER_INPUT_PORT(three);

	REGISTER_STATE(IDLE);
}
