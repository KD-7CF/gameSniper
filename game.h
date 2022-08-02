#ifndef GAME_H
#define GAME_H

#define WIDTH 1080
#define HEIGTH 720

#include <QWidget>
#include <QKeyEvent>

#include <QComboBox>
#include <QPushButton>

#include "principal.h"
#include "mechants.h"
#include "paysage.h"

class game : public QWidget
{
    Q_OBJECT

public:
    game(QWidget *parent = nullptr);
    ~game();

    void center_mouse();

private:
    QGraphicsScene *scene;
    QGraphicsView *view;

    Principal *avatar;
    Mechants *mechants;
    Univers *paysage;

    bool state = false;

    QComboBox *combobox;
    QPushButton *btn_state;
    QPushButton *btn_quit;

signals:
    void posxy(QPointF);
    void wheel(bool, QPointF);
    void new_part();

public slots:
    void GAME_OVER();
    void quit();
    void play();
    void stop();

protected:
    bool eventFilter (QObject *object , QEvent *event);
    void timerEvent(QTimerEvent *event);
};

#endif // GAME_H
