#ifndef SetView_H
#define SetView_H

#include "BaseWidget.h"

//���õ������ǰ������
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

//�̳���QWidget���Զ��崰�ڣ��������ڲ��ƶ������϶�����
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
	QRadioButton *Fram;
	QDoubleSpinBox *doubleSpinBox;
	QPushButton *BackButton;//ȡ��
	QSlider *hSlider;


signals:
	void updateOpacity(int);
	void updateVH(bool);
	void updateFramless(bool);

	private slots:
	void setOpacity(int);
	void setVH(bool);
	void setFramless(bool);
	void Back();
};

#endif // SetView_H