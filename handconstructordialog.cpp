#include "handconstructordialog.h"
#include "ui_handconstructordialog.h"

#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QScreen>

#include "screenShotWindow.h"

handConstructorDialog::handConstructorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::handConstructorDialog)
{
    ui->setupUi(this);

    ui->handTableWidget->setAlternatingRowColors(true);

    for (int col = 0; col < ui->handTableWidget->columnCount(); col++)
    {
        ui->handTableWidget->setColumnWidth(col, 40);
    }

    for (int col = 0; col < ui->handTableWidget->columnCount(); col++)
    {
        for (int row = 0; row < ui->handTableWidget->rowCount(); row++)
        {
            //ui->handTableWidget->item(row, col)->setTextAlignment(Qt::AlignLeft);
        }
    }
}

handConstructorDialog::~handConstructorDialog()
{
    delete ui;
}

void handConstructorDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void handConstructorDialog::on_buttonBox_accepted()
{
    int checkedSum = 0;

    QString suit_hand = "";

    for (int row = ui->handTableWidget->rowCount() - 1; row >=0; row--)
    {
        for (int col =  0; col < ui->handTableWidget->columnCount(); col++)
        {
            if (ui->handTableWidget->item(row, col)->checkState() == Qt::Checked)
            {
                checkedSum++;
                if (ui->handTableWidget->item(row, col)->text() != "10")
                {
                    suit_hand = suit_hand + ui->handTableWidget->item(row, col)->text();
                }
                else
                {
                    suit_hand = suit_hand + "T";
                }
            }
        }
        suit_hand = suit_hand + ".";
    }

    suit_hand = suit_hand.left(suit_hand.length() - 1);

    qDebug() << "suit_hand: " << suit_hand << endl;

    if (checkedSum != 13)
    {
        switch( QMessageBox::warning(NULL, "warning", QString::number(checkedSum, 10) + tr(" is not 13, not enough, retry?"),
                                     QMessageBox::Yes | QMessageBox::Default,
                                     QMessageBox::No | QMessageBox::Escape ))
        {
        case QMessageBox::Yes:
            this->show();
            break;
        case QMessageBox::No:
            return;
        }
    }

    emit sendData(suit_hand);
}

void handConstructorDialog::on_screenshotPushButton_clicked()
{
    //QScreen *screen = QGuiApplication::primaryScreen();
    //screen->grabWindow(0).save("123.bmp","bmp");

    screenShotWindow *screenMain = new screenShotWindow;
    screenMain->show();
}
