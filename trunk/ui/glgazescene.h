#ifndef GLGAZESCENE_H
#define GLGAZESCENE_H

#include <QGraphicsScene>

class Store;

class GLGazeScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GLGazeScene(Store* st, int w, int h, QObject *parent = 0);

    void setCalibInfo(int x, int y, int dx, int dy);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect);

private:
    int topLeftX;
    int topLeftY;
    int botRightX;
    int botRightY;
    int deltaX;
    int deltaY;

    Store* store;
};

#endif // GLGAZESCENE_H
