#include "oakpch.h"
#include "oak/core/Panel.h"


namespace Oak
{
	Panel::Panel(const std::string& name, bool open)
		: m_Name(name), m_open(open), m_id(++idGenerator)
	{

	}
}