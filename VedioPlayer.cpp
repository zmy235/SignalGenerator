#include "VedioPlayer.h"
#include <QTimer>

VedioPlayer::VedioPlayer(QWidget *parent) : BaseWidget(parent), mpPlayer(0), mpVo(0), mpTempRenderer(0)
{
	setWindowTitle("QtAV Player");
	setupUi();
}

VedioPlayer::~VedioPlayer()
{
	if (m_preview) 
	{
		m_preview->close();
		delete m_preview;
	}
	if (mpVolumeSlider && !mpVolumeSlider->parentWidget()) 
	{
		mpVolumeSlider->close();
		delete mpVolumeSlider;
		mpVolumeSlider = 0;
	}
}

void VedioPlayer::setupUi()
{
	//播放器主窗体布局
	mainLayout = new QVBoxLayout();
	mainLayout->setSpacing(0);
	mainLayout->setMargin(0);
	setLayout(mainLayout);

	mpControl = new QWidget(this);
	//mpControl->setMaximumHeight(30);

	mpTimeSlider = new Slider(mpControl);
	mpTimeSlider->setDisabled(true);
	mpTimeSlider->setTracking(true);
	mpTimeSlider->setOrientation(Qt::Horizontal);
	mpTimeSlider->setMinimum(0);

	mpCurrent = new QLabel(mpControl);
	mpCurrent->setToolTip(tr("Current time"));
	mpCurrent->setMargin(2);
	mpCurrent->setText(QString::fromLatin1("00:00:00"));

	mpEnd = new QLabel(mpControl);
	mpEnd->setToolTip(tr("Duration"));
	mpEnd->setMargin(2);
	mpEnd->setText(QString::fromLatin1("00:00:00"));

	mpPlayPauseBtn = new QToolButton(mpControl);
	mpPlayPauseBtn->setToolTip(tr("Play"));
	mpPlayPauseBtn->setIcon(QIcon(QString::fromLatin1("./image/play.png")));
	mpStopBtn = new QToolButton(mpControl);
	mpStopBtn->setToolTip(tr("Stop"));
	mpStopBtn->setIcon(QIcon(QString::fromLatin1("./image/stop.png")));
	mpBackwardBtn = new QToolButton(mpControl);
	mpBackwardBtn->setToolTip(tr("Backward"));
	mpBackwardBtn->setIcon(QIcon(QString::fromLatin1("./image/backward.png")));
	mpForwardBtn = new QToolButton(mpControl);
	mpForwardBtn->setToolTip(tr("Forward"));
	mpForwardBtn->setIcon(QIcon(QString::fromLatin1("./image/forward.png")));
	mpOpenBtn = new QToolButton(mpControl);
	mpOpenBtn->setToolTip(tr("Open"));
	mpOpenBtn->setIcon(QIcon(QString::fromLatin1("./image/open.png")));
	mpVolumeBtn = new QToolButton();
	mpVolumeBtn->setToolTip(tr("Volume"));
	mpVolumeBtn->setIcon(QIcon(QString::fromLatin1("./image/sound.png")));

	mpVolumeSlider = new Slider();
	mpVolumeSlider->hide();
	mpVolumeSlider->setOrientation(Qt::Horizontal);
	mpVolumeSlider->setMinimum(0);
	mpVolumeSlider->setMaximum(kVolumeSliderMax);
	mpVolumeSlider->setMaximumHeight(12);
	mpVolumeSlider->setMaximumWidth(88);
	mpVolumeSlider->setValue(int(1.0 / kVolumeInterval*qreal(kVolumeSliderMax) / 100.0));
	setVolume();

	pRepeatLabel = new QLabel(tr("Times"));
	vb = new QVBoxLayout;

	hb = new QHBoxLayout;
	hb->addWidget(pRepeatLabel);
	hb->addWidget(mpRepeatBox);
	vb->addLayout(hb);

	pRepeatLabel = new QLabel(tr("From"));
	mpRepeatA = new QTimeEdit();
	mpRepeatA->setDisplayFormat(QString::fromLatin1("HH:mm:ss"));
	mpRepeatA->setToolTip(tr("negative value means from the end"));
	connect(mpRepeatA, SIGNAL(timeChanged(QTime)), SLOT(repeatAChanged(QTime)));

	hb = new QHBoxLayout;
	hb->addWidget(pRepeatLabel);
	hb->addWidget(mpRepeatA);
	vb->addLayout(hb);

	pRepeatLabel = new QLabel(tr("To"));
	mpRepeatB = new QTimeEdit();
	mpRepeatB->setDisplayFormat(QString::fromLatin1("HH:mm:ss"));
	mpRepeatB->setToolTip(tr("negative value means from the end"));
	connect(mpRepeatB, SIGNAL(timeChanged(QTime)), SLOT(repeatBChanged(QTime)));

	hb = new QHBoxLayout;
	hb->addWidget(pRepeatLabel);
	hb->addWidget(mpRepeatB);
	vb->addLayout(hb);

	QWidget *wgt = new QWidget;
	wgt->setLayout(vb);

	QWidgetAction *pWA = new QWidgetAction(0);
	pWA->setDefaultWidget(wgt);
	pWA->defaultWidget()->setEnabled(false);
	mpRepeatAction = pWA;

	mpVo = new VideoOutput(this);
	if (!mpVo->widget())
	{
		QMessageBox::warning(0, QString::fromLatin1("QtAV error"), tr("Can not create video renderer"));
		return;
	}

	mpPlayer = new AVPlayer(this);
	mpPlayer->setRenderer(mpVo);
	mainLayout->addWidget(mpVo->widget());
	mainLayout->addWidget(mpTimeSlider);
	mainLayout->addWidget(mpControl);

	controlLayout = new QHBoxLayout();
	controlLayout->setSpacing(0);
	controlLayout->setMargin(1);
	mpControl->setLayout(controlLayout);
	controlLayout->addWidget(mpCurrent);
	QSpacerItem *space = new QSpacerItem(mpPlayPauseBtn->width(), mpPlayPauseBtn->height(), QSizePolicy::MinimumExpanding);
	controlLayout->addSpacerItem(space);
	controlLayout->addWidget(mpVolumeSlider);
	controlLayout->addWidget(mpVolumeBtn);
	controlLayout->addWidget(mpStopBtn);
	controlLayout->addWidget(mpBackwardBtn);
	controlLayout->addWidget(mpPlayPauseBtn);
	controlLayout->addWidget(mpForwardBtn);
	controlLayout->addWidget(mpOpenBtn);
	controlLayout->addWidget(mpEnd);

	connect(mpOpenBtn, SIGNAL(clicked()), SLOT(openFile()));
	connect(mpPlayPauseBtn, SIGNAL(clicked()), SLOT(togglePlayPause()));

	mIsReady = true;
	//    connect(mpTimeSlider, SIGNAL(sliderMoved(int)), SLOT(seek(int)));
	connect(mpTimeSlider, SIGNAL(sliderPressed()), SLOT(seek()));
	connect(mpTimeSlider, SIGNAL(sliderReleased()), SLOT(seek()));
	connect(mpTimeSlider, SIGNAL(onLeave()), SLOT(onTimeSliderLeave()));
	connect(mpTimeSlider, SIGNAL(onHover(int, int)), SLOT(onTimeSliderHover(int, int)));
	QTimer::singleShot(0, this, SLOT(initPlayer()));
}

