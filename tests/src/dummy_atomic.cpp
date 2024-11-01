#include "dummy_atomic.hpp"

CDummyAtomic::CDummyAtomic(std::string name)
	:evsim::CAtomic(name)
{
	REGISTER_INPUT_PORT(one);
	REGISTER_INPUT_PORT(two);
	REGISTER_INPUT_PORT(three);

	REGISTER_STATE(IDLE1);
	REGISTER_STATE(IDLE2);

	cur_state = IDLE1;
}
