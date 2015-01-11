#include "biddingboxwindow.h"
#include "ui_biddingboxwindow.h"
#include "handconstructordialog.h"
#include "ui_handconstructordialog.h"
#include "screenShotWindow.h"

#include <QStandardItemModel>
#include <QString>
#include <QMessageBox>
#include <QProcess>
#include <QDebug>


BiddingBoxWindow::BiddingBoxWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BiddingBoxWindow)
{
    ui->setupUi(this);

    setItemCount();
    showBiddingOnTable("?");

    //ui->biddingHistoryLable->setText("<font color=red>Hello!</font>");
    QRegExp regExp("[AKQJTakqjt2-9\\.]{16}");
    ui->handLineEdit->setValidator(new QRegExpValidator(regExp, this));

    ui->biddingTableWidget->setFrameShape(QFrame::NoFrame);
    ui->biddingTableWidget->setShowGrid(false);
    ui->biddingTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    //QPalette pal = ui->biddingTableWidget->palette();
    //pal.setBrush(this->backgroundRole(), QBrush(QPixmap("images/background.png")) );
    //ui->biddingTableWidget->setPalette( pal );

    /*
    QPalette pll = musicTable->palette();

    pll.setBrush(QPalette::Base,QBrush(QColor(255,255,255,0)));

    ui->biddingTableWidget->setPalette(pll);

    QTableWidgetItem *cubesHeaderItem = new QTableWidgetItem(tr("Cubes"));
    cubesHeaderItem->setIcon(QIcon(QPixmap("1.png")));
    cubesHeaderItem->setTextAlignment(Qt::AlignVCenter);
     ui->biddingTableWidget->setItem(1,1,cubesHeaderItem);
    */

    ui->biddingTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //ui->biddingTableWidget->verticalHeader()->setVisible(false);
    //ui->biddingTableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");

    //QPalette palette = ui->biddingTableWidget->palette();
    //palette.setBrush(QPalette::Text, QBrush(QColor(255,128,0,0)));



    ui->biddingTableWidget->setAlternatingRowColors(true);
    //ui->biddingTableWidget->setStyleSheet("QTableView{background-color: rgb(250, 250, 115);"
    //    "alternate-background-color: rgb(141, 163, 215);}");

    ui->biddingTableWidget->setWindowTitle(tr("Bidding table"));
    ui->biddingTableWidget->verticalHeader()->setVisible(false);
    ui->biddingTableWidget->horizontalHeader()->setStretchLastSection(true);
}

BiddingBoxWindow::~BiddingBoxWindow()
{
    delete ui;
}

void BiddingBoxWindow::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

int BiddingBoxWindow::getBiddingTableItemRow()
{
    return itemCount / 4;
}

int BiddingBoxWindow::getBiddingTableItemColumn()
{
    return itemCount % 4;
}

void BiddingBoxWindow::showBiddingOnTable(const QString& showStr)
{
    itemRow = getBiddingTableItemRow();
    itemColumn = getBiddingTableItemColumn();

    ui->biddingTableWidget->setItem(itemRow, itemColumn, new QTableWidgetItem(showStr));
    qDebug() << "Draw" << itemRow << ":" << itemColumn << ": " << showStr << endl;
    ui->biddingTableWidget->item(itemRow, itemColumn)->setTextAlignment(Qt::AlignCenter);
}

void BiddingBoxWindow::bid(const QString& bidStr, const QString& showStr)
{
    isBidding = true;

    biddingHistoryVector.push_back(bidStr);

    biddingHistory.clear();

    for (std::vector<QString>::iterator i = biddingHistoryVector.begin();
         i != biddingHistoryVector.end(); i++)
    {
        if (biddingHistory.length() == 0)
            biddingHistory = *i;
        else
            biddingHistory = biddingHistory + " " + *i;
    }

    ui->biddingHistoryLable->setText(biddingHistory);

    showBiddingOnTable(showStr);

    lastBiddingRank = biddingRank;
    lastBiddingSuit = biddingSuit;

    itemCount++;

    biddingRank = "";
    biddingSuit = "";

    QString biddingEndFlag = "P P P";

    if (biddingHistory.right(biddingEndFlag.length()) == biddingEndFlag
            && biddingHistory.length() != biddingEndFlag.length())
    {

        isBidding = false;

        setBidPushButtonsEnabled(false, false, false, false, false);
    }
    else
    {
        showBiddingOnTable("?");
        resetButtonStatus();
    }
}

