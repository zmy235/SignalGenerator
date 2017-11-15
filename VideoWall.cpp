#include "VideoWall.h"

#include <Windows.h>
#include <QApplication>
#include <QDesktopWidget>
#include <QComboBox>
#include <QDebug>
#include <QEvent>
#include <QFileDialog>
#include <QFont>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QKeyEvent>
#include <QMenu>
#include <QMessageBox>
#include <QMimeData>
#include <QPalette>
#include <QPushButton>
#include <QPoint>
#include <QRect>
#include <QTextEdit>
#include <QtCore/QUrl>
#include <QUrl>
#include <QWidget>
#include <QToolTip>
#include <QtAV/AudioOutput.h>
#include <QtAVWidgets/QtAVWidgets>

using namespace QtAV;

typedef struct
{
	int screen_no;
	QRect rect;
}SCREEN;
SCREEN g_screens[10];

VideoRendererId v = VideoRendererId_Widget;

VideoWall::VideoWall(QWidget *parent) : QWidget(parent)
{
	setWindowTitle(QString::fromLocal8Bit("视频输出窗口"));
	setMinimumSize(640, 480);
	font = QFont("Times", 16, 32, false);
	font.setBold(true);
	pe.setColor(QPalette::ButtonText, Qt::white);

	installEventFilter(this);
	v = VideoRendererId_OpenGLWidget;//视频渲染器的ID，即使用的解码器来源

	QGridLayout *layout = new QGridLayout(this);
	layout->setSizeConstraint(QLayout::SetMaximumSize);
	layout->setSpacing(1);
	layout->setMargin(0);
	layout->setContentsMargins(0, 0, 0, 0);

	//将每一个播放器放入播放器列表
	r = 3, c = 3, nth = 0, menu = 0;
	int w = this->frameGeometry().width() / c;
	int h = this->frameGeometry().height() / r;
	for (int i = 0; i < r; ++i)
	{
		for (int j = 0; j < c; ++j)
		{
			VideoRenderer* renderer = VideoRenderer::create(v);
			QWidget *rendererWidget = renderer->widget();
			rendererWidget->setWindowFlags(rendererWidget->windowFlags() | Qt::FramelessWindowHint);
			rendererWidget->setAttribute(Qt::WA_DeleteOnClose);
			rendererWidget->resize(w, h);
			rendererWidget->setAcceptDrops(true);
			rendererWidget->move(j*w, i*h);
			QVBoxLayout *layout = new QVBoxLayout(rendererWidget);
			layout->setAlignment(Qt::AlignBottom);
			((QGridLayout*) this->layout())->addWidget(rendererWidget, i, j);//将每个播放器渲染器按格子布局放入
			rendererWidgets.append(rendererWidget);

			AVPlayer* player = new AVPlayer();
			player->addVideoRenderer(renderer);
			players.append(player);

			QSlider* timeSlider = new QSlider(rendererWidget);
			timeSlider->setOrientation(Qt::Horizontal);
			timeSlider->setMinimum(0);
			timeSlider->setMaximum(100);
			timeSlider->setFixedHeight(15);
			timeSlider->setValue(0);
			timeSlider->setEnabled(true);
			timeSliders.append(timeSlider);
			QSlider* VolumeSlider = new QSlider(rendererWidget);
			VolumeSlider->setTracking(true);//实时改变
			VolumeSlider->setOrientation(Qt::Horizontal);
			VolumeSlider->setMinimum(0);
			VolumeSlider->setMaximum(100);
			VolumeSlider->setFixedHeight(15);
			VolumeSlider->setValue(0);
			VolumeSlider->setEnabled(true);
			volumeSliders.append(VolumeSlider);

			QHBoxLayout *hlayout = new QHBoxLayout();
			hlayout->setSpacing(20);
			hlayout->addWidget(timeSlider, 5);
			hlayout->addWidget(VolumeSlider, 1);
			layout->addLayout(hlayout);

			connect(timeSlider, SIGNAL(sliderMoved(int)), this, SLOT(setPlayerPosition(int)));
			connect(VolumeSlider, SIGNAL(sliderMoved(int)), this, SLOT(setVolume(int)));
			connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(setSliderPosition(qint64)));
		}
	}

	//设置屏
	QDesktopWidget *desktop = QApplication::desktop();
	int screen_count = desktop->screenCount();
	int prim_screen = desktop->primaryScreen();
	for (int i = 0; i < screen_count; i++)
	{
		g_screens[i].screen_no = prim_screen + i;
		g_screens[i].rect = desktop->screenGeometry(prim_screen + i);
	}

}

VideoWall::~VideoWall()
{
	if (menu)delete menu;
	foreach(AVPlayer *p, players) delete p;
	foreach(QSlider *t, timeSliders) delete t;
	foreach(QSlider *v, volumeSliders) delete v;
	foreach(QWidget *r, rendererWidgets) delete r;
	if (!tasks.isEmpty())
	{
		foreach(VideoTask *task, tasks) delete task;
		taskMap.clear();
	}
}

void VideoWall::setRows(int n)
{
	r = n;
}

void VideoWall::setCols(int n)
{
	c = n;
}

