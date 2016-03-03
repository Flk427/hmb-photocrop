#include "MainWindow.h"
#include <QApplication>
#include <QTranslator>
#include "ApplicationParams.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	ApplicationParams params;
	params.setParams(argc, argv);

	MainWindow w(params.getParams());
	w.show();

	return a.exec();
}
