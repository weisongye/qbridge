#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class BiddingBoxWindow;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private slots:
    void on_showBiddingBoxPushButton_clicked();

private:
    Ui::MainWindow *ui;
    BiddingBoxWindow *biddingBoxWindow;
};

#endif // MAINWINDOW_H
