#ifndef MainWindow_H
#define MainWindow_H

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

//���õ������ǰ������
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
class    AddView;
class    SetView;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

public:
	static QList<Task*>* taskList;//ȫ��

private:
	Ui::MainWindow ui;

private:
	void createActions();
	void createToolBars();

	AddView *addView;
	SetView *setView;
	bool toolBarView;//ȫ��

	QFont font;
	QPalette font_pe;
	QPalette palette; 

	QToolBar *MainToolBar;

	QAction *refresh;//�б�
	QAction *add;//���
	QAction *history;//��ʷ��¼
	QAction *lookfor;//����
	QAction *setting;//����
	QAction *about;//����
	QAction *shutdown;//�ر�

	QAction *back;//����
	QAction *audio;//��Ƶ
	QAction *vedio;//��Ƶ

	QList<Task*>::iterator TaskListHead;
	int TaskListSize;
	QList<QWidget*>* WigetList;
	QWidget *Base;
	QWidget *dockContents;
	QLabel *taskName;
	int ListNum;
	QMouseEvent *eNote;
	QProgressBar *progressBar;
	QPushButton *remove;//�Ƴ�
	QPushButton *start;//��ʼ
	QPushButton *stop;//ֹͣ
	QPushButton *info;//����

	QRect m_areaMovable;//���ƶ����ڵ��������ֻ���ڸ������²����ƶ�����
	bool m_bPressed;//��갴�±�־���������Ҽ���
	QPoint m_ptPress;//��갴�µĳ�ʼλ��

	void setAreaMovable(const QRect rt);
	void mousePressEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);

	private slots:

	void updateVH(bool);
	void updateOpacity(int);
	void updateList(Task*);

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
