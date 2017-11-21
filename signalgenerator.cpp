#include "SignalGenerator.h"

#include<QDebug>
#include<QMessageBox>
#include<QList>
#include<QMouseEvent>
#include<QRect>
#include<QPoint>
#include<QLayout>
#include<QScrollBar>
#include<QScrollArea>
#include<QVariant> 
#include<QProgressBar>
#include<QPushButton>
#include<QLabel>
#include<QTimer>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this);
	setWindowTitle(QString::fromLocal8Bit("信号发生器"));
	setWindowFlags(Qt::FramelessWindowHint);
	Base = ui.widget;
	vLayout = ui.verticalLayout;
	vLayout->setAlignment(Qt::AlignTop);
	scrollArea = ui.scrollArea;
	scrollArea->setWidgetResizable(true);
	scrollArea->setWidget(Base);
	scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setMinimumSize(640, 480);
	m_areaMovable = geometry();
	m_bPressed = false;
	createActions();
	createToolBars();
	addToolBar(Qt::LeftToolBarArea, MainToolBar); //TopToolBarArea
	font = QFont("Cambria", 12, 28, false);
	font.setBold(true);
	font_pe.setColor(QPalette::WindowText, Qt::white);
	palette.setBrush(QPalette::Background, QBrush(QPixmap("./Resources/bg.png")));
	setPalette(palette);
	//setStyleSheet("QMainWindow { background-color: #AACBEB; }");

	//窗口初始化
	audioView = new AudioWall();
	videoView = new VideoWall();
	setView = new SetView();
	historyView = new HistoryView();
	aboutView = new AboutView();

	connect(audioView, &AudioWall::updateAudioList, this, &MainWindow::updateAudioList);
	connect(audioView, &AudioWall::updateAudioProgress, this, &MainWindow::updateAudioProgress);
	connect(audioView, &AudioWall::updateAudioState, this, &MainWindow::updateAudioState);
	connect(videoView, &VideoWall::updateVideoList, this, &MainWindow::updateVideoList);
	connect(videoView, &VideoWall::updateVideoProgress, this, &MainWindow::updateVideoProgress);
	connect(videoView, &VideoWall::updateVideoState, this, &MainWindow::updateVideoState);
	connect(setView, &SetView::updateVH, this, &MainWindow::updateVH);
	connect(setView, &SetView::updateFramless, this, &MainWindow::updateFramless);
	connect(setView, &SetView::updateOpacity, this, &MainWindow::updateOpacity);
}

MainWindow::~MainWindow()
{
	delete historyView;
	delete setView;
	delete aboutView;
	delete audioView;
	delete videoView;
}

void MainWindow::createActions()
{
	audio = new QAction(QIcon("./Resources/audio.png"), tr("&audio"), this);
	audio->setStatusTip(tr("audio"));
	video = new QAction(QIcon("./Resources/video.png"), tr("&video"), this);
	video->setStatusTip(tr("video"));
	history = new QAction(QIcon("./Resources/history.png"), tr("&history"), this);
	history->setStatusTip(tr("history"));
	setting = new QAction(QIcon("./Resources/setting.png"), tr("&setting"), this);
	setting->setStatusTip(tr("setting"));
	about = new QAction(QIcon("./Resources/about.png"), tr("&about"), this);
	about->setStatusTip(tr("about"));
	shutdown = new QAction(QIcon("./Resources/shutdown.png"), tr("&shutdown"), this);
	shutdown->setStatusTip(tr("exit"));

	connect(audio, SIGNAL(triggered()), this, SLOT(AudioView()));
	connect(video, SIGNAL(triggered()), this, SLOT(VideoView()));
	connect(history, SIGNAL(triggered()), this, SLOT(History()));
	connect(setting, SIGNAL(triggered()), this, SLOT(Setting()));
	connect(about, SIGNAL(triggered()), this, SLOT(About()));
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
	int k = tasks.indexOf(task);
	rows[k]->close();
	rows.removeAt(k);
}

/*槽函数*/

void MainWindow::updateAudioList(AudioTask* task)
{
	if (task->isFinished)
	{
		deleteRow(task);
	}
	else
	{
		tasks.append(task);
		audioTasks.append(task);
		qDebug()
			<< "=============================================================\n"
			<< task->taskType << endl
			<< task->taskName << endl
			<< task->taskProgress << endl
			<< task->isFinished << endl
			<< "=============================================================";

		TaskRow *row = new TaskRow(Base, task);
		row->setFixedHeight(50);
		row->setWindowFlags(Qt::FramelessWindowHint);
		row->setPalette(palette);
		vLayout->addWidget(row);
		rows.append(row);
		row->show();
		connect(row->infoButton, SIGNAL(clicked()), this, SLOT(Info()));
	}
}

