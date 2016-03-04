#include "AspectInfo.h"

AspectInfo::AspectInfo()
{
	// TODO: Брать из конфига.
	append(1, 1);
	append(4, 3);
	append(5, 4);
	append(16, 9);
	append(16, 10);
}

void AspectInfo::append(int w, int h)
{
	SASPECT aspect = SASPECT(w, h);
	m_aspects.push_back(aspect);
}

int AspectInfo::size()
{
	return m_aspects.size();
}

AspectInfo::SASPECT AspectInfo::getAspect(int index)
{
	return m_aspects[index];
}

AspectInfo::SASPECT AspectInfo::getAspect(int index, int& w, int& h)
{
	if (index >= m_aspects.size())
	{
		w = 1;
		h = 1;
		return SASPECT();
	}
	else
	{
		w = m_aspects[index].w;
		h = m_aspects[index].h;
		return m_aspects[index];
	}
}

qreal AspectInfo::getAspectValue(int index, bool portrait)
{
	if (portrait)
	{
		return ((qreal) m_aspects[index].h) / m_aspects[index].w;
	}
	else
	{
		return ((qreal) m_aspects[index].w) / m_aspects[index].h;
	}
}
