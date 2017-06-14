#include "SignalGenerator.h"
#include <QtWidgets/QApplication>
#include <QDesktopWidget>
#include <cstdio>
#include <QMessageBox>


int main(int argc, char *argv[])
{
	QApplication SignalGenerator(argc, argv);
	SignalGenerator.setQuitOnLastWindowClosed(true);
	MainWindow a;
	a.setAttribute(Qt::WA_QuitOnClose, true);
	a.show();

	return SignalGenerator.exec();
}