//ͷ�ļ���BaseWidget.h
#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include <QWidget>
#include <QToolBar>
#include <QMouseEvent>
#include <QRect>
#include <QPoint>

//�̳���QWidget���Զ��崰�ڣ��������ڲ��ƶ������϶�����
class BaseWidget : public QWidget
{
	Q_OBJECT

public:
	BaseWidget(QWidget *parent = 0);
	void setAreaMovable(const QRect rt);//������갴�¿��ƶ����ڵ��������Ӵ����б������ø�����

public:
	static float v;//ȫ��

protected:
	void mousePressEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);

private:
	QRect m_areaMovable;//���ƶ����ڵ��������ֻ���ڸ������²����ƶ�����
	bool m_bPressed;//��갴�±�־���������Ҽ���
	QPoint m_ptPress;//��갴�µĳ�ʼλ��
};

#endif // BASEWIDGET_H