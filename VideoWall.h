#ifndef QTAV_VIDEOWALL_H
#define QTAV_VIDEOWALL_H

#include <QtCore/QList>
#include <QtAV/AVPlayer.h>
#include <QtAVWidgets/WidgetRenderer.h>
#include "BaseWidget.h"
#include "Task.h"

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
	void setView(BaseWidget *base);
	void show();
    void play();
	QList<QtAV::AVPlayer*> players;
	QMap<QtAV::AVPlayer*,QString> files;

private:
    int r, c;
	int nth;
	BaseWidget *view; 
	BaseWidget *AddView;
    QMenu *menu;
    QString vid;
	Task tasks[9];
	QVector<QString> vedioTypes;

protected:
    virtual bool eventFilter(QObject *, QEvent *);

signals:
	void updateList(Task*);

public slots:
	void stop();
	void close();
    void openUrl();
	void help();
	void addVideoView();
	void on_sel_vedio(const int &text);
	void openLocalFile();
	void VedioOK();

};

#endif // QTAV_VIDEOWALL_H
