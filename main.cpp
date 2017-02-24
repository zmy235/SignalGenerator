#include "SignalGenerator.h"
#include <QtWidgets/QApplication>
#include <qtimer.h>

int main(int argc, char *argv[])
{
	QApplication SignalGenerator(argc, argv);
	MainWindow a;
	a.show();
	return SignalGenerator.exec();
}