void BiddingBoxWindow::setBidPushButtonsEnabled(bool pass, bool dbl, bool redbl,
                                                bool rank, bool suit)
{
    ui->bidPassPushButton->setEnabled(pass);
    ui->bidDoublePushButton->setEnabled(dbl);
    ui->bidRedoublePushButton->setEnabled(redbl);
    ui->bidRankPushButton_1->setEnabled(rank);
    ui->bidRankPushButton_2->setEnabled(rank);
    ui->bidRankPushButton_3->setEnabled(rank);
    ui->bidRankPushButton_4->setEnabled(rank);
    ui->bidRankPushButton_5->setEnabled(rank);
    ui->bidRankPushButton_6->setEnabled(rank);
    ui->bidRankPushButton_7->setEnabled(rank);
    ui->bidSuitPushButton_Clubs->setEnabled(suit);
    ui->bidSuitPushButton_Diamonds->setEnabled(suit);
    ui->bidSuitPushButton_Hearts->setEnabled(suit);
    ui->bidSuitPushButton_Spades->setEnabled(suit);
    ui->bidSuitPushButton_NoTrump->setEnabled(suit);
}

void BiddingBoxWindow::setBidRankPushButtonsChecked(bool rank1, bool rank2, bool rank3,
                                                    bool rank4, bool rank5, bool rank6,
                                                    bool rank7)
{
    ui->bidRankPushButton_1->setChecked(rank1);
    ui->bidRankPushButton_2->setChecked(rank2);
    ui->bidRankPushButton_3->setChecked(rank3);
    ui->bidRankPushButton_4->setChecked(rank4);
    ui->bidRankPushButton_5->setChecked(rank5);
    ui->bidRankPushButton_6->setChecked(rank6);
    ui->bidRankPushButton_7->setChecked(rank7);
}

void BiddingBoxWindow::receiveData(QString data)
{
    ui->handLineEdit->setText(data);
}

void BiddingBoxWindow::resetButtonStatus()
{
    setBidPushButtonsEnabled(true, true, true, true, false);
    setBidRankPushButtonsChecked(false, false, false, false, false, false, false);
}

void BiddingBoxWindow::on_bidRankPushButton_clicked(bool checked)
{
    setBidPushButtonsEnabled(true, true, true, true, checked);
    //setBidRankPushButtonsChecked(false, false, false, false, false, false, false);
}

void BiddingBoxWindow::on_bidSuitPushButton_clicked()
{

}

void BiddingBoxWindow::on_bidRankPushButton_1_clicked(bool checked)
{
    setBidRankPushButtonsChecked(checked, false, false, false, false, false, false);
    biddingRank = "1";
}

void BiddingBoxWindow::on_bidRankPushButton_2_clicked(bool checked)
{
    setBidRankPushButtonsChecked(false, checked, false, false, false, false, false);
    biddingRank = "2";
}

void BiddingBoxWindow::on_bidRankPushButton_3_clicked(bool checked)
{
    setBidRankPushButtonsChecked(false, false, checked, false, false, false, false);
    biddingRank = "3";
}

void BiddingBoxWindow::on_bidRankPushButton_4_clicked(bool checked)
{
    setBidRankPushButtonsChecked(false, false, false, checked, false, false, false);
    biddingRank = "4";
}

void BiddingBoxWindow::on_bidRankPushButton_5_clicked(bool checked)
{
    setBidRankPushButtonsChecked(false, false, false, false, checked, false, false);
    biddingRank = "5";
}

void BiddingBoxWindow::on_bidRankPushButton_6_clicked(bool checked)
{
    setBidRankPushButtonsChecked(false, false, false, false, false, checked, false);
    biddingRank = "6";
}

