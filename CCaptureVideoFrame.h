/*
 * ��Player�в�����Ƶ֡��
 */

#ifndef CAPTUREVIDEOFRAME_H
#define CAPTUREVIDEOFRAME_H

#include <QAbstractVideoSurface>
#include <QVideoProbe>
#include <QMediaPlayer>

class CCaptureVideoFrame : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    explicit CCaptureVideoFrame(QObject *parent = 0);
    virtual QList<QVideoFrame::PixelFormat> supportedPixelFormats( QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const;
	bool setSource(QMediaPlayer *pPlayer);

signals:
    void CaptureFrame(const QVideoFrame &frame);//��Ƶ֡�����ź�

private slots:
    virtual bool present(const QVideoFrame &frame);

private:
     QVideoProbe m_Probe;//��probe������Ƶ

};

#endif // CAPTUREVIDEOFRAME_H


