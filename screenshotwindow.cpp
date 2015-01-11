#include "screenshotwindow.h"

#include <QDebug>

screenShotWindow::screenShotWindow(QWidget * parent) : QMainWindow(parent)
{
    createActions();
    createToolBars();
    fullWidget = new fullScreenWidget();

    connect(newAct, SIGNAL(triggered()), this, SLOT(screenshot()));
    connect(this, SIGNAL(setPixmap(QPixmap)), fullWidget,
            SLOT(loadBackgroundPixmap(QPixmap)));

    this->resize(500, 300);
}

screenShotWindow::~screenShotWindow()
{
}

void screenShotWindow::createToolBars()
{
    mainToolBar = new QToolBar("mainToolBar", this);
    mainToolBar->addAction(newAct);
    this->addToolBar(mainToolBar);
}

void screenShotWindow::createActions()
{
    newAct = new QAction(tr("screenshoot"), this);
}

void screenShotWindow::screenshot()
{
    QPixmap pixmap = fullWidget->getFullScreenPixmap();
    fullWidget->show();
    emit setPixmap(pixmap);
}
