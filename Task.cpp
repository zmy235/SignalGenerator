#include "Task.h"

Task::Task()
{

}

Task::~Task()
{

}

void Task::start()
{
	if (type == "audio")
		audio->start();
	else if (!video->isPlaying())
		video->play();

	playing = true;
}

void Task::pause()
{
	if (type == "audio")
		audio->stop();
	else
		video->pause(!video->isPaused());

	playing = false;
}