#include "SignalGenerator.h"
#include "TaskRow.h"
#include "Task.h"

#include<QWidget>
#include<QPalette>
#include<QFont>
#include<QHBoxLayout>
#include<QProgressBar>
#include<QPushButton>
#include<QLabel>

TaskRow::TaskRow(QWidget *parent, Task *task) : QWidget(parent)
{
	QHBoxLayout *layout = new QHBoxLayout();
	layout->setSpacing(5);
	setLayout(layout);
	setFixedHeight(50);
	setWindowFlags(Qt::FramelessWindowHint);
	QPalette palette;
	palette.setBrush(QPalette::Background, QBrush(QPixmap("./Resources/bg0.png")));
	setPalette(palette);
	QFont font = QFont("Cambria", 12, 28, false);
	font.setBold(true);
	QPalette pe;
	pe.setColor(QPalette::WindowText, Qt::white);
	setAttribute(Qt::WA_QuitOnClose, false);

	taskTypeLabel = new QLabel();
	taskTypeLabel->setText(task->taskType);
	taskTypeLabel->setFixedHeight(45);
	taskTypeLabel->setFont(font);
	taskTypeLabel->setPalette(pe);

	taskNameLabel = new QLabel();
	taskNameLabel->setText(task->taskName);
	taskNameLabel->setFixedHeight(45);
	taskNameLabel->setFont(font);
	taskNameLabel->setPalette(pe);

	taskFileLabel = new QLabel();
	taskFileLabel->setText(task->taskInfo.absolutePath());
	taskFileLabel->setFixedHeight(45);
	taskFileLabel->setFont(font);
	taskFileLabel->setPalette(pe);

	taskProgressBar = new QProgressBar();
	taskProgressBar->setFixedHeight(25);
	taskProgressBar->setFont(font);
	taskProgressBar->setPalette(pe);
	taskProgressBar->setInputMethodHints(Qt::ImhNone);
	taskProgressBar->setValue(0);
	taskProgressBar->setOrientation(Qt::Horizontal);
	taskProgressBar->setTextVisible(true);

	taskStateLabel = new QLabel();
	taskStateLabel->setObjectName("taskState");
	taskStateLabel->setText(task->isPlaying ? "Playing" : "Paused");
	taskStateLabel->setFixedHeight(45);
	taskStateLabel->setFont(font);
	taskStateLabel->setPalette(pe);

	infoButton = new QPushButton(QIcon("./Resources/info.png"), tr(""));
	infoButton->setFixedSize(QSize(45, 45));
	infoButton->setIconSize(QSize(45, 45));

	layout->addWidget(taskTypeLabel);
	layout->addWidget(taskNameLabel);
	layout->addWidget(taskProgressBar);
	layout->addWidget(taskStateLabel);
	layout->addWidget(infoButton);

	layout->setStretchFactor(taskTypeLabel, 3);
	layout->setStretchFactor(taskNameLabel, 4);
	layout->setStretchFactor(taskProgressBar, 6);
	layout->setStretchFactor(taskStateLabel, 3);
	layout->setStretchFactor(infoButton, 1);
}

TaskRow::~TaskRow()
{
}
