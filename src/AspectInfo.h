#ifndef ASPECTINFO_H
#define ASPECTINFO_H

#include <QVector>

class AspectInfo
{
public:
	struct SASPECT
	{
		int w;
		int h;

		SASPECT() {w = 1; h = 1;}
		SASPECT(int _w, int _h) {w = _w; h = _h;}
	};

	AspectInfo();

	void append(int w, int h);
	int size();
	SASPECT getAspect(int index);
	SASPECT getAspect(int index, int& w, int& h);
	qreal getAspectValue(int index, bool portrait);

private:
	QVector<SASPECT> m_aspects;
};

#endif // ASPECTINFO_H
