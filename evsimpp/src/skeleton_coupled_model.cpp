#include "skeleton_coupled_model.hpp"

evsim::CSkeletonCoupledModel::CSkeletonCoupledModel(std::string name)
	:CCoupledModel(name)
{
}

evsim::CSkeletonCoupledModel::CSkeletonCoupledModel(const CCoupledModel& rhs)
	:CCoupledModel(rhs.get_name())
{
}
evsim::CSkeletonCoupledModel::~CSkeletonCoupledModel()
{

}
