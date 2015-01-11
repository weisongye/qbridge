#include <QString>
#include <QtGui/QtEvents>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QDesktopWidget>

#include "fullscreenwidget.h"

#include <QDebug>

fullScreenWidget::fullScreenWidget(QWidget * parent) : QWidget(parent)
{
    setWindowState(Qt::WindowActive | Qt::WindowFullScreen);
    tipWidth = 300;
    tipHeight = 100;
    infoWidth = 100;
    infoHeight = 50;
    initFullScreenWidget();

    connect(this, SIGNAL(finishPixmap(const QPixmap &)), this, SLOT(savePixmap()));
}

fullScreenWidget::~fullScreenWidget()
{
}

void fullScreenWidget::initSelectedMenu()
{
    savePixmapAction = new QAction(tr("Save selection"), this);
    cancelAction = new QAction(tr("Reselect"), this);
    quitAction = new QAction(tr("Exit"), this);
    contextMenu = new QMenu(this);
    connect(savePixmapAction, SIGNAL(triggered()), this,
            SLOT(savePixmap()));
    connect(cancelAction, SIGNAL(triggered()), this,
            SLOT(cancelSelectedRect()));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(hide()));
}

void fullScreenWidget::savePixmap()
{
    QString fileName = "screenshot.bmp";
#if 0
    fileName =
            QFileDialog::getSaveFileName(this, tr("Save image"),
                                         QDir::currentPath(),
                                         tr("Images (*.jpg *.png *.bmp)"));
#endif
    qDebug() <<" ###### " << QDir::currentPath()<<endl;
    if (fileName.isNull())
        return;
    shotPixmap.save(fileName);
    hide();
}

void fullScreenWidget::loadBackgroundPixmap(const QPixmap & bgPixmap)
{
    int width, height;
    width = QApplication::desktop()->size().width();
    height = QApplication::desktop()->size().height();
    loadBackgroundPixmap(bgPixmap, 0, 0, width, height);
}

void fullScreenWidget::loadBackgroundPixmap(const QPixmap & bgPixmap,
                                            int x, int y, int width,
                                            int height)
{
    loadPixmap = bgPixmap;
    screenx = x;
    screeny = y;
    screenwidth = width;
    screenheight = height;
    initFullScreenWidget();
}

QPixmap fullScreenWidget::getFullScreenPixmap()
{
    initFullScreenWidget();
    QPixmap result = QPixmap();
    result = QPixmap::grabWindow(QApplication::desktop()->winId());
    return result;
}

void fullScreenWidget::paintEvent(QPaintEvent *event)
{
    QColor shadowColor;
    shadowColor = QColor(0, 0, 0, 100);
    painter.begin(this);
    painter.setPen(QPen(Qt::blue, 2, Qt::SolidLine, Qt::FlatCap));
    painter.drawPixmap(screenx, screeny, loadPixmap);
    painter.fillRect(screenx, screeny, screenwidth, screenheight,
                     shadowColor);
    switch (currentShotState) {
    case initShot:
        drawTipsText();
        break;
    case beginShot:
    case finishShot:
        selectedRect = getRect(beginPoint, endPoint);
        drawSelectedPixmap();
        break;
    case beginMoveShot:
    case finishMoveShot:
        selectedRect = getMoveAllSelectedRect();
        drawSelectedPixmap();
        break;
    case beginControl:
    case finishControl:
        selectedRect = getMoveControlSelectedRect();
        drawSelectedPixmap();
        break;
    default:
        break;
    }
    drawXYWHInfo();
    painter.end();
    if (currentShotState == finishMoveShot
            || currentShotState == finishControl) {
        updateBeginEndPointValue(selectedRect);
    }
}

void fullScreenWidget::keyPressEvent(QKeyEvent * event)
{
    if (event->key() == Qt::Key_Escape) {
        initFullScreenWidget();
        hide();
    }
}

void fullScreenWidget::mousePressEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton && currentShotState == initShot) {
        currentShotState = beginShot;
        beginPoint = event->pos();
    }
    if (event->button() == Qt::LeftButton
            && isInSelectedRect(event->pos())
            && getMoveControlState(event->pos()) == moveControl0) {
        currentShotState = beginMoveShot;
        moveBeginPoint = event->pos();
    }
    if (event->button() == Qt::LeftButton
            && getMoveControlState(event->pos()) != moveControl0) {
        currentShotState = beginControl;
        controlValue = getMoveControlState(event->pos());
        moveBeginPoint = event->pos();
    }
}

