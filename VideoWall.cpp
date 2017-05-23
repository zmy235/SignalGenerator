#include "VideoWall.h"
#include "BaseWidget.h"
#include "Task.h"

#include <Windows.h>
#include <QApplication>
#include <QComboBox>
#include <QDesktopWidget>
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
#include <QtAV/AudioOutput.h>
#include <QtAVWidgets>
#include <QUrl>
#include <QWidget>

using namespace QtAV;
const int kSyncInterval = 2000;

VideoWall::VideoWall(QObject *parent) : QObject(parent), r(3), c(3), nth(0), menu(0), vid(QString::fromLatin1("qpainter"))
{
	QtAV::Widgets::registerRenderers(); 
	vedioTypes = { tr("AV"), tr("VGA"), tr("RGB"), tr("YPbPr"), tr("LVDS"), tr("DVI"), tr("SDI"), tr("FPDLINK"), tr("CameraLink") };
}

VideoWall::~VideoWall()
{
	if (menu)
	{
		delete menu;
		menu = 0;
	}
	if (!players.isEmpty())
	{
		foreach(AVPlayer *player, players)
		{
			player->stop();
			VideoRenderer* renderer = player->renderer();
			if (renderer->widget())
			{
				renderer->widget()->close();
				if (!renderer->widget()->testAttribute(Qt::WA_DeleteOnClose) && !renderer->widget()->parent())
					delete renderer;
				delete player;
			}
		}
		players.clear();
	}
	delete view;
}

void VideoWall::setVideoRendererTypeString(const QString &vt)
{
	vid = vt.toLower();
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
		//qDebug("WA_OpaquePaintEvent=%d", view->testAttribute(Qt::WA_OpaquePaintEvent));
		//view->show();
	}
	view->installEventFilter(this);
}

void VideoWall::show()
{
	//清空当前播放器列表
	if (!players.isEmpty())
	{
		foreach(AVPlayer *player, players)
		{
			player->stop();
			VideoRenderer* renderer = player->renderer();
			if (renderer->widget())
			{
				renderer->widget()->close();
				if (!renderer->widget()->testAttribute(Qt::WA_DeleteOnClose) && !renderer->widget()->parent())
					delete renderer;
				delete player;
			}
		}
		players.clear();
	}
	qDebug("show wall: %d x %d", r, c);

	//确定每一个render的尺寸
	int w = view ? view->frameGeometry().width() / c : qApp->desktop()->width() / c;
	int h = view ? view->frameGeometry().height() / r : qApp->desktop()->height() / r;
	if (view)
	{
		QGridLayout *layout = new QGridLayout;
		layout->setSizeConstraint(QLayout::SetMaximumSize);
		layout->setSpacing(1);
		layout->setMargin(0);
		layout->setContentsMargins(0, 0, 0, 0);
		view->setLayout(layout);
	}

	//视频渲染器的ID，即使用的解码器来源
	VideoRendererId v = VideoRendererId_Widget;
	if (vid == QLatin1String("gl"))
		v = VideoRendererId_GLWidget2;
	else if (vid == QLatin1String("opengl"))
		v = VideoRendererId_OpenGLWidget;
	else if (vid == QLatin1String("d2d"))
		v = VideoRendererId_Direct2D;
	else if (vid == QLatin1String("gdi"))
		v = VideoRendererId_GDI;
	else if (vid == QLatin1String("x11"))
		v = VideoRendererId_X11;
	else if (vid == QLatin1String("xv"))
		v = VideoRendererId_XV;

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

			AVPlayer *player = new AVPlayer;
			player->setRenderer(renderer);
			players.append(player);

			//将每个播放器渲染器按格子布局放入
			if (view) ((QGridLayout*)view->layout())->addWidget(renderer->widget(), i, j);
		}
	}
	view->show();
}

void VideoWall::play()
{
	if (players.isEmpty()) return;
	foreach(AVPlayer *player, players)
	{
		player->play();
	}
}

void VideoWall::stop()
{
	if (players.isEmpty()) return;
	foreach(AVPlayer* player, players)
	{
		player->stop();
	}
}

void VideoWall::close()
{
	view->close();
}

void VideoWall::openLocalFile()
{
	QString file = QFileDialog::getOpenFileName(0, tr("Open a video"), "./", "Vedio(*.mp4 *.mkv)");
	if (file.isEmpty()) return;
	players.at(nth)->setFile(file);
	players.at(nth)->play();
}