int VideoWall::getRows() const
{
	return r;
}

int VideoWall::getCols() const
{
	return c;
}

void VideoWall::playAll()
{
	if (tasks.isEmpty()) return;
	foreach(VideoTask *task, tasks)
	{
		task->video->play();
	}
}

void VideoWall::stopAll()
{
	if (tasks.isEmpty()) return;
	foreach(VideoTask *task, tasks)
	{
		task->video->stop();
	}
}

void VideoWall::openLocalFile()
{
	QString file_path = QFileDialog::getOpenFileName(0, tr("Open a video"), "./", "Vedio(*.mp4 *.mkv *.avi *.wmv)");
	if (file_path.isEmpty()) return;
	taskInfo = QFileInfo(file_path);
	players[nth]->setFile(file_path);
}

void VideoWall::help()
{
	QMessageBox::about(0, tr("Help"),
		tr("Drag and drop a file to player\n")
		+ tr("Space: pause/continue\n")
		+ tr("N: next frame\n")
		+ tr("O: open a file\n")
		+ tr("P: replay\n")
		+ tr("S: stop\n")
		+ tr("M: mute on/off\n")
		+ tr("C: capture video")
		+ tr("Up/Down: volume +/-\n"));
}

bool VideoWall::eventFilter(QObject *watched, QEvent *event)
{
	//qDebug("EventFilter::eventFilter to %p", watched);
	int w = frameGeometry().width() / c;
	int h = frameGeometry().height() / r;
	switch (event->type())
	{
	case QEvent::MouseButtonPress:
	{
		QMouseEvent *e = static_cast<QMouseEvent*>(event);
		nth = e->y() / h * c + e->x() / w;
		qDebug("mouse eventFilter to %d, %d, %d", e->y() / h, e->x() / w, nth);
		if (taskMap.find(nth) != taskMap.end())
			if (e->button() == Qt::LeftButton)
			{
				if (!taskMap[nth]->video->isPlaying()) taskMap[nth]->video->play();
				else taskMap[nth]->video->pause(!taskMap[nth]->video->isPaused());
				emit updateVideoState(nth);
			}
	}
	break;

	case QEvent::KeyPress:
	{
		QKeyEvent *key_event = static_cast<QKeyEvent*>(event);
		Qt::KeyboardModifiers modifiers = key_event->modifiers();

		switch (key_event->key())
		{
		case Qt::Key_F:
		{
			QWidget *w = qApp->activeWindow();
			if (!w) return false;
			if (!(w->isFullScreen())) w->showFullScreen();
		}
		break;

		case Qt::Key_N:
		{
			foreach(VideoTask* task, tasks)
			{
				task->video->stepForward();
			}
		}
		break;

		case Qt::Key_O:
		{
			if (modifiers == Qt::ControlModifier)
			{
				openLocalFile();
				return true;
			}
			else
			{
				return false;
			}
		}
		break;

		case Qt::Key_Space:
		{
			int playing_count = 0;
			foreach(VideoTask* task, tasks)
			{
				if (task->video->isPlaying())
				{
					task->video->pause(!task->video->isPaused());
					playing_count++;
					emit updateVideoState(playing_count);
				}
			}
			if (playing_count == 0) playAll();
		}
		break;

		case Qt::Key_Up:
		{
			if (modifiers == Qt::ControlModifier)
			{
				AVPlayer* player = taskMap[nth]->video;
				if (player->audio())
				{
					qreal v = player->audio()->volume();
					if (v > 0.5)
						v += 0.1;
					else if (v > 0.1)
						v += 0.05;
					else
						v += 0.025;
					player->audio()->setVolume(v);
				}
			}
		}
		break;

		case Qt::Key_Down:
		{
			if (modifiers == Qt::ControlModifier)
			{
				AVPlayer* player = taskMap[nth]->video;
				if (player->audio())
				{
					qreal v = player->audio()->volume();
					if (v > 0.5)
						v -= 0.1;
					else if (v > 0.1)
						v -= 0.05;
					else
						v -= 0.025;
					player->audio()->setVolume(v);
				}
			}
		}
		break;

		case Qt::Key_M:
		{
			if (modifiers == Qt::ControlModifier)
			{
				AVPlayer* player = taskMap[nth]->video;
				if (player->audio())
				{
					player->audio()->setMute(!player->audio()->isMute());
				}
			}
		}
		break;

		default:
			return false;
		}
	}
	break;

	case QEvent::ContextMenu:
	{
		QContextMenuEvent *e = static_cast<QContextMenuEvent*>(event);
		nth = e->y() / h * c + e->x() / w;
		menu = new QMenu();
		menu->addAction(tr("Add Task"), this, SLOT(addVideoView()));
		menu->addAction(tr("Start All"), this, SLOT(playAll()));
		menu->addSeparator();
		menu->addAction(tr("Help"), this, SLOT(help()));
		menu->addAction(tr("Exit"), this, SLOT(close()));
		menu->popup(e->globalPos());
		menu->exec();
	}
	break;

	case QEvent::DragEnter:
	case QEvent::DragMove:
	{
		QDropEvent *e = static_cast<QDropEvent*>(event);
		e->acceptProposedAction();
	}
	break;

	case QEvent::Drop:
	{
		QDropEvent *e = static_cast<QDropEvent*>(event);
		QString path = e->mimeData()->urls().first().toLocalFile();
		nth = e->pos().y() / h * c + e->pos().x() / w;
		qDebug("droped to %dth render just now.", nth);
		players[nth]->play(path);
		e->acceptProposedAction();
	}
	break;

	default:
		return false;
	}
	return true; //false: for text input
}

