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
    explicit GLGazeScene(Store* st, float scale, int dpix, int dpiy, QObject *parent = 0);
    void updateCalib();

protected:
    void drawBackground(QPainter* painter, const QRectF& rect);
    void drawForeground(QPainter* painter, const QRectF& rect);
    void keyPressEvent(QKeyEvent* event);

private:
    float dpiScale;
    int dpiX;
    int dpiY;

    int deltaX;
    int deltaY;
    int outerW;
    int outerH;

    int hystThreshX;
    int hystThreshY;
    int currGazeX;
    int currGazeY;

    Store* store;
};

#endif // GLGAZESCENE_H
