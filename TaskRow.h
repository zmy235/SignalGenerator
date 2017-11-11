#ifndef TaskRow_H
#define TaskRow_H

#include <QWidget>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include "Task.h"

//�̳���QWidget���Զ���ؼ�
class TaskRow : public QWidget
{
	Q_OBJECT

public:
	TaskRow(QWidget *parent = 0, Task *task = 0);
	~TaskRow();

	QLabel *taskTypeLabel;
	QLabel *taskNameLabel;
	QLabel *taskFileLabel;
	QProgressBar *taskProgressBar;
	QLabel *taskStateLabel;
	QPushButton *infoButton;
};

#endif