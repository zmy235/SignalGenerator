#ifndef QTAV_VIDEOWALL_H
#define QTAV_VIDEOWALL_H

#include <QtCore/QList>
#include <QtAV/AVPlayer.h>
#include <QtAVWidgets/WidgetRenderer.h>
#include "BaseWidget.h"

QT_BEGIN_NAMESPACE
class QMenu;
QT_END_NAMESPACE

class VideoWall : public QObject
{
    Q_OBJECT
public:
    explicit VideoWall(QObject *parent = 0);
    ~VideoWall();
    void setVideoRendererTypeString(const QString& vt);
    void setRows(int n);
    void setCols(int n);
    int rows() const;
	int cols() const;
	void show();
	void setView(BaseWidget *base);
    void play(const QString& file);

private:
    int r, c;
    int timer_id;
    QtAV::AVClock *clock;
    QList<QtAV::AVPlayer*> players;
	BaseWidget *view;
    QMenu *menu;
    QString vid;

protected:
    virtual bool eventFilter(QObject *, QEvent *);
    virtual void timerEvent(QTimerEvent *e);

public slots:
    void stop();
    void openLocalFile();
    void openUrl();
    void about();
    void help();

private Q_SLOTS:
    void changeClockType();
};

#endif // QTAV_VIDEOWALL_H
