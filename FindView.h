//ͷ�ļ���FindView_H
#ifndef FindView_H
#define FindView_H

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
	QPushButton *BackButton;//ȡ��
};

#endif // FindView_H