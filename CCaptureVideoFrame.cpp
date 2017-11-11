#include "CCaptureVideoFrame.h"
#include <QCamera>

CCaptureVideoFrame::CCaptureVideoFrame(QObject *parent) : QAbstractVideoSurface(parent)
{
}

QList<QVideoFrame::PixelFormat> CCaptureVideoFrame::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const
{
    Q_UNUSED(handleType);
    QList<QVideoFrame::PixelFormat> lst;

    //lst.push_back(QVideoFrame::Format_YUYV);//Qt���ڲ�֧�ִ˸�ʽ����ΪQt�ڲ�����QImage��������Ƶ֡��
    lst.push_back(QVideoFrame::Format_RGB32);
    lst.push_back(QVideoFrame::Format_BGR32);

    return lst;
}

//������Ƶ֡��windows�¸�ʽ��RGB32��android����NV21
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


