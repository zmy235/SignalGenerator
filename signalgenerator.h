#ifndef MainWindow_H
#define MainWindow_H

#include <QtWidgets/QMainWindow>
#include "ui_signalgenerator.h"

//所用到的类的前置声明
class    Action;
class    QToolBar;
class    QWidget;
class    BaseWidget;
class    QRect;
class    QPoint;
class    QMouseEvent;
class    QProgressBar;
class    QListView;
class    QPushButton;
class    QLabel;
class    QDateTime;
class    Task;
class    SetView;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

public:
	static QList<Task*>* taskList;//全局

private:
	Ui::SignalGeneratorClass ui;

private:
	void createActions();
	void createToolBars();

	SetView *setView;
	bool toolBarView;//全局

	QFont font;
	QPalette font_pe;
	QPalette palette; 

	QToolBar *MainToolBar;

	QAction *refresh;//列表
	QAction *add;//添加
	QAction *history;//历史记录
	QAction *lookfor;//查找
	QAction *setting;//设置
	QAction *about;//关于
	QAction *shutdown;//关闭

	QAction *back;//返回
	QAction *audio;//音频
	QAction *vedio;//视频

	QList<Task*>::iterator TaskListHead;
	int TaskListSize;
	QList<QWidget*>* WigetList;
	QWidget *Base;
	QWidget *dockContents;
	QLabel *taskName;
	int ListNum;
	QMouseEvent *eNote;
	QProgressBar *progressBar;
	QPushButton *remove;//移除
	QPushButton *start;//开始
	QPushButton *stop;//停止
	QPushButton *info;//详情

	QRect m_areaMovable;//可移动窗口的区域，鼠标只有在该区域按下才能移动窗口
	bool m_bPressed;//鼠标按下标志（不分左右键）
	QPoint m_ptPress;//鼠标按下的初始位置

	void setAreaMovable(const QRect rt);
	void mousePressEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);

	private slots:

	void updateVH(bool);
	void updateOpacity(int);
	void createList();
	void Add();
	void History();
	void Lookfor();
	void Setting();
	void About();

	void Start();
	void Stop();
	void Remove();
	void Info();

};

#endif // MainWindow_H
