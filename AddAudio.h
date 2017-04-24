//头文件：AddAudio.h
#ifndef AddAudio_H
#define AddAudio_H

#include "BaseWidget.h"

//所用到的类的前置声明
class    Action;
class    QToolBar;
class    QWidget;
class    QRect;
class    QPoint;
class    QMouseEvent;
class    QPushButton;
class    QComboBox;


//继承自QWidget的自定义窗口，单击窗口并移动鼠标可拖动窗口
class AddAudio : public BaseWidget
{
	Q_OBJECT

public:
	AddAudio(QWidget *parent = 0);
	~AddAudio();

private:
	void createOption();

	QPushButton *BackButton;
	QPushButton *AddButton;
	QComboBox *comboBox;

	private slots:
	void OK();
	void Back();
	void Add();
};

#endif // AddAudio_H