void fullScreenWidget::mouseReleaseEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton && currentShotState == beginShot) {
        currentShotState = finishShot;
        endPoint = event->pos();
        update();
    }
    if (event->button() == Qt::LeftButton
            && currentShotState == beginMoveShot) {
        currentShotState = finishMoveShot;
        moveEndPoint = event->pos();
        update();
    }
    if (event->button() == Qt::LeftButton
            && currentShotState == beginControl) {
        currentShotState = finishControl;
        moveEndPoint = event->pos();
        update();
    }
}

void fullScreenWidget::mouseMoveEvent(QMouseEvent * event)
{
    if (currentShotState == beginShot) {
        endPoint = event->pos();
        update();
    }
    if (currentShotState == beginMoveShot
            || currentShotState == beginControl) {
        moveEndPoint = event->pos();
        update();
    }
    updateMouseShape(event->pos());
    setMouseTracking(true);
}

void fullScreenWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (currentShotState == finishShot
            || currentShotState == finishMoveShot
            || currentShotState == finishControl) {
        emit finishPixmap(shotPixmap);
        hide();
    }
}

QRect fullScreenWidget::getRect(const QPoint & beginPoint,
                                const QPoint & endPoint)
{
    int x, y, width, height;
    width = qAbs(beginPoint.x() - endPoint.x());
    height = qAbs(beginPoint.y() - endPoint.y());
    x = beginPoint.x() < endPoint.x()? beginPoint.x() : endPoint.x();
    y = beginPoint.y() < endPoint.y()? beginPoint.y() : endPoint.y();
    return QRect(x, y, width, height);
}

void fullScreenWidget::initFullScreenWidget()
{
    currentShotState = initShot;
    controlValue = moveControl0;
    beginPoint = QPoint(0, 0);
    endPoint = QPoint(0, 0);
    moveBeginPoint = QPoint(0, 0);
    moveEndPoint = QPoint(0, 0);
    tlRect = QRect(0, 0, 0, 0);
    trRect = QRect(0, 0, 0, 0);
    blRect = QRect(0, 0, 0, 0);
    brRect = QRect(0, 0, 0, 0);
    tcRect = QRect(0, 0, 0, 0);
    bcRect = QRect(0, 0, 0, 0);
    lcRect = QRect(0, 0, 0, 0);
    rcRect = QRect(0, 0, 0, 0);
    setCursor(Qt::CrossCursor);
}

bool fullScreenWidget::isInSelectedRect(const QPoint & point)
{
    int x, y;
    QRect selectedRect;
    if (currentShotState == initShot || currentShotState == beginShot)
        return false;
    selectedRect = getSelectedRect();
    x = point.x();
    y = point.y();
    return selectedRect.contains(x, y);
}

void fullScreenWidget::cancelSelectedRect()
{
    initFullScreenWidget();
    update();
}

void fullScreenWidget::contextMenuEvent(QContextMenuEvent * event)
{
    initSelectedMenu();
    if (isInSelectedRect(event->pos())) {
        contextMenu->addAction(savePixmapAction);
    }

    else {
        contextMenu->addAction(cancelAction);
        contextMenu->addAction(quitAction);
    }
    contextMenu->exec(event->pos());
}

void fullScreenWidget::drawTipsText()
{
    int x = (screenwidth - tipWidth) / 2;
    int y = (screenheight - tipHeight) / 2;
    QColor color = QColor(100, 100, 100, 200);
    QRect rect = QRect(x, y, tipWidth, tipHeight);
    QString strTipsText = QString(tr("Tips:\nDrag mouse to screenshot"));
    painter.fillRect(rect, color);
    painter.setPen(QPen(Qt::white));
    painter.drawText(rect, Qt::AlignCenter, strTipsText);
}

QRect fullScreenWidget::getSelectedRect()
{
    if (currentShotState == beginMoveShot) {
        return getMoveAllSelectedRect();
    }

    else if (currentShotState == beginControl) {
        return getMoveControlSelectedRect();
    }

    else {
        return getRect(beginPoint, endPoint);
    }
}

void fullScreenWidget::updateBeginEndPointValue(const QRect & rect)
{
    beginPoint = rect.topLeft();
    endPoint = rect.bottomRight();
    moveBeginPoint = QPoint(0, 0);
    moveEndPoint = QPoint(0, 0);
}

