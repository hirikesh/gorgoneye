#ifndef GLGAZE_H
#define GLGAZE_H

#include <QGraphicsView>
#include "config.h"

class Store;

class GLGaze : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GLGaze(Store* st);

signals:
    void closed();

protected:
    void resizeEvent(QResizeEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void closeEvent(QCloseEvent* event);
};

#endif // GLGAZE_H
