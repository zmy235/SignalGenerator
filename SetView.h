//头文件：SetView.h
#ifndef SetView_H
#define SetView_H

#include "BaseWidget.h"

//所用到的类的前置声明
class    QWidget;
class    QFont;
class    QPalette;
class    QRect;
class    QPoint;
class    QHBoxLayout;
class    QPushButton;
class    QComboBox;
class    QDoubleSpinBox;
class    QRadioButton;
class    QSlider;


//继承自QWidget的自定义窗口，单击窗口并移动鼠标可拖动窗口
class SetView : public BaseWidget
{
	Q_OBJECT

public:
	SetView(QWidget *parent = 0);
	~SetView();

private:

	QFont font;
	QPalette pe;
	QWidget *BaseView;
	QRadioButton *VH;
	QDoubleSpinBox *doubleSpinBox;
	QPushButton *BackButton;//取消
	QSlider *hSlider;


	signals:
	void updateOpacity(int);
	void updateVH(int);

	private slots:
	void Back();
	void setOpacity(int);
	void setVH(bool);
};

#endif // SetView_H