void fullScreenWidget::checkMoveEndPoint()
{
    int x, y;
    QRect selectedRect = getRect(beginPoint, endPoint);
    QPoint bottomRightPoint = selectedRect.bottomRight();
    x = moveEndPoint.x() - moveBeginPoint.x();
    y = moveEndPoint.y() - moveBeginPoint.y();
    if (x + selectedRect.x() < 0) {
        moveEndPoint.setX(qAbs(selectedRect.x() - moveBeginPoint.x()));
    }
    if (y + selectedRect.y() < 0) {
        moveEndPoint.setY(qAbs(selectedRect.y() - moveBeginPoint.y()));
    }
    if (x + bottomRightPoint.x() > screenwidth) {
        moveEndPoint.setX(screenwidth - bottomRightPoint.x() +
                          moveBeginPoint.x());
    }
    if (y + bottomRightPoint.y() > screenheight) {
        moveEndPoint.setY(screenheight - bottomRightPoint.y() +
                          moveBeginPoint.y());
    }
}

void fullScreenWidget::draw8ControlPoint(const QRect & rect)
{
    int x, y;
    QColor color = QColor(0, 0, 255);
    QPoint tlPoint = rect.topLeft();
    QPoint trPoint = rect.topRight();
    QPoint blPoint = rect.bottomLeft();
    QPoint brPoint = rect.bottomRight();
    x = (tlPoint.x() + trPoint.x()) / 2;
    y = tlPoint.y();
    QPoint tcPoint = QPoint(x, y);
    x = (blPoint.x() + brPoint.x()) / 2;
    y = blPoint.y();
    QPoint bcPoint = QPoint(x, y);
    x = tlPoint.x();
    y = (tlPoint.y() + blPoint.y()) / 2;
    QPoint lcPoint = QPoint(x, y);
    x = trPoint.x();
    y = (trPoint.y() + brPoint.y()) / 2;
    QPoint rcPoint = QPoint(x, y);
    tlRect = QRect(tlPoint.x() - 2, tlPoint.y() - 2, 6, 6);
    trRect = QRect(trPoint.x() - 2, trPoint.y() - 2, 6, 6);
    blRect = QRect(blPoint.x() - 2, blPoint.y() - 2, 6, 6);
    brRect = QRect(brPoint.x() - 2, brPoint.y() - 2, 6, 6);
    tcRect = QRect(tcPoint.x() - 2, tcPoint.y() - 2, 6, 6);
    bcRect = QRect(bcPoint.x() - 2, bcPoint.y() - 2, 6, 6);
    lcRect = QRect(lcPoint.x() - 2, lcPoint.y() - 2, 6, 6);
    rcRect = QRect(rcPoint.x() - 2, rcPoint.y() - 2, 6, 6);
    painter.fillRect(tlRect, color);
    painter.fillRect(trRect, color);
    painter.fillRect(blRect, color);
    painter.fillRect(brRect, color);
    painter.fillRect(tcRect, color);
    painter.fillRect(bcRect, color);
    painter.fillRect(lcRect, color);
    painter.fillRect(rcRect, color);
}

void fullScreenWidget::updateMouseShape(const QPoint & point)
{
    switch (currentShotState) {
    case initShot:
    case beginShot:
        setCursor(Qt::CrossCursor);
        break;
    case beginMoveShot:
        setCursor(Qt::OpenHandCursor);
        break;
    case finishShot:
    case finishMoveShot:
    case finishControl:
        if (getSelectedRect().contains(point))
            setCursor(Qt::OpenHandCursor);
        else
            updateMoveControlMouseShape(getMoveControlState
                                        (point));
        break;
    case beginControl:
        updateMoveControlMouseShape(controlValue);
        break;
    default:
        setCursor(Qt::ArrowCursor);
        break;
    }
}

void fullScreenWidget::updateMoveControlMouseShape(controlPointEnum controlValue)
{
    switch (controlValue) {
    case moveControl1:
    case moveControl5:
        setCursor(Qt::SizeFDiagCursor);
        break;
    case moveControl2:
    case moveControl6:
        setCursor(Qt::SizeVerCursor);
        break;
    case moveControl3:
    case moveControl7:
        setCursor(Qt::SizeBDiagCursor);
        break;
    case moveControl4:
    case moveControl8:
        setCursor(Qt::SizeHorCursor);
        break;
    default:
        setCursor(Qt::ArrowCursor);
        break;
    }
}

