#include "oakpch.h"
#include "Node.h"


namespace Oak
{
	Node::Node(bool open)
		: m_open(open), m_id(++idGenerator)
	{

	}
}