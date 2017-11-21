#include "SignalGenerator.h"
#include "TaskRow.h"

TaskRow::TaskRow(QWidget *parent, Task *task) : QWidget(parent)
{
	QHBoxLayout *layout = new QHBoxLayout();
	layout->setSpacing(1);
	setLayout(layout);
	setFixedHeight(50);
	setWindowFlags(Qt::FramelessWindowHint);
	QPalette palette;
	palette.setBrush(QPalette::Background, QBrush(QPixmap("./Resources/bg.png")));
	setPalette(palette);
	QFont font = QFont("Cambria", 12, 28, false);
	font.setBold(true);
	QPalette pe;
	pe.setColor(QPalette::WindowText, Qt::white);
	setAttribute(Qt::WA_QuitOnClose, false);

	taskTypeLabel = new QLabel(this);
	taskTypeLabel->setObjectName("taskType");
	taskTypeLabel->setText(task->taskType);
	taskTypeLabel->setFixedHeight(45);
	taskTypeLabel->setFont(font);
	taskTypeLabel->setPalette(pe);

	taskNameLabel = new QLabel(this);
	taskNameLabel->setObjectName("taskName");
	taskNameLabel->setText(task->taskName);
	taskNameLabel->setFixedHeight(45);
	taskNameLabel->setFont(font);
	taskNameLabel->setPalette(pe);

	taskFileLabel = new QLabel(this);
	taskFileLabel->setObjectName("taskFile");
	taskFileLabel->setText(task->taskInfo.completeBaseName());
	taskFileLabel->setFixedHeight(45);
	taskFileLabel->setMaximumWidth(150);
	taskFileLabel->setFont(font);
	taskFileLabel->setPalette(pe);

	taskProgressBar = new QProgressBar(this);
	taskProgressBar->setObjectName("taskProgress");
	taskProgressBar->setFixedHeight(25);
	taskProgressBar->setFont(font);
	taskProgressBar->setPalette(pe);
	taskProgressBar->setInputMethodHints(Qt::ImhNone);
	taskProgressBar->setValue(0);
	taskProgressBar->setOrientation(Qt::Horizontal);
	taskProgressBar->setTextVisible(true);

	taskStateLabel = new QLabel(this);
	taskStateLabel->setObjectName("taskState");
	taskStateLabel->setText("Playing");
	taskStateLabel->setFixedHeight(45);
	taskStateLabel->setFont(font);
	taskStateLabel->setPalette(pe);

	infoButton = new QPushButton(QIcon("./Resources/info.png"), tr(""), this);
	infoButton->setFixedSize(QSize(45, 45));
	infoButton->setIconSize(QSize(45, 45));

	layout->addWidget(taskTypeLabel);
	layout->addWidget(taskNameLabel);
	layout->addWidget(taskFileLabel);
	layout->addWidget(taskProgressBar);
	layout->addWidget(taskStateLabel);
	layout->addWidget(infoButton);

	layout->setStretchFactor(taskTypeLabel, 1);
	layout->setStretchFactor(taskNameLabel, 1);
	layout->setStretchFactor(taskFileLabel, 4);
	layout->setStretchFactor(taskProgressBar, 5);
	layout->setStretchFactor(taskStateLabel, 1);
	layout->setStretchFactor(infoButton, 1);
}

TaskRow::~TaskRow()
{
	delete taskTypeLabel;
	delete taskNameLabel;
	delete taskFileLabel;
	delete taskProgressBar;
	delete taskStateLabel;
	delete infoButton;
}