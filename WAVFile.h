
#ifndef WAVFILE_H
#define WAVFILE_H

#include "sndfile.h"
#include <QIODevice>
#include <QAudioFormat>


class WAVFile : public QIODevice
{
public:
	WAVFile();
	~WAVFile();

	bool isSequential() const { return false; }
	bool open(QString fileName, OpenMode mode, QAudioFormat format = QAudioFormat());
	void close();
	bool canReadLine() const { return false; }
	qint64 pos() const;
	qint64 size() const;
	bool seek(qint64 pos);
	bool atEnd() const;
	bool reset();
	QAudioFormat format();

protected:
	qint64 readData(char * data, qint64 maxSize);
	qint64 writeData(const char * data, qint64 maxSize);

private:
	SNDFILE * m_sndfile;
	SF_INFO m_info;
	QString m_fileName;
};

#endif // WAVFILE_H