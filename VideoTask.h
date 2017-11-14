#ifndef VIDEOTASK_H
#define VIDEOTASK_H

#include "Task.h"
#include "Slider.h"
#include <QtAV/AVPlayer.h>
#include <QSlider>
#include <QFileInfo>
using namespace QtAV;

class VideoTask : public Task
{
public:
	VideoTask(QString a, QString b, QFileInfo c, AVPlayer* d) : Task(a, b, c), video(d)
	{
		taskType = a;
		taskName = b;
		taskInfo = c;
		taskProgress = 0;
		isFinished = false;
	}
	~VideoTask()
	{
		delete video;
		delete timeSlider;
		delete volumeSlider;
	}
	AVPlayer::State getState(){ return video ? video->state() : AVPlayer::StoppedState; }
	qint64 size(){ return taskInfo.size(); }
	void start(){ if (getState() != AVPlayer::PlayingState) video->play(); }
	void pause(){ if (getState() == AVPlayer::PlayingState) video->pause(); }
	void stop(){ if (getState() != AVPlayer::StoppedState) video->pause(); }
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