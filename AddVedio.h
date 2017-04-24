//ͷ�ļ���AddVedio.h
#ifndef AddVedio_H
#define AddVedio_H

#include "BaseWidget.h"


//���õ������ǰ������
class    Action;
class    QToolBar;
class    QWidget;
class    QRect;
class    QPoint;
class    QMouseEvent;


//�̳���QWidget���Զ��崰�ڣ��������ڲ��ƶ������϶�����
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