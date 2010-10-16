#ifndef GLGAZE_H
#define GLGAZE_H

#include <QGraphicsView>

class QPushButton;
class Store;

class GLGaze : public QGraphicsView
{
    Q_OBJECT
public:
    GLGaze(Store* st);

signals:
    void closed();

protected:
    void resizeEvent(QResizeEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void closeEvent(QCloseEvent* event);

protected slots:
    void setTrainingMode(bool en);

private:
    Store* store;
    QPushButton* trainingModeBtn;
};

#endif // GLGAZE_H
