#include "rvpch.h"
#include "UniversallyUniqueIdentifiers.h"

#include <random>



namespace Revoke
{
	static std::random_device s_RD;
	static std::mt19937_64 s_Engine(s_RD());
	static std::uniform_int_distribution<uint64_t> s_Distribution;

	UUID::UUID()
	{
		m_UUID = s_Distribution(s_Engine);
	}
	UUID::UUID(uint64_t uuid)
	{
		m_UUID = uuid;
	}
}