#ifndef VedioPlayer_H
#define VedioPlayer_H

#include "Slider.h"
#include "BaseWidget.h"

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QTime>
#include <QAction>
#include <QToolTip>
#include <QWidgetAction>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QToolButton>
#include <QSpinBox>
#include <QMessageBox>
#include <QTimeEdit>

#include <QtAVWidgets/QtAVWidgets>
#include <QtAV/QtAV>
#include <QtAV/AVClock.h>
#include <QtAV/AVPlayer.h>

using namespace QtAV;

class VedioPlayer : public BaseWidget
{
    Q_OBJECT

public:
	explicit VedioPlayer(QWidget *parent = 0);
	~VedioPlayer();

private:
    int mRepeateMax;
    int mCursorTimer;
	const qreal kVolumeInterval = 0.04;
	const int kVolumeSliderMax = 100;
    bool mIsReady,mHasPendingPlay;

    VideoOutput *mpVo;
    AVPlayer *mpPlayer;
    AVClock *mpClock;
    VideoRenderer *mpRenderer, *mpTempRenderer;

    QString mFile;
    QString mTitle;
    QTimeEdit *mpRepeatA, *mpRepeatB;

	QVBoxLayout *mainLayout;
	QVBoxLayout *mpPlayerLayout;
	QVBoxLayout *vb;
	QHBoxLayout *hb; 
	QHBoxLayout *controlLayout;

    QAction *mpRepeatEnableAction;
    QWidgetAction *mpRepeatAction;
    QWidget *mpControl;
    QLabel *mpCurrent, *mpEnd;
    QLabel *mpSpeed;
	Slider *mpTimeSlider; 
	Slider *mpVolumeSlider;
    QSpinBox *mpRepeatBox;

    QToolButton *mpVolumeBtn;
    QToolButton *mpPlayPauseBtn;
	QToolButton *mpStopBtn;
	QToolButton *mpForwardBtn;
	QToolButton *mpBackwardBtn;
    QToolButton *mpOpenBtn;

	QLabel *pRepeatLabel;

	VideoPreviewWidget *m_preview;

public slots:
	void play(const QString &name);
	void openFile();
	void togglePlayPause();

private slots:
	void setupUi();
	void initPlayer();
	void setVolume();
	void setRepeateMax(int m);
	void stopUnload();
	void seek();
	void onPaused(bool p);
	void onStartPlay();
	void onStopPlay();
	void onSeekFinished();
	void onTimeSliderHover(int pos, int value);
	void onTimeSliderLeave();
	void handleError(const QtAV::AVError& e);
	void onPositionChange(qint64 pos);
	void showHideVolumeBar();
	void tryShowControlBar();
	void toggleRepeat(bool r);
	void repeatAChanged(const QTime& t);
	void repeatBChanged(const QTime& t);

};

#endif // VedioPlayer_H
