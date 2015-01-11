#ifndef FULLSCREENWIDGET_H
#define FULLSCREENWIDGET_H

#include <QWidget>
#include <QtWidgets/QWidget>
#include <QtWidgets/QMenu>
#include <QtGui/QPixmap>
#include <QtCore/QPoint>
#include <QtGui/QPainter>
#include <QtGui/QBrush>
#include <QtCore/QRect>

class fullScreenWidget : public QWidget
{
    Q_OBJECT

public:
    explicit fullScreenWidget(QWidget * parent = 0);
    ~fullScreenWidget();

    enum shotState
    {
        initShot, beginShot, finishShot, endShot, beginMoveShot,
        finishMoveShot, beginControl, finishControl
    };
    enum controlPointEnum
    {
        moveControl0, moveControl1, moveControl2, moveControl3,
        moveControl4, moveControl5, moveControl6, moveControl7, moveControl8
    };

    QPixmap getFullScreenPixmap();

signals:
    void finishPixmap(const QPixmap & finishPixmap);

public slots:
    void loadBackgroundPixmap(const QPixmap &
                              bgPixmap);
    void loadBackgroundPixmap(const QPixmap & bgPixmap, int x, int y,
                              int width, int height);
    void cancelSelectedRect();
    void savePixmap();

private:
    QRect tlRect;
    QRect trRect;
    QRect blRect;
    QRect brRect;
    QRect tcRect;
    QRect bcRect;
    QRect lcRect;
    QRect rcRect;
    QPainter painter;
    QPoint beginPoint, endPoint, moveBeginPoint, moveEndPoint;
    QRect selectedRect;
    QPixmap loadPixmap, shotPixmap;
    shotState currentShotState;
    controlPointEnum controlValue;
    QAction * savePixmapAction;
    QAction * cancelAction;
    QAction * quitAction;
    QMenu * contextMenu;
    int screenwidth;
    int screenheight;
    int screenx;
    int screeny;
    int tipWidth, tipHeight, infoWidth, infoHeight;
    QRect getSelectedRect();
    QRect getRect(const QPoint & beginPoint, const QPoint & endPoint);
    void initFullScreenWidget();
    bool isInSelectedRect(const QPoint & point);
    void initSelectedMenu();
    void drawTipsText();
    void drawSelectedPixmap(void);
    void updateBeginEndPointValue(const QRect & rect);
    void checkMoveEndPoint();
    void draw8ControlPoint(const QRect & rect);
    void updateMouseShape(const QPoint & point);
    void updateMoveControlMouseShape(controlPointEnum controlValue);
    controlPointEnum getMoveControlState(const QPoint & point);
    QRect getMoveAllSelectedRect(void);
    QRect getMoveControlSelectedRect(void);
    int getMinValue(int num1, int num2);
    void drawXYWHInfo(void);
    void keyPressEvent(QKeyEvent * event);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent * event);
};

#endif // FULLSCREENWIDGET_H
