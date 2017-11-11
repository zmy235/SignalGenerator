#ifndef HistoryView_H
#define HistoryView_H

#include "BaseWidget.h"

//所用到的类的前置声明
class    QWidget;
class    QFont;
class    QPalette;
class    QRect;
class    QPoint;
class    QHBoxLayout;
class    QLabel;
class    QPushButton;

//继承自QWidget的自定义窗口，单击窗口并移动鼠标可拖动窗口
class HistoryView : public BaseWidget
{
	Q_OBJECT

public:
	HistoryView(QWidget *parent = 0);
	~HistoryView();

private:

	QFont font;
	QPalette pe;
	QWidget *BaseView;
	QHBoxLayout * layout;
	QLabel *text;
	QPushButton *BackButton;//取消
};

#endif // AboutView_H