void VideoWall::addVideoView()
{
	AddView = new BaseWidget();
	AddView->setFixedSize(500, 350);

	VedioComboBox = new QComboBox(AddView);
	VedioComboBox->setFixedSize(QSize(150, 45));
	VedioComboBox->insertItems(0, QStringList() << "AV" << "VGA" << "RGB" << "YPbPr" << "LVDS" << "DVI-D" << "SDI" << "FPDLINK" << "CameraLink");
	VedioComboBox->setFont(font);
	VedioComboBox->setPalette(pe);
	VedioComboBox->setStyleSheet("QComboBox { background-color: #454545; }");
	QPushButton *VedioFileButton;
	VedioFileButton = new QPushButton(QIcon("./Resources/file.png"), tr(""), AddView);
	VedioFileButton->setStatusTip(tr("Choose a File"));
	VedioFileButton->setFixedSize(QSize(45, 45));
	VedioFileButton->setIconSize(QSize(45, 45));
	QPushButton *BackButton;//取消
	BackButton = new QPushButton(QIcon("./Resources/cancel.png"), tr(""), AddView);
	BackButton->setStatusTip(tr("返回"));
	BackButton->setFixedSize(QSize(45, 45));
	BackButton->setIconSize(QSize(45, 45));
	QPushButton *VedioOKButton;//添加
	VedioOKButton = new QPushButton(QIcon("./Resources/ok.png"), tr(""), AddView);
	VedioOKButton->setStatusTip(tr("返回"));
	VedioOKButton->setFixedSize(QSize(45, 45));
	VedioOKButton->setIconSize(QSize(45, 45));
	QVBoxLayout *vLayout = new QVBoxLayout(AddView);
	vLayout->setAlignment(Qt::AlignHCenter);
	vLayout->setSpacing(45);
	vLayout->addWidget(VedioComboBox);
	vLayout->addWidget(VedioFileButton);
	QHBoxLayout *hLayout = new QHBoxLayout(AddView);
	hLayout->addWidget(BackButton);
	hLayout->addWidget(VedioOKButton);
	vLayout->addLayout(hLayout);
	AddView->show();

	connect(VedioComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setVideoType(int)));
	connect(VedioFileButton, SIGNAL(clicked()), this, SLOT(openLocalFile()));
	connect(BackButton, SIGNAL(clicked()), AddView, SLOT(close()));
	connect(VedioOKButton, SIGNAL(clicked()), this, SLOT(VedioOK()));
}

void VideoWall::setVideoType(const int &index)
{
	taskName = VedioComboBox->itemText(index);
}

void VideoWall::VedioOK()
{
	VideoTask* task = new VideoTask(tr("Video"), taskName, taskInfo, players[nth]);
	task->taskTime = QDateTime::currentDateTime();
	task->timeSlider = timeSliders[nth];
	task->volumeSlider = volumeSliders[nth];
	emit updateVideoList(task);
	tasks.append(task);
	taskMap[nth] = task;

	VideoRenderer* renderer = VideoRenderer::create(v);
	QWidget *rendererWidget = renderer->widget();
	rendererWidget->setWindowFlags(rendererWidget->windowFlags() | Qt::FramelessWindowHint);
	rendererWidget->setAttribute(Qt::WA_DeleteOnClose);
	rendererWidget->resize(g_screens[nth].rect.width(), g_screens[nth].rect.height());
	rendererWidget->move(g_screens[nth].rect.x(), g_screens[nth].rect.y());
	//rendererWidget->show();
	//player->addVideoRenderer(renderer);
	players[nth]->play();
	AddView->close();
}

void VideoWall::setSliderPosition(qint64 pos)
{
	AVPlayer * s = qobject_cast<AVPlayer *>(sender());
	int n = players.indexOf(s);
	timeSliders[n]->setValue(pos * 100 / s->mediaStopPosition());
}

void VideoWall::setPlayerPosition(int value)
{
	QSlider *s = qobject_cast<QSlider *>(sender());
	int n = timeSliders.indexOf(s);
	qint64 p = (value * players[n]->mediaStopPosition()) / 100;
	qDebug() << "seekPosition:" << p << value;
	players[n]->setPosition(p);
}

void VideoWall::setVolume(int value)
{
	QSlider *test = qobject_cast<QSlider *>(sender());
	nth = volumeSliders.indexOf(test);
	qDebug() << nth << "\n" << value;
	if (taskMap[nth])
	{
		AVPlayer *temp = tasks[nth]->video;
		if (temp) temp->audio()->setVolume(value*1.0 / 100.0);
	}
	test->setToolTip(QString::number(value));
}