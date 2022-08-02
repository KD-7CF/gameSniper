#ifndef PRINCIPAL_H
#define PRINCIPAL_H
#define SRC_NOSNIPER_IMG QString(SRC_PR) + "1.png";
#define SRC_NOSNIPER_IMG_SHOT QString(SRC_PR) + "2.png";
#define SRC_SNIPER_IMG QString(SRC_PR) + "Sniper/sniper.png";
#define SRC_SNIPER_IMG_SHOT QString(SRC_PR) + "Sniper/sniper_shoot.png";

#include <QProgressBar>
#include <QTimerEvent>
#include <QScrollBar>

#include "personnage.h"

class Principal : public Personnage
{
    Q_OBJECT

private:
    int ntime = 0;
    QTransform transform;
    QPointF ptf_cursor;
    QGraphicsEllipseItem *circle = nullptr;
    void target_area(QPointF, int r = 300);
    QGraphicsPixmapItem pixblood;
    QProgressBar *progressbar;

public:
    explicit Principal(QString src, QGraphicsScene* scene, QGraphicsView *view);
    virtual ~Principal();
    bool state_sniper = false;
    QPointF ptf_adjust;
    bool update();

signals:
    void activate_sniper(bool);
    void adjust_sniper();
    void action_shot(bool);
    void shot_mechant(QPointF);
    void game_over();

public slots:
    void move(QPointF);
    void zoom(bool, QPointF);
    void reset();
    void mode_sniper(bool);
    void move_gun();
    void shot(bool);
    void setlife();
    void win();

protected:
    void timerEvent(QTimerEvent *event);
};

#endif // PRINCIPAL_H
