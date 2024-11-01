#include "dummy_atomic.hpp"

CDummyAtomic::CDummyAtomic(std::string name)
	:evsim::CAtomicModel(name)
{
	REGISTER_INPUT_PORT(one);
	REGISTER_INPUT_PORT(two);
	REGISTER_INPUT_PORT(three);

	REGISTER_STATE(IDLE1);
	REGISTER_STATE(IDLE2);

	SET_INIT_STATE(IDLE1);
}