fullScreenWidget::controlPointEnum fullScreenWidget::getMoveControlState(const QPoint & point)
{
    fullScreenWidget::controlPointEnum result = moveControl0;
    if (currentShotState == initShot || currentShotState == beginShot) {
        result = moveControl0;
    }

    else if (tlRect.contains(point)) {
        result = moveControl1;
    }

    else if (tcRect.contains(point)) {
        result = moveControl2;
    }

    else if (trRect.contains(point)) {
        result = moveControl3;
    }

    else if (rcRect.contains(point)) {
        result = moveControl4;
    }

    else if (brRect.contains(point)) {
        result = moveControl5;
    }

    else if (bcRect.contains(point)) {
        result = moveControl6;
    }

    else if (blRect.contains(point)) {
        result = moveControl7;
    }

    else if (lcRect.contains(point)) {
        result = moveControl8;
    }

    else {
        result = moveControl0;
    }
    return result;
}

QRect fullScreenWidget::getMoveAllSelectedRect(void)
{
    QRect result;
    QPoint tmpBeginPoint, tmpEndPoint;
    int moveX, moveY;
    checkMoveEndPoint();
    moveX = moveEndPoint.x() - moveBeginPoint.x();
    moveY = moveEndPoint.y() - moveBeginPoint.y();
    tmpBeginPoint.setX(beginPoint.x() + moveX);
    tmpBeginPoint.setY(beginPoint.y() + moveY);
    tmpEndPoint.setX(endPoint.x() + moveX);
    tmpEndPoint.setY(endPoint.y() + moveY);
    result = getRect(tmpBeginPoint, tmpEndPoint);
    return result;
}

QRect fullScreenWidget::getMoveControlSelectedRect(void)
{
    int x, y, w, h;
    QRect rect = getRect(beginPoint, endPoint);
    QRect result;
    switch (controlValue) {
    case moveControl1:
        result = getRect(rect.bottomRight(), moveEndPoint);
        return result;
        break;
    case moveControl2:
        x = rect.x();
        y = getMinValue(moveEndPoint.y(), rect.bottomLeft().y());
        w = rect.width();
        h = qAbs(moveEndPoint.y() - rect.bottomRight().y());
        break;
    case moveControl3:
        result = getRect(rect.bottomLeft(), moveEndPoint);
        return result;
        break;
    case moveControl4:
        x = getMinValue(rect.x(), moveEndPoint.x());
        y = rect.y();
        w = qAbs(rect.bottomLeft().x() - moveEndPoint.x());
        h = rect.height();
        break;
    case moveControl5:
        result = getRect(rect.topLeft(), moveEndPoint);
        return result;
        break;
    case moveControl6:
        x = rect.x();
        y = getMinValue(rect.y(), moveEndPoint.y());
        w = rect.width();
        h = qAbs(moveEndPoint.y() - rect.topLeft().y());
        break;
    case moveControl7:
        result = getRect(moveEndPoint, rect.topRight());
        return result;
        break;
    case moveControl8:
        x = getMinValue(moveEndPoint.x(), rect.bottomRight().x());
        y = rect.y();
        w = qAbs(rect.bottomRight().x() - moveEndPoint.x());
        h = rect.height();
        break;
    default:
        result = getRect(beginPoint, endPoint);
        return result;
        break;
    }
    return QRect(x, y, w, h);
}

int fullScreenWidget::getMinValue(int num1, int num2)
{
    return num1 < num2 ? num1 : num2;
}

void fullScreenWidget::drawSelectedPixmap(void)
{
    painter.drawRect(selectedRect);
    shotPixmap = loadPixmap.copy(selectedRect);
    if (selectedRect.width() > 0 && selectedRect.height()) {
        painter.drawPixmap(selectedRect.topLeft(), shotPixmap);
    }
    draw8ControlPoint(selectedRect);
}

void fullScreenWidget::drawXYWHInfo(void)
{
    int x, y;
    QColor color = QColor(239, 234, 228, 200);
    QRect rect;
    QString strTipsText;
    switch (currentShotState) {
    case beginShot:
    case finishShot:
    case beginMoveShot:
    case finishMoveShot:
    case beginControl:
    case finishControl:
        x = selectedRect.x() + 5;
        y = selectedRect.y() >
                infoHeight ? selectedRect.y() -
                             infoHeight : selectedRect.y();
        rect = QRect(x, y, infoWidth, infoHeight);
        strTipsText =
                QString(tr(" XY:\n x:%1 y:%2\n w:%3 h:%4")).
                arg(selectedRect.x(), 4).arg(selectedRect.y(),
                                             4) .arg(selectedRect.width(),
                                                     4).arg(selectedRect.
                                                            height(), 4);
        painter.fillRect(rect, color);
        painter.setPen(QPen(Qt::black));
        painter.drawText(rect, Qt::AlignLeft | Qt::AlignVCenter,
                         strTipsText);
        break;
    default:
        break;
    }
}
