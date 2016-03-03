#include "ApplicationParams.h"

ApplicationParams::ApplicationParams() :
	m_langParam("-lang="),
	m_sourceParam("-src="),
	m_destinationParam("-dst="),
	m_argc(0),
	m_argv(nullptr)
{

}

void ApplicationParams::setParams(int argc, char* argv[])
{
	m_argc = argc;
	m_argv = argv;
}

SApplicationParams ApplicationParams::getParams()
{
	for (int i = 1; i < m_argc; ++i)
	{
		QString param = m_argv[i];

		if (param.left(m_langParam.length()) == m_langParam)
		{
			m_values.language = param.right(param.length() - m_langParam.length());
		}

		if (param.left(m_sourceParam.length()) == m_sourceParam)
		{
			m_values.sourceImage = param.right(param.length() - m_sourceParam.length());
		}

		if (param.left(m_destinationParam.length()) == m_destinationParam)
		{
			m_values.destinationImage = param.right(param.length() - m_destinationParam.length());
		}
	}

	return m_values;
}

