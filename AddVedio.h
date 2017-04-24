//头文件：AddVedio.h
#ifndef AddVedio_H
#define AddVedio_H

#include "BaseWidget.h"


//所用到的类的前置声明
class    Action;
class    QToolBar;
class    QWidget;
class    QRect;
class    QPoint;
class    QMouseEvent;


//继承自QWidget的自定义窗口，单击窗口并移动鼠标可拖动窗口
class AddVedio : public BaseWidget
{
	Q_OBJECT

public:
	AddVedio(QWidget *parent = 0);
	~AddVedio();

private:
	void createActions();
	void createToolBars();

	QToolBar *VedioToolBar;

	QAction *back;
	QAction *AV;
	QAction *VGA;
	QAction *RGB;
	QAction *YPbPr;
	QAction *LVDS;
	QAction *SDI;
	QAction *DVI;
	QAction *FPDLink;
	QAction *CameraLink;

	private slots:
	void Back();
	void Add();
};

#endif // AddVedio_H