void VedioPlayer::initPlayer()
{
	connect(mpVolumeBtn, SIGNAL(clicked()), SLOT(showHideVolumeBar()));
	connect(mpVolumeSlider, SIGNAL(sliderPressed()), SLOT(setVolume()));
	connect(mpVolumeSlider, SIGNAL(valueChanged(int)), SLOT(setVolume()));
	connect(mpStopBtn, SIGNAL(clicked()), this, SLOT(stopUnload()));
	connect(mpForwardBtn, SIGNAL(clicked()), mpPlayer, SLOT(seekForward()));
	connect(mpBackwardBtn, SIGNAL(clicked()), mpPlayer, SLOT(seekBackward()));
	connect(mpPlayer, SIGNAL(seekFinished()), SLOT(onSeekFinished()));
	connect(mpPlayer, SIGNAL(error(QtAV::AVError)), this, SLOT(handleError(QtAV::AVError)));
	connect(mpPlayer, SIGNAL(started()), this, SLOT(onStartPlay()));
	connect(mpPlayer, SIGNAL(stopped()), this, SLOT(onStopPlay()));
	connect(mpPlayer, SIGNAL(paused(bool)), this, SLOT(onPaused(bool)));
	//    connect(mpPlayer, SIGNAL(speedChanged(qreal)), this, SLOT(onSpeedChange(qreal)));
	connect(mpPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(onPositionChange(qint64)));

}

