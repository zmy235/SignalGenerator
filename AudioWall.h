#ifndef AudioWALL_H
#define AudioWALL_H

#include "BaseWidget.h"
#include "AudioTask.h"
#include "WAVFile.h"

QT_BEGIN_NAMESPACE

class QMenu;
class QPushButton;
class QVBoxLayout;
class QComboBox;
class QToolBox;
class QFile;
class QAudioFormat;
class QAudioOutput;

QT_END_NAMESPACE

class AudioWall : public BaseWidget
{
	Q_OBJECT

public:
	explicit AudioWall(QWidget *parent = 0);
	~AudioWall();
	bool eventFilter(QObject *watched, QEvent *event);

public:
	QList<AudioTask*> tasks;
	QList<QAudioOutput*> players;
	QList<QSlider*> timeSliders;
	QList<QSlider*> volumeSliders;

	int nth;
	QVBoxLayout *layout;
	BaseWidget *AddView;
	QWidget *row;
	QString taskName;
	QString file_path;
	QFileInfo taskInfo;
	QComboBox *sampleRateComboBox;
	QString sampleRate;
	WAVFile *inputFile;
	qint64 fileSize;
	QComboBox *m_deviceBox;
	QAudioDeviceInfo m_device;
	QAudioFormat m_format;

	QFont font;
	QPalette pe;
	QPalette font_pe;

signals:
	void updateAudioList(AudioTask*);
	void updateAudioState(int);
	void updateAudioProgress(int, int);

	public slots:
	void addAudioView();
	void setSampleRate(const int &);
	void deviceChanged(const int &);
	void openLocalFile();
	void AudioOK();
	void Remove();
	void setAudioState(QAudio::State);
	void setState();
	void setSliderPosition();
	void setPlayerPosition(int);
	void setVolume(int);

};

#endif
