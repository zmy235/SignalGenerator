#include "WAVFile.h"

WAVFile::WAVFile()
{

}

WAVFile::~WAVFile()
{

}

qint64 WAVFile::pos() const
{
	sf_count_t p = sf_seek(m_sndfile, 0, SEEK_CUR);
	return p;
}

qint64 WAVFile::size() const
{
	sf_count_t p = sf_seek(m_sndfile, 0, SEEK_END);
	return p;
}

bool WAVFile::seek(qint64 pos)
{
	sf_seek(m_sndfile, pos, SEEK_SET);
	return true;
}

bool WAVFile::atEnd() const
{
	if (pos() == size())
	{
		return true;
	}
	return false;
}

bool WAVFile::reset()
{
	sf_count_t p = sf_seek(m_sndfile, 0, SEEK_SET);
	if (p == -1)
	{
		return false;
	}
	return true;
}

bool WAVFile::open(QString fileName, OpenMode mode, QAudioFormat format)
{
	int snd_mode;
	switch (mode)
	{
	case QIODevice::ReadOnly:
		snd_mode = SFM_READ;
		m_info.channels = 0;
		m_info.format = 0;
		m_info.frames = 0;
		m_info.samplerate = 0;
		break;
	case QIODevice::WriteOnly:
		snd_mode = SFM_WRITE;
		m_info.channels = format.channelCount();
		m_info.format = SF_FORMAT_WAV;
		m_info.frames = 0;
		m_info.samplerate = format.sampleRate();
		break;
	case QIODevice::ReadWrite:
		snd_mode = SFM_RDWR;
		break;
	default:
		return false;
	}
	m_sndfile = sf_open(fileName.toLocal8Bit(), snd_mode, &m_info);
	QIODevice::open(mode);
	return m_sndfile;
}

qint64 WAVFile::readData(char * data, qint64 maxSize)
{
	return sf_read_raw(m_sndfile, data, maxSize);
}

qint64 WAVFile::writeData(const char * data, qint64 maxSize)
{
	return sf_write_raw(m_sndfile, data, maxSize);
}

QAudioFormat WAVFile::format()
{
	QAudioFormat audioFormat;
	audioFormat.setCodec("audio/pcm");
	audioFormat.setByteOrder(QAudioFormat::LittleEndian);
	audioFormat.setSampleRate(m_info.samplerate);
	audioFormat.setChannelCount(m_info.channels);
	switch (m_info.format & 0x0f)
	{
	case SF_FORMAT_PCM_U8:
		audioFormat.setSampleSize(8);
		audioFormat.setSampleType(QAudioFormat::UnSignedInt);
		break;
	case SF_FORMAT_PCM_S8:
		audioFormat.setSampleSize(8);
		audioFormat.setSampleType(QAudioFormat::SignedInt);
		break;
	case SF_FORMAT_PCM_16:
		audioFormat.setSampleSize(16);
		audioFormat.setSampleType(QAudioFormat::SignedInt);
		break;
	case SF_FORMAT_PCM_24:
		audioFormat.setSampleSize(24);
		audioFormat.setSampleType(QAudioFormat::SignedInt);
		break;
	case SF_FORMAT_PCM_32:
		audioFormat.setSampleSize(32);
		audioFormat.setSampleType(QAudioFormat::SignedInt);
		break;
	default:
		audioFormat.setSampleSize(8);
		audioFormat.setSampleType(QAudioFormat::UnSignedInt);
		break;
	}
	return audioFormat;
}

void WAVFile::close()
{
	sf_close(m_sndfile);
}