void VedioPlayer::stopUnload()
{
	mpPlayer->stop();
	//    mpPlayer->unload();
}

void VedioPlayer::handleError(const AVError &e)
{
	QMessageBox::warning(0, tr("Player error"), e.string());
}

void VedioPlayer::onStartPlay()
{
	mFile = mpPlayer->file(); //open from EventFilter's menu
	mTitle = mFile;
	if (!mFile.contains(QLatin1String("://")) || mFile.startsWith(QLatin1String("file://")))
		mTitle = QFileInfo(mFile).fileName();
	setWindowTitle(mTitle);

	mpPlayPauseBtn->setIcon(QIcon(QString::fromLatin1("./image/pause.png")));
	mpTimeSlider->setMinimum(mpPlayer->mediaStartPosition());
	mpTimeSlider->setMaximum(mpPlayer->mediaStopPosition());
	mpTimeSlider->setValue(0);
	mpTimeSlider->setEnabled(mpPlayer->isSeekable());
	mpEnd->setText(QTime(0, 0, 0).addMSecs(mpPlayer->mediaStopPosition()).toString(QString::fromLatin1("HH:mm:ss")));
	setVolume();

	QTimer::singleShot(3000, this, SLOT(tryHideControlBar()));
	mpRepeatA->setMinimumTime(QTime(0, 0, 0).addMSecs(mpPlayer->mediaStartPosition()));
	mpRepeatA->setMaximumTime(QTime(0, 0, 0).addMSecs(mpPlayer->mediaStopPosition()));
	mpRepeatB->setMinimumTime(QTime(0, 0, 0).addMSecs(mpPlayer->mediaStartPosition()));
	mpRepeatB->setMaximumTime(QTime(0, 0, 0).addMSecs(mpPlayer->mediaStopPosition()));
	mpRepeatA->setTime(QTime(0, 0, 0).addMSecs(mpPlayer->startPosition()));
	mpRepeatB->setTime(QTime(0, 0, 0).addMSecs(mpPlayer->stopPosition()));
	mCursorTimer = startTimer(3000);
}

void VedioPlayer::onStopPlay()
{
	if (mpPlayer->currentRepeat() < mpPlayer->repeat())
		return;
	mpPlayPauseBtn->setIcon(QIcon(QString::fromLatin1("./image/play.png")));
	mpTimeSlider->setValue(0);
	qDebug(">>>>>>>>>>>>>>disable slider");
	mpTimeSlider->setDisabled(true);
	mpTimeSlider->setMinimum(0);
	mpTimeSlider->setMaximum(0);
	mpCurrent->setText(QString::fromLatin1("00:00:00"));
	mpEnd->setText(QString::fromLatin1("00:00:00"));
	tryShowControlBar();
	toggleRepeat(false);
	//    mRepeateMax = 0;
	killTimer(mCursorTimer);
	unsetCursor();
	if (m_preview)
		m_preview->setFile(QString());
}

void VedioPlayer::seek()
{
	mpPlayer->setSeekType(AccurateSeek);
	mpPlayer->seek((qint64)mpTimeSlider->value());
}

void VedioPlayer::onTimeSliderLeave()
{
	if (m_preview && m_preview->isVisible())
		m_preview->hide();
}