void BiddingBoxWindow::on_bidRankPushButton_7_clicked(bool checked)
{
    setBidRankPushButtonsChecked(false, false, false, false, false, false, checked);
    biddingRank = "7";
}


void BiddingBoxWindow::on_bidPassPushButton_clicked()
{
    bid("P", ui->bidPassPushButton->text());
}

void BiddingBoxWindow::on_bidDoublePushButton_clicked()
{
    bid("X", "X");
}

void BiddingBoxWindow::on_bidRedoublePushButton_clicked()
{
    bid("XX", "XX");
}

void BiddingBoxWindow::on_bidSuitPushButton_Clubs_clicked()
{
    if (biddingRank == "")
    {
        return;
    }

    biddingSuit = "C";
    bid(biddingRank + biddingSuit, biddingRank + ui->bidSuitPushButton_Clubs->text());

    on_bidSuitPushButton_clicked();
}

void BiddingBoxWindow::on_bidSuitPushButton_Diamonds_clicked()
{
    if (biddingRank == "")
    {
        return;
    }

    biddingSuit = "D";
    bid(biddingRank + biddingSuit, biddingRank + ui->bidSuitPushButton_Diamonds->text());

    on_bidSuitPushButton_clicked();
}

void BiddingBoxWindow::on_bidSuitPushButton_Hearts_clicked()
{
    if (biddingRank == "")
    {
        return;
    }

    biddingSuit = "H";
    bid(biddingRank + biddingSuit, biddingRank + ui->bidSuitPushButton_Hearts->text());

    on_bidSuitPushButton_clicked();
}

void BiddingBoxWindow::on_bidSuitPushButton_Spades_clicked()
{
    if (biddingRank == "")
    {
        return;
    }

    biddingSuit = "S";
    bid(biddingRank + biddingSuit, biddingRank + ui->bidSuitPushButton_Spades->text());

    on_bidSuitPushButton_clicked();
}

void BiddingBoxWindow::on_bidSuitPushButton_NoTrump_clicked()
{
    if (biddingRank == "")
    {
        return;
    }

    biddingSuit = "N";
    bid(biddingRank + biddingSuit, biddingRank + ui->bidSuitPushButton_NoTrump->text());

    on_bidSuitPushButton_clicked();
}

void BiddingBoxWindow::on_bidNewPushButton_clicked()
{
    if (ui->newGameConfirmCheckBox->checkState() == Qt::Unchecked)
    {
        setBidPushButtonsEnabled(true, false, false, true, false);
        ui->biddingTableWidget->clearContents();
        biddingHistoryVector.clear();
        biddingHistory.clear();
        setItemCount();
        showBiddingOnTable("?");
        resetButtonStatus();

        return;
    }
    else
    {
        QMessageBox message(QMessageBox::Warning, "Information", tr("New game?"),
            QMessageBox::Yes | QMessageBox::No, NULL);

        if (message.exec() == QMessageBox::Yes)
        {
            setBidPushButtonsEnabled(true, false, false, true, false);
            ui->biddingTableWidget->clearContents();
            biddingHistoryVector.clear();
            biddingHistory.clear();
            setItemCount();
            showBiddingOnTable("?");
            resetButtonStatus();
            //ui->handLineEdit->clear();
        }
        else
        {
            // Do nothing
        }
    }
}

int BiddingBoxWindow::getBaseItemCount()
{
    if (ui->biddingStartRadioButton_West->isChecked() == true)
    {
        return 0;
    }
    else if (ui->biddingStartRadioButton_North->isChecked() == true)
    {
        return 1;
    }
    else if (ui->biddingStartRadioButton_East->isChecked() == true)
    {
        return 2;
    }
    else if (ui->biddingStartRadioButton_South->isChecked() == true)
    {
        return 3;
    }

    return 0;
}

void BiddingBoxWindow::setItemCount()
{
    itemCount = getBaseItemCount();
}

