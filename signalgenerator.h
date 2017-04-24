#ifndef MainWindow_H
#define MainWindow_H

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

//���õ������ǰ������
class    Action;
class    QToolBar;
class    QWidget;
class    QRect;
class    QPoint;
class    QMouseEvent;
class    QProgressBar;
class    QListView; 
class    QDoubleSpinBox;


class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();
	QListView *listView;

private:
	Ui::MainWindow ui;

private:
	void createActions();
	void createToolBars();
	void createList();

	QWidget *centralwidget;
	QTabWidget *tabWidget;
	QWidget *tab;
	QWidget *tab_2;
	QProgressBar *progressBar;
	QToolBar *MainToolBar;
	QToolBar *AddToolBar;
	QDoubleSpinBox *doubleSpinBox;


	QAction *list;//�б�
	QAction *add;//���
	QAction *history;//��ʷ��¼
	QAction *lookfor;//����
	QAction *setting;//����
	QAction *about;//����
	QAction *shutdown;//�ر�

	QAction *back;//����
	QAction *audio;//��Ƶ
	QAction *vedio;//��Ƶ
	QAction *start;//��ʼ
	QAction *stop;//ֹͣ
	QAction *remove;//�Ƴ�

	QRect m_areaMovable;//���ƶ����ڵ��������ֻ���ڸ������²����ƶ�����
	bool m_bPressed;//��갴�±�־���������Ҽ���
	QPoint m_ptPress;//��갴�µĳ�ʼλ��

	void setAreaMovable(const QRect rt);
	void mousePressEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);

	private slots:
	void List();
	void Add();
	void History();
	void Lookfor();
	void Setting();
	void About();

	void Back();
	void AddAudioWin();
	void AddVedioWin();
	void Start();
	void Stop();
	void Remove();

};

#endif // MainWindow_H