void VedioPlayer::onTimeSliderHover(int pos, int value)
{
	QPoint gpos = mapToGlobal(mpTimeSlider->pos() + QPoint(pos, 0));
	QToolTip::showText(gpos, QTime(0, 0, 0).addMSecs(value).toString(QString::fromLatin1("HH:mm:ss")));
	if (!m_preview)
		m_preview = new VideoPreviewWidget();
	m_preview->setFile(mpPlayer->file());
	m_preview->setTimestamp(value);
	m_preview->setWindowFlags(m_preview->windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
}

void VedioPlayer::onPaused(bool p)
{
	if (p) {
		qDebug("start pausing...");
		mpPlayPauseBtn->setIcon(QIcon(QString::fromLatin1("./image/play.png")));
	}
	else {
		qDebug("stop pausing...");
		mpPlayPauseBtn->setIcon(QIcon(QString::fromLatin1("./image/pause.png")));
	}
}

void VedioPlayer::showHideVolumeBar()
{
	if (mpVolumeSlider->isHidden()) {
		mpVolumeSlider->show();
	}
	else {
		mpVolumeSlider->hide();
	}
}

void VedioPlayer::tryShowControlBar()
{
	unsetCursor();
	if (mpTimeSlider && mpTimeSlider->isHidden())
		mpTimeSlider->show();
	if (mpControl && mpControl->isHidden())
		mpControl->show();
}

void VedioPlayer::toggleRepeat(bool r)
{
	mpRepeatAction->defaultWidget()->setEnabled(r); //why need defaultWidget?
	if (r) {
		mRepeateMax = mpRepeatBox->value();
	}
	else {
		mRepeateMax = 0;
	}
	if (mpPlayer) {
		mpPlayer->setRepeat(mRepeateMax);
	}
}

void VedioPlayer::setVolume()
{
	AudioOutput *ao = mpPlayer ? mpPlayer->audio() : 0;
	qreal v = qreal(mpVolumeSlider->value())*kVolumeInterval;
	if (ao) {
		if (qAbs(int(ao->volume() / kVolumeInterval) - mpVolumeSlider->value()) >= int(0.1 / kVolumeInterval)) {
			ao->setVolume(v);
		}
	}
	mpVolumeSlider->setToolTip(QString::number(v));
	mpVolumeBtn->setToolTip(QString::number(v));
}

void VedioPlayer::openFile()
{
	QString file = QFileDialog::getOpenFileName(0, tr("Open a media file"));
	if (file.isEmpty())
		return;
	play(file);

}

void VedioPlayer::onPositionChange(qint64 pos)
{
	if (mpPlayer->isSeekable())
		mpTimeSlider->setValue(pos);
	mpCurrent->setText(QTime(0, 0, 0).addMSecs(pos).toString(QString::fromLatin1("HH:mm:ss")));
}

void VedioPlayer::togglePlayPause()
{
	if (mpPlayer->isPlaying()) {
		qDebug("isPaused = %d", mpPlayer->isPaused());
		mpPlayer->pause(!mpPlayer->isPaused());
	}
	else {
		if (mFile.isEmpty())
			return;
		if (!mpPlayer->isPlaying()){
			play(mFile);
		}
		else{
			//            qDebug()<<"111111111111111111111111111111111111111\n";
			mpPlayer->play();
			mpPlayPauseBtn->setIcon(QIcon(QString::fromLatin1("./image/pause.png")));
		}
	}
}

void VedioPlayer::play(const QString &name)
{
	mFile = name;
	if (!mIsReady) {
		mHasPendingPlay = true;
		return;
	}
	mTitle = mFile;
	if (!mFile.contains(QLatin1String("://")) || mFile.startsWith(QLatin1String("file://"))) {
		mTitle = QFileInfo(mFile).fileName();
	}
	setWindowTitle(mTitle);
	mpPlayer->play(name);
}

void VedioPlayer::onSeekFinished()
{
	qDebug("seek finished at %lld", mpPlayer->position());
}

void VedioPlayer::setRepeateMax(int m)
{
	mRepeateMax = m;
	if (mpPlayer) {
		mpPlayer->setRepeat(m);
	}
}

void VedioPlayer::repeatAChanged(const QTime& t)
{
	if (!mpPlayer)
		return;
	mpPlayer->setStartPosition(QTime(0, 0, 0).msecsTo(t));
}

void VedioPlayer::repeatBChanged(const QTime& t)
{
	if (!mpPlayer)
		return;
	// when this slot is called? even if only range is set?
	if (t <= mpRepeatA->time())
		return;
	mpPlayer->setStopPosition(QTime(0, 0, 0).msecsTo(t));
}
