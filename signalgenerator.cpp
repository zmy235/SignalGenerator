#include "SignalGenerator.h"
#include "BaseWidget.h"
#include "Task.h"
#include "TaskRow.h"
#include "HistoryView.h"
#include "SetView.h"
#include "AboutView.h"
#include "AudioWall.h"
#include "VideoWall.h"

#include<QDebug>
#include<QMessageBox>
#include<QList>
#include<QMouseEvent>
#include<QRect>
#include<QPoint>
#include<QLayout>
#include<QScrollBar>
#include<QScrollArea>
#include<QDockWidget>
#include<QVariant> 
#include<QProgressBar>
#include<QListView>
#include<QPushButton>
#include<QLabel>
#include<QTimer>

const int kVolumeSliderMax = 100;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	setWindowTitle(tr("信号发生器"));
	ui.setupUi(this);

	//变量初始化
	ListNum = 0;
	Base = ui.widget;
	vLayout = ui.verticalLayout;
	vLayout->setAlignment(Qt::AlignTop);
	scrollArea = ui.scrollArea;
	scrollArea->setWidgetResizable(true);
	scrollArea->setWidget(Base);
	scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_areaMovable = geometry();
	m_bPressed = false;
	setMinimumSize(640, 480);
	createActions();
	createToolBars();
	addToolBar(Qt::LeftToolBarArea, MainToolBar); //TopToolBarArea
	font = QFont("Cambria", 12, 28, false);
	font.setBold(true);
	font_pe.setColor(QPalette::WindowText, Qt::white);
	palette.setBrush(QPalette::Background, QBrush(QPixmap("./Resources/background.png")));
	setPalette(palette);
	//setStyleSheet("QMainWindow { background-color: #AACBEB; }");

	//窗口初始化
	historyView = new HistoryView();
	setView = new SetView();
	aboutView = new AboutView();
	audioView = new AudioWall();
	videoView = new BaseWidget();
	wall = new VideoWall();
	wall->setView(videoView);

	connect(setView, &SetView::updateVH, this, &MainWindow::updateVH);
	connect(setView, &SetView::updateOpacity, this, &MainWindow::updateOpacity);
	connect(audioView, &AudioWall::updateAudioList, this, &MainWindow::updateAudioList);
	connect(wall, &VideoWall::updateVideoList, this, &MainWindow::updateVideoList);
}

MainWindow::~MainWindow()
{
	historyView->close();
	setView->close();
	aboutView->close();
	wall->stop();
	delete wall;
}

void MainWindow::createActions()
{
	audio = new QAction(QIcon("./Resources/audio.png"), tr("&audio"), this);
	audio->setStatusTip(tr("audio"));
	connect(audio, SIGNAL(triggered()), this, SLOT(AudioView()));

	video = new QAction(QIcon("./Resources/video.png"), tr("&video"), this);
	video->setStatusTip(tr("video"));
	connect(video, SIGNAL(triggered()), this, SLOT(VideoView()));

	history = new QAction(QIcon("./Resources/history.png"), tr("&history"), this);
	history->setStatusTip(tr("history"));
	connect(history, SIGNAL(triggered()), this, SLOT(History()));

	setting = new QAction(QIcon("./Resources/setting.png"), tr("&setting"), this);
	setting->setStatusTip(tr("setting"));
	connect(setting, SIGNAL(triggered()), this, SLOT(Setting()));

	about = new QAction(QIcon("./Resources/about.png"), tr("&about"), this);
	about->setStatusTip(tr("about"));
	connect(about, SIGNAL(triggered()), this, SLOT(About()));

	shutdown = new QAction(QIcon("./Resources/shutdown.png"), tr("&shutdown"), this);
	shutdown->setStatusTip(tr("exit"));
	connect(shutdown, SIGNAL(triggered()), this, SLOT(close()));

}

void MainWindow::createToolBars()
{
	MainToolBar = ui.toolBar;
	MainToolBar->setMovable(false);
	MainToolBar->setFloatable(false);
	MainToolBar->setIconSize(QSize(45, 45));
	MainToolBar->addAction(audio);
	MainToolBar->addAction(video);
	MainToolBar->addAction(history);
	MainToolBar->addAction(setting);
	MainToolBar->addAction(about);
	MainToolBar->addAction(shutdown);
	//insertToolBarBreak(AddToolBar);
}

void MainWindow::mousePressEvent(QMouseEvent *e)//鼠标左键
{
	if (e->button() == Qt::LeftButton)
	{
		qDebug() << e->pos();
		m_ptPress = e->pos();
		m_bPressed = m_areaMovable.contains(m_ptPress);
	}
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)//鼠标
{
	if (m_bPressed)
	{
		move(pos() + e->pos() - m_ptPress);
	}
}

void MainWindow::mouseReleaseEvent(QMouseEvent *)//鼠标
{
	m_bPressed = false;
}

