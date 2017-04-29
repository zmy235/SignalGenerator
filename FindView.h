//头文件：FindView_H
#ifndef FindView_H
#define FindView_H

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
class FindView : public BaseWidget
{
	Q_OBJECT

public:
	FindView(QWidget *parent = 0);
	~FindView();

private:

	QFont font;
	QPalette pe;
	QWidget *BaseView;
	QHBoxLayout * layout;
	QLabel *text;
	QPushButton *BackButton;//取消
};

#endif // FindView_H