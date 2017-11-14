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
class    QSpinBox;
class    QCheckBox;
class    QSlider;

class SetView : public BaseWidget
{
	Q_OBJECT

public:
	SetView(QWidget *parent = 0);
	~SetView();

private:
	QCheckBox *VH;
	QCheckBox *Fram;
	QSpinBox *spinBox;
	QSlider *hSlider;
	QPushButton *BackButton;
	QPushButton *OKButton;

signals:
	void updateOpacity(int);
	void updateVH(bool);
	void updateFramless(bool);

	private slots:
	void setOpacity(int);
	void setVH(int);
	void setFramless(int);
};

#endif // SetView_H