void VideoWall::openUrl()
{
	QString url = QInputDialog::getText(0, tr("Open an url"), tr("Url"));
	if (url.isEmpty()) return;
	foreach(AVPlayer* player, players)
	{
		player->setFile(url);
		player->play();
	}
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

	if (players.isEmpty()) return false;

	QEvent::Type type = event->type();
	switch (type)
	{
	case QEvent::MouseButtonPress:
	{
		QMouseEvent *m_event = static_cast<QMouseEvent*>(event);
		nth = m_event->y() / h * c + m_event->x() / w;
		qDebug("mouse eventFilter to %d", nth);
		if (m_event->button() == Qt::LeftButton)
		{
			if (!players.at(nth)->isPlaying()) players.at(nth)->play();
			else players.at(nth)->pause(!players.at(nth)->isPaused());
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
			foreach(AVPlayer* player, players)
			{
				player->stepForward();
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
			foreach(AVPlayer* player, players)
			{
				if (player->isPlaying())
				{
					player->pause(!player->isPaused());
					playing_count++;
				}
			}
			if (playing_count == 0) play();
		}
		break;

		case Qt::Key_Up:
			foreach(AVPlayer* player, players)
			{
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
			foreach(AVPlayer* player, players)
			{
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
			foreach(AVPlayer* player, players)
			{
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
		if (!menu)
		{
			menu = new QMenu();
			menu->addAction(tr("Add Task"), this, SLOT(addVideoView()));
			menu->addAction(tr("Open Path"), this, SLOT(openUrl()));
			menu->addSeparator();
			menu->addAction(tr("Help"), this, SLOT(help()));
			menu->addAction(tr("Exit"), this->view, SLOT(close()));
		}
		menu->popup(e->globalPos());
		menu->exec();
	}
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

		players.at(nth)->play(path);
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
	AddView->setGeometry(QRect(0, 50, 500, 350));

	tasks[nth].type = "vedio";

	//VedioComboBox
	QComboBox *VedioComboBox;
	VedioComboBox = new QComboBox(AddView);
	VedioComboBox->insertItems(0, QStringList() << "AV" << "VGA" << "RGB" << "YPbPr" << "LVDS" << "DVI" << "SDI" << "FPDLINK" << "CameraLink");
	VedioComboBox->setStatusTip(tr("返回"));
	QFont font = QFont("Times", 16, 32, false);
	font.setBold(true);
	VedioComboBox->setFont(font);
	QPalette pe;
	pe.setColor(QPalette::ButtonText, Qt::white);
	VedioComboBox->setPalette(pe);
	VedioComboBox->setGeometry(QRect(100, 0, 160, 45));
	VedioComboBox->setStyleSheet("QComboBox { background-color: #454545; }");
	connect(VedioComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_sel_vedio(int)));

	//VedioFileButton
	QPushButton *VedioFileButton;//添加
	VedioFileButton = new QPushButton(QIcon("./Resources/file.png"), tr(""), AddView);
	VedioFileButton->setStatusTip(tr("Choose a File"));
	VedioFileButton->setFixedSize(QSize(45, 45));
	VedioFileButton->setIconSize(QSize(45, 45));
	VedioFileButton->setGeometry(QRect(100, 50, 45, 45));
	connect(VedioFileButton, SIGNAL(clicked()), this, SLOT(openLocalFile()));

	//Button
	QPushButton *BackButton;//取消
	BackButton = new QPushButton(QIcon("./Resources/cancel.png"), tr(""), AddView);
	BackButton->setStatusTip(tr("返回"));
	BackButton->setFixedSize(QSize(45, 45));
	BackButton->setIconSize(QSize(45, 45));
	BackButton->setGeometry(QRect(120, 260, 45, 45));
	connect(BackButton, SIGNAL(clicked()), AddView, SLOT(close()));

	//Button
	QPushButton *VedioOKButton;//添加
	VedioOKButton = new QPushButton(QIcon("./Resources/ok.png"), tr(""), AddView);
	VedioOKButton->setStatusTip(tr("返回"));
	VedioOKButton->setFixedSize(QSize(45, 45));
	VedioOKButton->setIconSize(QSize(45, 45));
	VedioOKButton->setGeometry(QRect(250, 260, 45, 45));
	connect(VedioOKButton, SIGNAL(clicked()), this, SLOT(VedioOK()));

	AddView->show();

}

void VideoWall::on_sel_vedio(const int &text)
{
	tasks[nth].type = tr("Vedio") + vedioTypes[text];
}

void VideoWall::VedioOK()
{
	tasks[nth].time = QDateTime::currentDateTime();
	tasks[nth].progress = 0;
	tasks[nth].playing = true;
	
	//更新
	emit updateList(&tasks[nth]);
	this->close();
}