void BiddingBoxWindow::on_biddingStartRadioButton_East_clicked()
{
    setItemCount();
    ui->bidNewPushButton->click();
}

void BiddingBoxWindow::on_biddingStartRadioButton_West_clicked()
{
    setItemCount();
    ui->bidNewPushButton->click();
}

void BiddingBoxWindow::on_biddingStartRadioButton_North_clicked()
{
    setItemCount();
    ui->bidNewPushButton->click();
}

void BiddingBoxWindow::on_biddingStartRadioButton_South_clicked()
{
    setItemCount();
    ui->bidNewPushButton->click();
}

QString BiddingBoxWindow::calcBiddingResult()
{
    QProcess *proc = new QProcess;

    QStringList hand;
    hand << ui->handLineEdit->text();
    hand << biddingHistory;

    proc->start("sample.exe", hand);

    if (!proc->waitForStarted())
    {
        QMessageBox message(QMessageBox::Warning, "Information",
            tr("Process start failed."), QMessageBox::Yes | QMessageBox::No, NULL);
        if (message.exec() == QMessageBox::Yes)
        {
            qDebug() << "clicked yes\n";
        }
        else
        {
            qDebug() << "clicked no\n";
        }

        return "";
    }

    proc->closeWriteChannel();

    QByteArray procOutput;

    while (proc->waitForFinished() == false)
    {
        ;
    }

    procOutput = proc->readAll();

    qDebug() << procOutput.data() << endl;
    QString ret(procOutput.data());

    return ret;
}

void BiddingBoxWindow::on_bidHintPushButton_clicked()
{
    ui->hintLabel->setText(calcBiddingResult().left(2));
    QMessageBox::about(NULL, "Hint", calcBiddingResult());
}

void BiddingBoxWindow::on_handLineEdit_textChanged(const QString &arg1)
{
    ui->handLineEdit->setText(arg1.toUpper());
}

void BiddingBoxWindow::on_constructHandPushButton_clicked()
{
    handConstructorDialog *dialog = new handConstructorDialog();
    connect(dialog, SIGNAL(sendData(QString)), this, SLOT(receiveData(QString)));
    dialog->show();
}

void BiddingBoxWindow::on_screenShotPushButton_clicked()
{
    screenShotWindow *screenMain = new screenShotWindow;
    screenMain->screenshot();
}

void BiddingBoxWindow::on_getOCRPushButton_clicked()
{
    QProcess *proc = new QProcess;

    QStringList args;
    args << "screenshot.bmp";

    proc->start("pcr", args);

    if (!proc->waitForStarted())
    {
        QMessageBox message(QMessageBox::Warning, "Information",
            tr("Process start failed."), QMessageBox::Yes | QMessageBox::No, NULL);
        if (message.exec() == QMessageBox::Yes)
        {
            qDebug() << "clicked yes\n";
        }
        else
        {
            qDebug() << "clicked no\n";
        }

        return;
    }

    proc->closeWriteChannel();

    QByteArray procOutput;

    while (proc->waitForFinished() == false)
    {
        ;
    }

    procOutput = proc->readAll();

    qDebug() << procOutput.data() << endl;


    QFile f("cards.txt");
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Open failed." << endl;
        ui->handLineEdit->clear();
        return;
    }

    QTextStream txtInput(&f);
    QString lineStr =  txtInput.readLine();
    ui->handLineEdit->setText(lineStr);

    f.close();

    return;
}

void BiddingBoxWindow::on_bidUndoPushButton_clicked()
{
    if (itemCount <= getBaseItemCount())
    {
        return;
    }

    showBiddingOnTable("");
    itemCount--;
    showBiddingOnTable("?");

    biddingHistoryVector.pop_back();

    biddingHistory.clear();

    for (std::vector<QString>::iterator i = biddingHistoryVector.begin();
         i != biddingHistoryVector.end(); i++)
    {
        if (biddingHistory.length() == 0)
            biddingHistory = *i;
        else
            biddingHistory = biddingHistory + " " + *i;
    }

    ui->biddingHistoryLable->setText(biddingHistory);

    resetButtonStatus();
}
