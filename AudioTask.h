#ifndef AUDIOTASK_H
#define AUDIOTASK_H

#include "Task.h"
#include "WAVFile.h"
#include <QAudioOutput>
#include <QSlider>
#include <QTextStream>

class AudioTask : public Task
{
public:
	AudioTask(QString a, QString b, QFileInfo c, QAudioOutput* d, QString e) : Task(a, b, c), audio(d), sampleRate(e)
	{
		taskType = a;
		taskName = b;
		taskInfo = c;
		taskProgress = 0;
	}
	~AudioTask()
	{
		record();
		delete audio;
		delete file;
		delete timeSlider;
	}
	QAudio::State getState(){ return audio ? audio->state() : QAudio::StoppedState; }
	void start(){ if (getState() != QAudio::ActiveState) audio->start(); }
	void pause() { if (getState() == QAudio::ActiveState) audio->stop(); }
	void stop() { if (getState() != QAudio::StoppedState) audio->blockSignals(true); }
	void record() 
	{
		QFile file("task.log");
		if (!file.open(QIODevice::ReadWrite | QIODevice::Append)) return;
		QTextStream in(&file);
		QString str = QString::fromLocal8Bit("���񴴽�ʱ��: ") + taskTime.toString()
			+QString::fromLocal8Bit("\n�ļ�����·��: ") + taskInfo.absoluteFilePath()
			+ QString::fromLocal8Bit("\n�ļ���С��") + QString::number(taskInfo.size())
			+ QString::fromLocal8Bit("\n�źţ�") + taskName;
		in << str << "\n";
		file.close();
	}
	void setState()
	{
		if (getState() == QAudio::ActiveState)audio->stop();
		else audio->start();
	}

public:

	QString sampleRate;
	qint64 size;
	WAVFile *file;
	QAudioOutput* audio;
	QSlider *timeSlider;
};

#endif