void MainWindow::setAreaMovable(const QRect rt)//设置鼠标按下的区域
{
	if (m_areaMovable != rt)
	{
		m_areaMovable = rt;
	}
}

void MainWindow::deleteRow(Task *task)
{
	int k = taskList.indexOf(task);
	rows[k]->close();
	rows.removeAt(k);
}

/*槽函数*/

void MainWindow::updateAudioList(AudioTask* task)
{
	if (task->isFinished)
	{
		deleteRow(task);
		ListNum--;
	}
	else
	{
		taskList.append(task);
		AudioTaskList.append(task);
		audioNth[AudioTaskList.size()] = taskList.size();
		qDebug()
			<< "============================================================="
			<< task->taskType << endl
			<< task->taskInfo.absolutePath() << endl
			<< task->taskName << endl
			<< task->taskInfo.size() << endl
			<< task->taskProgress << endl
			<< task->isPlaying << endl
			<< task->isFinished << endl
			<< task->taskTime << endl
			<< taskList.size() << endl
			<< "  ListNum：" << ListNum << endl
			<< "=============================================================";

		TaskRow *row = new TaskRow(Base, task);
		row->setFixedHeight(50);
		row->setWindowFlags(Qt::FramelessWindowHint);
		row->setPalette(palette);
		vLayout->addWidget(row);
		rows.append(row);
		row->show();
		connect(row->infoButton, SIGNAL(clicked()), this, SLOT(Info()));

		ListNum++;
	}
}

void MainWindow::updateVideoList(VideoTask* task)
{
	taskList.append(task);
	VideoTaskList.append(task);
	videoNth[VideoTaskList.size()] = taskList.size();

	qDebug()
		<< "============================================================="
		<< task->taskType << endl
		<< task->taskInfo.absoluteFilePath() << endl
		<< task->taskName << endl
		<< task->taskInfo.size() << endl
		<< task->taskProgress << endl
		<< task->isPlaying << endl
		<< task->isFinished << endl
		<< task->taskTime << endl
		<< taskList.size() << endl
		<< "  ListNum：" << ListNum << endl
		<< "=============================================================";

	TaskRow *row = new TaskRow(Base, task);
	row->setFixedHeight(50);
	row->setWindowFlags(Qt::FramelessWindowHint);
	row->setPalette(palette);
	vLayout->addWidget(row);
	rows.append(row);
	row->show();
	connect(row->infoButton, SIGNAL(clicked()), this, SLOT(Info()));

	ListNum++;
}

void MainWindow::updateAudioState(int n)
{
	nth = audioNth[n];
	QLabel *test = rows.at(nth)->findChild<QLabel *>("state");

	if (AudioTaskList.at(n)->isPlaying)
	{
		test->setText("Paused");
	}
	else
	{
		test->setText("Playing");
	}
}

void MainWindow::updateVideoState(int n)
{
	nth = videoNth[n];
	QLabel *test = rows.at(nth)->findChild<QLabel *>("state");
	if (VideoTaskList.at(n)->isPlaying)
	{
		test->setText("Paused");
	}
	else
	{
		test->setText("Playing");
	}
}

void MainWindow::updateVH(bool value)
{
	addToolBar(value ? Qt::TopToolBarArea : Qt::LeftToolBarArea, MainToolBar);
	Base->setGeometry(QRect(value ? 0 : 60, value ? 60 : 0, 670, 500));
}

void MainWindow::updateOpacity(int value)
{
	this->setWindowOpacity((100.0 - value) / 100.0);
	this->historyView->setWindowOpacity((100.0 - value) / 100.0);
	this->aboutView->setWindowOpacity((100.0 - value) / 100.0);
	this->audioView->setWindowOpacity((100.0 - value) / 100.0);
}

void MainWindow::updateFramless(bool value)
{
	setWindowFlags(Qt::FramelessWindowHint);
	setWindowFlags(Qt::CustomizeWindowHint);
}

//菜单栏点击操作

void MainWindow::AudioView()
{
	audioView->show();
}

void MainWindow::VideoView()
{
	wall->show();
}

void MainWindow::History()
{
	historyView->show();
}

void MainWindow::Setting()
{
	setView->show();
}

void MainWindow::About()
{
	aboutView->show();
}

//列表操作

void MainWindow::Info()
{
	QPushButton *test = qobject_cast<QPushButton *>(sender());
	QWidget *w = test->parentWidget();
	nth = w->y() / 50;
	QMessageBox::about(0, tr("Help"),
		QString::fromLocal8Bit("文件绝对路径:")
		+ taskList.at(nth)->taskInfo.absoluteFilePath()
		+ QString::fromLocal8Bit("\n文件大小：")
		+ QString::number(taskList.at(nth)->taskInfo.size())
		+ QString::fromLocal8Bit("\n信号：")
		+ taskList.at(nth)->taskName
		+ QString::fromLocal8Bit("\n"));
}