#ifndef VIDEOTASK_H
#define VIDEOTASK_H

#include "Task.h"
#include "Slider.h"
#include <QtAV/AVPlayer.h>
#include <QSlider>
#include <QFileInfo>
#include <QTextStream>
using namespace QtAV;

class VideoTask : public Task
{
public:
	VideoTask(QString a, QString b, QFileInfo c, AVPlayer* d) : Task(a, b, c), video(d)
	{
		taskType = a;
		taskName = b;
		taskInfo = c;
		timeSlider = 0;
		volumeSlider = 0;
		taskProgress = 0;
		isFinished = false;
	}
	~VideoTask()
	{

	}
	AVPlayer::State getState(){ return video ? video->state() : AVPlayer::StoppedState; }
	qint64 size(){ return taskInfo.size(); }
	void start(){ if (getState() != AVPlayer::PlayingState) video->play(); }
	void pause(){ if (getState() == AVPlayer::PlayingState) video->pause(); }
	void stop(){ if (getState() != AVPlayer::StoppedState) video->pause(); }
	void record()
	{
		QFile file("task.log");
		if (!file.open(QIODevice::ReadWrite | QIODevice::Append)) return;
		QTextStream in(&file);
		QString str = QString::fromLocal8Bit("���񴴽�ʱ��: ") + taskTime.toString()
			+ QString::fromLocal8Bit("\n�ļ�����·��: ") + taskInfo.absoluteFilePath()
			+ QString::fromLocal8Bit("\n�ļ���С��") + QString::number(taskInfo.size())
			+ QString::fromLocal8Bit("\n�źţ�") + taskName;
		in << str << "\n";
		file.close();
	}
	void setState()
	{
		if (getState() == AVPlayer::PlayingState)video->pause();
		else video->play();
	}
public:
	AVPlayer* video;
	QSlider* timeSlider;
	QSlider* volumeSlider;
};

#endif