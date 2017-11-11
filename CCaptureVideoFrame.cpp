#include "CCaptureVideoFrame.h"
#include <QCamera>

CCaptureVideoFrame::CCaptureVideoFrame(QObject *parent) : QAbstractVideoSurface(parent)
{
}

QList<QVideoFrame::PixelFormat> CCaptureVideoFrame::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const
{
    Q_UNUSED(handleType);
    QList<QVideoFrame::PixelFormat> lst;

    //lst.push_back(QVideoFrame::Format_YUYV);//Qt现在不支持此格式，因为Qt内部用了QImage来处理视频帧。
    lst.push_back(QVideoFrame::Format_RGB32);
    lst.push_back(QVideoFrame::Format_BGR32);

    return lst;
}

//捕获视频帧。windows下格式是RGB32；android下是NV21
bool CCaptureVideoFrame::present(const QVideoFrame &frame)
{
    qDebug("CCaptureVideoFrame::present format:%d", frame.pixelFormat());
    emit CaptureFrame(frame);

    return true;
}

bool CCaptureVideoFrame::setSource(QMediaPlayer *pPlayer)
{
    bool ret = true;
	pPlayer->setVideoOutput(this);
    return ret;
}


