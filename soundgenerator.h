#ifndef SOUNDGENERATOR_H
#define SOUNDGENERATOR_H

#include <QtWidgets/QMainWindow>
#include <QFile>
#include <QAudioOutput>
#include <QComboBox>

#include "ui_soundgenerator.h"

class SoundGenerator : public QMainWindow
{
	Q_OBJECT

public:
	SoundGenerator(QWidget *parent = 0);
	~SoundGenerator();
	void createAudioOutput();

	QComboBox *m_deviceBox;
	QIODevice *m_output; // not owned
	QAudioDeviceInfo m_device;
	QAudioOutput *m_audioOutput;
	QAudioFormat m_format;

	private slots:
	void deviceChanged(int index);

private:
	Ui::SoundGeneratorClass ui;
};

#endif // SOUNDGENERATOR_H
