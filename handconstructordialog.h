#ifndef HANDCONSTRUCTORDIALOG_H
#define HANDCONSTRUCTORDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class handConstructorDialog;
}

class handConstructorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit handConstructorDialog(QWidget *parent = 0);
    ~handConstructorDialog();

protected:
    void changeEvent(QEvent *e);

signals:
    void sendData(QString);

private slots:
    void on_buttonBox_accepted();

    void on_screenshotPushButton_clicked();

private:
    Ui::handConstructorDialog *ui;
};

#endif // HANDCONSTRUCTORDIALOG_H
