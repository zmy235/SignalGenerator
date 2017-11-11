#include "VideoWall.h"
#include "BaseWidget.h"

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
#include <QtAV/AudioOutput.h>
#include <QtAVWidgets/QtAVWidgets>

using namespace QtAV;

typedef struct
{
	int screen_no;
	QRect rect;
}SCREEN;
SCREEN g_screens[10];
const int kSyncInterval = 2000;
VideoRendererId v = VideoRendererId_Widget;

VideoWall::VideoWall(QObject *parent) : QObject(parent)
{
	r = 3, c = 3, nth = 0, menu = 0;
}

VideoWall::~VideoWall()
{
	if (menu)
	{
		delete menu;
		menu = 0;
	}
	if (!tasks.isEmpty())
	{
		foreach(VideoTask *task, tasks)
		{
			task->video->stop();
			QList<VideoRenderer*> mRenderers = task->video->videoOutputs();
			foreach(VideoRenderer* renderer, mRenderers)
			{
				task->video->removeVideoRenderer(renderer);
				if (renderer->widget())
				{
					renderer->widget()->close();
					delete renderer;
				}
			}
			delete task->video;
			delete task->volumeSlider;
			delete task->timeSlider;
		}
		tasks.clear();
	}
	delete view;
}

void VideoWall::setRows(int n)
{
	r = n;
}

void VideoWall::setCols(int n)
{
	c = n;
}

int VideoWall::rows() const
{
	return r;
}

int VideoWall::cols() const
{
	return c;
}

void VideoWall::setView(BaseWidget *base)
{
	view = base;
	if (view)
	{
		view->setMinimumSize(640, 480);
		view->installEventFilter(this);
	}
}

void VideoWall::show()
{
	if (view)
	{
		//确定每一个render的尺寸
		int w = view->frameGeometry().width() / c;
		int h = view->frameGeometry().height() / r;
		QGridLayout *layout = new QGridLayout;
		layout->setSizeConstraint(QLayout::SetMaximumSize);
		layout->setSpacing(1);
		layout->setMargin(0);
		layout->setContentsMargins(0, 0, 0, 0);
		view->setLayout(layout);

		//视频渲染器的ID，即使用的解码器来源
		v = VideoRendererId_OpenGLWidget;

		QDesktopWidget *desktop = QApplication::desktop();
		int screen_count = desktop->screenCount();
		int prim_screen = desktop->primaryScreen();
		for (int i = 0; i < screen_count; i++)
		{
			g_screens[i].screen_no = prim_screen + i;
			g_screens[i].rect = desktop->screenGeometry(prim_screen + i);
		}

		//将每一个播放器放入播放器列表
		for (int i = 0; i < r; ++i)
		{
			for (int j = 0; j < c; ++j)
			{
				VideoRenderer* renderer = VideoRenderer::create(v);
				renderer->widget()->setWindowFlags(renderer->widget()->windowFlags() | Qt::FramelessWindowHint);
				renderer->widget()->setAttribute(Qt::WA_DeleteOnClose);
				renderer->widget()->resize(w, h);
				renderer->widget()->move(j*w, i*h);
				player = new AVPlayer();
				player->addVideoRenderer(renderer);

				//将每个播放器渲染器按格子布局放入
				((QGridLayout*)view->layout())->addWidget(renderer->widget(), i, j);
			}
		}

		view->show();
	}
}

void VideoWall::playAll()
{
	if (tasks.isEmpty()) return;
	foreach(VideoTask *task, tasks)
	{
		task->video->play();
	}
}

void VideoWall::stop()
{
	if (tasks.isEmpty()) return;
	foreach(VideoTask *task, tasks)
	{
		task->video->stop();
	}
}

void VideoWall::close()
{
	view->close();
}

