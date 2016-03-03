#ifndef APPLICATIONPARAMS_H
#define APPLICATIONPARAMS_H

#include "QString"

typedef struct _SApplicationParams
{
	QString sourceImage;
	QString destinationImage;
	QString language;
} SApplicationParams, *PSApplicationParams;

class ApplicationParams
{
public:
	ApplicationParams();

	void setParams(int m_argc, char* m_argv[]);
	SApplicationParams getParams();

private:
	const QString m_sourceParam;
	const QString m_destinationParam;
	const QString m_langParam;

	int m_argc;
	char** m_argv;
	SApplicationParams m_values;
};

#endif // APPLICATIONPARAMS_H