void MainWindow::updateVideoList(VideoTask* task)
{
	if (task->isFinished)
	{
		deleteRow(task);
	}
	else
	{
		tasks.append(task);
		videoTasks.append(task);

		qDebug()
			<< "============================================================="
			<< task->taskType << endl
			<< task->taskName << endl
			<< task->taskProgress << endl
			<< task->isFinished << endl
			<< "=============================================================";

		TaskRow *row = new TaskRow(Base, task);
		row->setFixedHeight(50);
		row->setWindowFlags(Qt::FramelessWindowHint);
		row->setPalette(palette);
		vLayout->addWidget(row);
		rows.append(row);
		row->show();
		connect(row->infoButton, SIGNAL(clicked()), this, SLOT(Info()));
	}
}

void MainWindow::updateAudioState(int n)
{
	int nth = tasks.indexOf(audioTasks[n]);
	QLabel *test = rows.at(nth)->findChild<QLabel *>("taskState");
	QAudio::State state = audioTasks[n]->getState();
	if (state == QAudio::ActiveState) test->setText("Playing");
	else if (state == QAudio::SuspendedState) test->setText("Paused");
	else test->setText("Stoped");
}

void MainWindow::updateVideoState(int n)
{
	int nth = tasks.indexOf(videoTasks[n]);
	QLabel *test = rows.at(nth)->findChild<QLabel *>("taskState");
	AVPlayer::State state = videoTasks[n]->getState();
	if (state == AVPlayer::PlayingState) test->setText("Playing");
	else if (state == AVPlayer::PausedState)  test->setText("Paused");
	else test->setText("Stoped");
}

void MainWindow::updateAudioProgress(int n, int value)
{
	int nth = tasks.indexOf(audioTasks[n]);
	QProgressBar *test = rows.at(nth)->findChild<QProgressBar *>("taskProgress");
	test->setValue(value);
}

void MainWindow::updateVideoProgress(int n, int value)
{
	int nth = tasks.indexOf(videoTasks[n]);
	QProgressBar *test = rows.at(nth)->findChild<QProgressBar *>("taskProgress");
	test->setValue(value);
}

void MainWindow::updateVH(bool value)
{
	addToolBar(value ? Qt::TopToolBarArea : Qt::LeftToolBarArea, MainToolBar);
	Base->setGeometry(QRect(value ? 0 : 60, value ? 60 : 0, 670, 500));
}

void MainWindow::updateOpacity(int value)
{
	setWindowOpacity((100.0 - value) / 100.0);
	audioView->setWindowOpacity((100.0 - value) / 100.0);
	videoView->setWindowOpacity((100.0 - value) / 100.0);
	historyView->setWindowOpacity((100.0 - value) / 100.0);
	aboutView->setWindowOpacity((100.0 - value) / 100.0);
}

void MainWindow::updateFramless(bool value)
{
	if (value)
	{
		setWindowFlags(Qt::FramelessWindowHint);
		audioView->setWindowFlags(Qt::FramelessWindowHint);
		videoView->setWindowFlags(Qt::FramelessWindowHint);
		historyView->setWindowFlags(Qt::FramelessWindowHint);
		aboutView->setWindowFlags(Qt::FramelessWindowHint);
	}
	else
	{
		setWindowFlags(Qt::WindowCloseButtonHint);
		audioView->setWindowFlags(Qt::WindowCloseButtonHint);
		videoView->setWindowFlags(Qt::WindowCloseButtonHint);
		historyView->setWindowFlags(Qt::WindowCloseButtonHint);
		aboutView->setWindowFlags(Qt::WindowCloseButtonHint);
	}
	show();
}

//菜单栏点击操作

void MainWindow::AudioView()
{
	audioView->show();
}

void MainWindow::VideoView()
{
	videoView->show();
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
	int nth = w->y() / 50;
	QMessageBox::about(0, tr("Help"),
		QString::fromLocal8Bit("文件绝对路径:")
		+ tasks.at(nth)->taskInfo.absoluteFilePath()
		+ QString::fromLocal8Bit("\n文件大小：")
		+ QString::number(tasks.at(nth)->taskInfo.size())
		+ QString::fromLocal8Bit("\n信号：")
		+ tasks.at(nth)->taskName
		+ QString::fromLocal8Bit("\n"));
}