void VideoWall::openLocalFile()
{
	QString file_path = QFileDialog::getOpenFileName(0, tr("Open a video"), "./", "Vedio(*.mp4 *.mkv)");
	if (file_path.isEmpty()) return;
	QFileInfo taskInfo = QFileInfo(file_path);
	player->setFile(file_path);
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
	qDebug("EventFilter::eventFilter to %p", watched);

	int w = view->frameGeometry().width() / c;
	int h = view->frameGeometry().height() / r;

	QEvent::Type type = event->type();
	switch (type)
	{
	case QEvent::MouseButtonPress:
	{
		QMouseEvent *m_event = static_cast<QMouseEvent*>(event);
		nth = m_event->y() / h * c + m_event->x() / w;
		qDebug("mouse eventFilter to %d, %d, %d", m_event->y() / h, m_event->x() / w, nth);
		if (m_event->button() == Qt::LeftButton)
		{
			if (!tasks[nth]->video->isPlaying()) tasks[nth]->video->play();
			else tasks[nth]->video->pause(!tasks[nth]->video->isPaused());
			emit updateVideoState(nth);
		}
	}

	case QEvent::KeyPress:
	{
		QKeyEvent *key_event = static_cast<QKeyEvent*>(event);
		Qt::KeyboardModifiers modifiers = key_event->modifiers();

		int key = key_event->key();
		switch (key)
		{
		case Qt::Key_F:
		{
			QWidget *w = qApp->activeWindow();
			if (!w) return false;
			if (w->isFullScreen()) w->showNormal();
			else w->showFullScreen();
		}
		break;

		case Qt::Key_N:
			foreach(VideoTask* task, tasks)
			{
				task->video->stepForward();
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
			nth = 0;
			foreach(VideoTask* task, tasks)
			{
				nth++;
				if (task->video->isPlaying())
				{
					task->video->pause(!task->video->isPaused());
					playing_count++;

					emit updateVideoState(nth);
				}
			}
			if (playing_count == 0) playAll();
		}
		break;

		case Qt::Key_Up:

			if (modifiers == Qt::ControlModifier)
			{
				AVPlayer* player = tasks[nth]->video;
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

			break;

		case Qt::Key_Down:

			if (modifiers == Qt::ControlModifier)
			{
				AVPlayer* player = tasks[nth]->video;
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
			break;

		case Qt::Key_M:

			if (modifiers == Qt::ControlModifier)
			{
				AVPlayer* player = tasks[nth]->video;
				if (player->audio())
				{
					player->audio()->setMute(!player->audio()->isMute());
				}
			}
			break;

		default:
			return false;
		}
		break;
	}

	case QEvent::ContextMenu:
	{
		QContextMenuEvent *e = static_cast<QContextMenuEvent*>(event);
		menu = new QMenu();
		menu->addAction(tr("Add Task"), this, SLOT(addVideoView()));
		menu->addAction(tr("Start All"), this, SLOT(playAll()));
		menu->addSeparator();
		menu->addAction(tr("Help"), this, SLOT(help()));
		menu->addAction(tr("Exit"), this->view, SLOT(close()));
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
		qDebug("mouse eventFilter to %d", nth);

		tasks[nth]->video->play(path);
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

	QVBoxLayout *vLayout = new QVBoxLayout(AddView);
	vLayout->setAlignment(Qt::AlignHCenter);
	vLayout->setSpacing(45);

	//VedioComboBox
	QComboBox *VedioComboBox;
	VedioComboBox = new QComboBox(AddView);
	VedioComboBox->setFixedSize(QSize(150, 45));
	VedioComboBox->insertItems(0, QStringList() << "AV" << "VGA" << "RGB" << "YPbPr" << "LVDS" << "DVI-D" << "SDI" << "FPDLINK" << "CameraLink");
	VedioComboBox->setStatusTip(tr("返回"));
	QFont font = QFont("Times", 16, 32, false);
	font.setBold(true);
	VedioComboBox->setFont(font);
	QPalette pe;
	pe.setColor(QPalette::ButtonText, Qt::white);
	VedioComboBox->setPalette(pe);
	VedioComboBox->setStyleSheet("QComboBox { background-color: #454545; }");
	vLayout->addWidget(VedioComboBox);
	connect(VedioComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(selectVideoType(int)));

	//VedioFileButton
	QPushButton *VedioFileButton;
	VedioFileButton = new QPushButton(QIcon("./Resources/file.png"), tr(""), AddView);
	VedioFileButton->setStatusTip(tr("Choose a File"));
	VedioFileButton->setFixedSize(QSize(45, 45));
	VedioFileButton->setIconSize(QSize(45, 45));
	vLayout->addWidget(VedioFileButton);
	connect(VedioFileButton, SIGNAL(clicked()), this, SLOT(openLocalFile()));


	QHBoxLayout *hLayout = new QHBoxLayout(AddView);
	vLayout->addLayout(hLayout);
	//Button
	QPushButton *BackButton;//取消
	BackButton = new QPushButton(QIcon("./Resources/cancel.png"), tr(""), AddView);
	BackButton->setStatusTip(tr("返回"));
	BackButton->setFixedSize(QSize(45, 45));
	BackButton->setIconSize(QSize(45, 45));
	hLayout->addWidget(BackButton);
	connect(BackButton, SIGNAL(clicked()), AddView, SLOT(close()));

	//Button
	QPushButton *VedioOKButton;//添加
	VedioOKButton = new QPushButton(QIcon("./Resources/ok.png"), tr(""), AddView);
	VedioOKButton->setStatusTip(tr("返回"));
	VedioOKButton->setFixedSize(QSize(45, 45));
	VedioOKButton->setIconSize(QSize(45, 45));
	hLayout->addWidget(VedioOKButton);
	connect(VedioOKButton, SIGNAL(clicked()), this, SLOT(VedioOK()));

	AddView->show();

}

void VideoWall::selectVideoType(const int &text)
{
	QVector<QString> vedioTypes = {
		tr("AV"),
		tr("VGA"),
		tr("RGB"),
		tr("YPbPr"),
		tr("LVDS"),
		tr("DVI"),
		tr("SDI"),
		tr("FPDLINK"),
		tr("CameraLink") };
	taskName = vedioTypes[text];
}

void VideoWall::VedioOK()
{
	taskTime = QDateTime::currentDateTime();

	//更新列表
	VideoTask* task = new VideoTask(tr("Video"), taskName, taskInfo, player);
	emit updateVideoList(task);

	VideoRenderer* renderer2 = VideoRenderer::create(v);
	renderer2->widget()->setWindowFlags(renderer2->widget()->windowFlags() | Qt::FramelessWindowHint);
	renderer2->widget()->setAttribute(Qt::WA_DeleteOnClose);
	renderer2->widget()->resize(g_screens[nth].rect.width(), g_screens[nth].rect.height());
	renderer2->widget()->move(g_screens[nth].rect.x(), g_screens[nth].rect.y());
	renderer2->widget()->show();

	player->addVideoRenderer(renderer2);
	player->play();

	AddView->close();
}
