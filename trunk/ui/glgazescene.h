#ifndef GLGAZESCENE_H
#define GLGAZESCENE_H

#include <QGraphicsScene>
#include "config.h"

class QPushButton;
class Store;

class GLGazeScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GLGazeScene(Store* st, QObject *parent = 0);

    void setCalibInfo(int dx, int dy, int cw, int ch);
    void updateWidgetPos();

protected:
    void drawBackground(QPainter* painter, const QRectF& rect);
    void drawForeground(QPainter* painter, const QRectF& rect);
    void keyPressEvent(QKeyEvent* event);

protected slots:
    void startCalibMode();

private:
    QPushButton* calibModeBtn;

    int deltaX;
    int deltaY;
    int calibW;
    int calibH;
    int botLeftX;
    int botLeftY;
    int topRightX;
    int topRightY;

    Store* store;
};

#endif // GLGAZESCENE_H
