#include "SignalGenerator.h"
#include "BaseWidget.h"
#include "HistoryView.h"
#include "Task.h"

#include <QWidget>
#include <QPalette>
#include <QFont>
#include <QHBoxLayout>
#include <QPushButton>
#include <QUrl>
#include <QTextEdit>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QDataStream>
#include <QDebug>

double XX_Angle;
HistoryView::HistoryView(QWidget *parent) : BaseWidget(parent)
{
	setWindowTitle(QString::fromLocal8Bit("任务历史记录"));
	setMinimumSize(480, 480);

	QFont font;
	font = QFont("Cambria", 12, 26, false);
	font.setBold(true);
	QPalette pe;
	pe.setColor(QPalette::WindowText, Qt::white);

	QLabel *title = new QLabel(this);
	title->setText(tr("TASK HISTORY"));
	title->setFont(font);
	title->setPalette(pe);

	content = new QTextEdit(this);
	content->setContentsMargins(20, 5, 20, 50);
	content->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	content->setFont(font);

	BackButton = new QPushButton(QIcon("./Resources/cancel.png"), tr(""), this);
	BackButton->setStatusTip(tr("back"));
	BackButton->setFixedSize(QSize(45, 45));
	BackButton->setIconSize(QSize(45, 45));
	SaveButton = new QPushButton(QIcon("./Resources/ok.png"), tr(""), this);
	SaveButton->setStatusTip(tr("save"));
	SaveButton->setFixedSize(QSize(45, 45));
	SaveButton->setIconSize(QSize(45, 45));

	QHBoxLayout * hlayout = new QHBoxLayout();
	hlayout->setAlignment(Qt::AlignCenter);
	hlayout->setSpacing(50);
	hlayout->addWidget(BackButton);
	hlayout->addWidget(SaveButton);

	QVBoxLayout * vlayout = new QVBoxLayout(this);
	vlayout->setAlignment(Qt::AlignCenter);
	vlayout->setSpacing(10);
	vlayout->addWidget(title);
	vlayout->addWidget(content);
	vlayout->addLayout(hlayout);

	loadData();
	connect(BackButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(SaveButton, SIGNAL(clicked()), this, SLOT(saveData()));
}

HistoryView::~HistoryView()
{
	delete content;
	delete BackButton;
	delete SaveButton;
}

void HistoryView::loadData()
{
	QFile file(tr("task.log"));
	QDir tempDir;
	QString currentDir = tempDir.currentPath();
	if (!file.exists())//如果filePath路径不存在，创建它
	{
		qDebug() << QString::fromLocal8Bit("记录文件已被删除或损坏，已重新创建.");
		tempDir.mkpath(currentDir + tr("task.log"));
	}
	tempDir.setCurrent(currentDir);
	qDebug() << tempDir.currentPath();

	if (!file.open(QIODevice::ReadWrite)) return;
	QTextStream out(&file);
	while (!file.atEnd())
	{
		content->setText(out.readAll());
	}
	file.close();
}

void HistoryView::saveData()
{
	QWidget *tmep = new BaseWidget();
	tmep->setMinimumSize(400, 300);
	QMessageBox::about(tmep, tr("FILE SAVED"), QString::fromLocal8Bit("文件保存成功！"));
	
}