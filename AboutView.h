#ifndef AboutView_H
#define AboutView_H

#include "BaseWidget.h"

//���õ������ǰ������
class    QWidget;
class    QFont;
class    QPalette;
class    QRect;
class    QPoint;
class    QHBoxLayout;
class    QLabel;
class    QPushButton;

//�̳���QWidget���Զ��崰�ڣ��������ڲ��ƶ������϶�����
class AboutView : public BaseWidget
{
	Q_OBJECT

public:
	AboutView(QWidget *parent = 0);
	~AboutView();

private:

	QFont font;
	QPalette pe;
	QWidget *BaseView;
	QHBoxLayout * layout;
	QLabel *text;
	QPushButton *BackButton;//ȡ��
};

#endif // AboutView_H