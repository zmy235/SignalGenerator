#ifndef AboutView_H
#define AboutView_H

#include "BaseWidget.h"

class    QWidget;
class    QFont;
class    QPalette;
class    QRect;
class    QPoint;
class    QHBoxLayout;
class    QLabel;
class    QPushButton;

class AboutView : public BaseWidget
{
	Q_OBJECT

public:
	AboutView(QWidget *parent = 0);
	~AboutView();

private:
	QLabel *text;
	QPushButton *BackButton;//È¡Ïû
};

#endif // AboutView_H