#include "SignalGenerator.h"
#include <QtWidgets/QApplication>
#include <qtimer.h>
//#include <QtAV>
//#include <QtAVWidgets>

int main(int argc, char *argv[])
{
	//QtAV::Widgets::registerRenderers();
	QApplication SignalGenerator(argc, argv);
	MainWindow a;
	a.show();
	return SignalGenerator.exec();
}
