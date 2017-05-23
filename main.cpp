#include "SignalGenerator.h"
#include <QtWidgets/QApplication>
#include <QDesktopWidget>
#include <cstdio>
#include <QMessageBox>

typedef struct
{
	int screen_no;
	QRect rect;
}SCREEN;
SCREEN g_screens[10];

int main(int argc, char *argv[])
{
	QApplication SignalGenerator(argc, argv);
	SignalGenerator.setQuitOnLastWindowClosed(true);
	MainWindow a;
	a.setAttribute(Qt::WA_QuitOnClose, true);
	a.show();

	//QDesktopWidget *desktop = QApplication::desktop();
	//int screen_count = desktop->screenCount();
	//int prim_screen = desktop->primaryScreen();
	//char warning[100], *idx = warning;
	//for (int i = 0; i<screen_count; i++)
	//{
	//	g_screens[i].screen_no = prim_screen + i;
	//	g_screens[i].rect = desktop->screenGeometry(prim_screen + i);
	//	sprintf(idx, "screen%d w[%d], h[%d] ", i, g_screens[i].rect.width(), g_screens[i].rect.height());
	//	idx += strlen(idx);
	//}
	//sprintf(idx, "total width[%d] , total height[%d] \n", desktop->width(), desktop->height());
	//QMessageBox::warning(NULL, "screen", warning, QMessageBox::Ok);
	//MainWindow wnd[5];
	//for (int i = 0; i < screen_count; i++)
	//{
	//	wnd[i].resize(g_screens[i].rect.width(), g_screens[i].rect.height());
	//	if (i == 0)
	//		wnd[i].move(0, 0);
	//	else
	//		wnd[i].move(i* g_screens[i - 1].rect.width(), 0);
	//	char str[50];
	//	sprintf(str, "this is screen %d", i);
	//	wnd[i].show();
	//}

	return SignalGenerator.exec();
}