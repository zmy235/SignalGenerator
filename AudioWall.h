#ifndef AudioWALL_H
#define AudioWALL_H

#include <QtCore/QList>
#include <QFile>
#include <QAudioFormat>
#include <QAudioOutput>
#include <QComboBox>

#include "BaseWidget.h"
#include "Slider.h"
#include "AudioTask.h"
//#include "WAVFile.h"

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
	void createAudioOutput();

public:
	int nth;
	QList<QPushButton*> addButtons;
	QVBoxLayout *vLayout;
	BaseWidget *AddView;
	QList<AudioTask*> tasks;
	QString taskName;
	QString file_path;
	QFileInfo taskInfo;

	QComboBox *sampleRateComboBox;
	QString sampleRate;

	QComboBox *m_deviceBox;
	QAudioDeviceInfo m_device;
	QIODevice *m_output;
	QAudioOutput *m_audioOutput;
	QAudioFormat m_format;

	Slider* TimeSlider;
	Slider* VolumeSlider;

	QFont font;
	QPalette pe;
	QPalette font_pe;

signals:
	void updateAudioList(AudioTask*);
	void updateAudioState(int);

public slots:
	void addAudioView();
	void setSampleRate(const int &index);
	void deviceChanged(const int &index);
	void openLocalFile();
	void AudioOK();

	void seek();
	void onTimeSliderHover(int pos, int value);
	void onTimeSliderLeave();
	void setVolume();

	void SetState();
	void Remove();
};

#endif
