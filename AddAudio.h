//ͷ�ļ���AddAudio.h
#ifndef AddAudio_H
#define AddAudio_H

#include "BaseWidget.h"

//���õ������ǰ������
class    Action;
class    QToolBar;
class    QWidget;
class    QRect;
class    QPoint;
class    QMouseEvent;
class    QPushButton;
class    QComboBox;


//�̳���QWidget���Զ��崰�ڣ��������ڲ��ƶ������϶�����
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