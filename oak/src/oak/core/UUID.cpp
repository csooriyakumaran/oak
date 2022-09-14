#include "oakpch.h"
#include "UUID.h"

#include <random>

namespace Oak
{
	//static std::random_device s_RandomDevice;
	//static std::mt19937_64 eng(s_RandomDevice());
	//static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

	//UUID::UUID()
	//	: m_UUID(s_UniformDistribution(eng))
	//{
	//}

	//UUID::UUID(uint64_t uuid)
	//	: m_UUID(uuid)
	//{
	//}


	//UUID& UUID::operator=(UUID& other)
	//{
	//	m_UUID = other.m_UUID;
	//	return *this;
	//}

	//const UUID& UUID::operator=(const UUID& other)
	//{
	//	m_UUID = other.m_UUID;
	//	return *this;
	//}

	//UUID& UUID::operator=(UUID&& other)
	//{
	//	if (this != &other)
	//	{
	//		m_UUID = std::move(other.m_UUID);
	//		other.m_UUID = 0;
	//	}
	//	return *this;
	//}
}