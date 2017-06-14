#ifndef QTAV_AudioWALL_H
#define QTAV_AudioWALL_H

#include <QtCore/QList>
#include <QAudioFormat>
#include <QAudioOutput>
#include "WAVFile.h"
#include "BaseWidget.h"
#include "Task.h"
#include "Slider.h"

QT_BEGIN_NAMESPACE
class QMenu;
class QPushButton;
class QVBoxLayout;
QT_END_NAMESPACE

class AudioWall : public BaseWidget
{
	Q_OBJECT

public:
	explicit AudioWall(QWidget *parent = 0);
	~AudioWall();
	void eventFilter(QMouseEvent *);

public:
	Task tasks[10];
	QList<QPushButton*> addButtons;
	QList<QAudioOutput*> players;
	QList<Slider*> mpTimeSliders;
	QList<Slider*> mpVolumeSliders;
	int SampleRates[10];

	int nth;
	QFont font;
	QPalette pe;
	QPalette font_pe;
	QVBoxLayout *vLayout;
	BaseWidget *AddView;

signals:
	void updateList(Task*);

public slots:
	void addAudioView();
	void selectAudioType(const int &text);
	void openLocalFile();
	void AudioOK();

	void seek();
	void onTimeSliderHover(int pos, int value);
	void onTimeSliderLeave();
	void setVolume();

	void Start();
	void Stop();
	void Remove();
};

#endif // QTAV_AudioWALL_H
