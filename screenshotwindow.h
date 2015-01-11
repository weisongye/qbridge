#ifndef SCREENSHOTWINDOW_H
#define SCREENSHOTWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QAction>
#include <QtWidgets/QVBoxLayout>
#include <QtGui/QPixmap>
#include <QtCore/QPoint>
#include <QtGui>

#include "fullscreenwidget.h"

class screenShotWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit screenShotWindow(QWidget * parent = 0);
    ~screenShotWindow();

signals:
    void setPixmap(QPixmap pixmap);

public slots:
    void screenshot();

private:
    void createToolBars();
    void createActions();
    QToolBar * mainToolBar;
    QAction * newAct;
    QVBoxLayout * mainLayout;
    QPixmap fullPixmap;
    fullScreenWidget * fullWidget;
};

#endif // SCREENSHOTWINDOW_H
