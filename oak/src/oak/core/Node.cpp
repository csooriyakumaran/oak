#include "oakpch.h"
#include "oak/core/Node.h"


namespace Oak
{
	Node::Node(bool open)
		: m_open(open), m_id(++idGenerator)
	{

	}
}