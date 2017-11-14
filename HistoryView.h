#ifndef HistoryView_H
#define HistoryView_H

#include "BaseWidget.h"

class    QWidget;
class    QFont;
class    QPalette;
class    QRect;
class    QPoint;
class    QHBoxLayout;
class    QLabel;
class    QTextEdit;
class    QPushButton;

class HistoryView : public BaseWidget
{
	Q_OBJECT

public:
	HistoryView(QWidget *parent = 0);
	~HistoryView();

private:
	QTextEdit *content;
	QPushButton *BackButton;
	QPushButton *SaveButton;

	private slots:
	void loadData();
	void saveData();
};

#endif