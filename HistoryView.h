#ifndef HistoryView_H
#define HistoryView_H

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
	QPushButton *BackButton;//ȡ��
};

#endif // AboutView_H