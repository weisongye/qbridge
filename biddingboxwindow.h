#ifndef BIDDINGBOXWINDOW_H
#define BIDDINGBOXWINDOW_H

#include <QWidget>
#include <QString>
#include <vector>

namespace Ui {
class BiddingBoxWindow;
}

class BiddingBoxWindow : public QWidget
{
    Q_OBJECT

public:
    explicit BiddingBoxWindow(QWidget *parent = 0);
    ~BiddingBoxWindow();

protected:
    void changeEvent(QEvent *e);

private slots:
    void receiveData(QString data);
    void on_bidRankPushButton_clicked(bool checked);
    void on_bidSuitPushButton_clicked();

    void on_bidRankPushButton_1_clicked(bool checked);
    void on_bidRankPushButton_2_clicked(bool checked);
    void on_bidRankPushButton_3_clicked(bool checked);
    void on_bidRankPushButton_4_clicked(bool checked);
    void on_bidRankPushButton_5_clicked(bool checked);
    void on_bidRankPushButton_6_clicked(bool checked);
    void on_bidRankPushButton_7_clicked(bool checked);

    void on_bidPassPushButton_clicked();

    void on_bidDoublePushButton_clicked();

    void on_bidRedoublePushButton_clicked();

    void on_bidSuitPushButton_Clubs_clicked();

    void on_bidSuitPushButton_Diamonds_clicked();

    void on_bidSuitPushButton_Hearts_clicked();

    void on_bidSuitPushButton_Spades_clicked();

    void on_bidSuitPushButton_NoTrump_clicked();

    void on_bidNewPushButton_clicked();

    void on_biddingStartRadioButton_East_clicked();

    void on_biddingStartRadioButton_West_clicked();

    void on_biddingStartRadioButton_North_clicked();

    void on_biddingStartRadioButton_South_clicked();

    void on_bidHintPushButton_clicked();

    void on_handLineEdit_textChanged(const QString &arg1);

    void on_constructHandPushButton_clicked();

    void on_screenShotPushButton_clicked();

    void on_getOCRPushButton_clicked();

    void on_bidUndoPushButton_clicked();

private:
    Ui::BiddingBoxWindow *ui;

    std::vector<QString> biddingHistoryVector;
    QString biddingHistory = "";
    QString biddingRank = "";
    QString biddingSuit = "";

    QString lastBiddingRank = "";
    QString lastBiddingSuit = "";

    //QString last_bidding = "";

    int itemCount = 0;
    int itemRow = 0, itemColumn = 0;
    bool isBidding = false;

    int getBiddingTableItemRow();
    int getBiddingTableItemColumn();

    void showBiddingOnTable(const QString& showStr);

    void bid(const QString& bidStr, const QString& showStr);

    void resetButtonStatus();

    void setBidPushButtonsEnabled(bool pass, bool dbl, bool redbl,
                                  bool rank, bool suit);
    void setBidRankPushButtonsChecked(bool rank1, bool rank2, bool rank3,
                                      bool rank4, bool rank5, bool rank6,
                                      bool rank7);

    int getBaseItemCount();
    void setItemCount();

    QString calcBiddingResult();
};

#endif // BIDDINGBOXWINDOW_H
