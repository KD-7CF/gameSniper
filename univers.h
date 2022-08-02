#ifndef UNIVERS_H
#define UNIVERS_H

#define SRC_PY "../GAME_QT/Images/Paysage/"
#define SRC_PR "../GAME_QT/Images/Personnage/Principal/"
#define SRC_ME "../GAME_QT/Images/Personnage/Mechant/"

#define del_ptab(x) { delete[] (x); (x) = nullptr; }

#include <QSoundEffect>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>

class Univers : public QObject
{
    Q_OBJECT

public:
    explicit Univers (QObject *parent = nullptr) : QObject(parent) {}
    QString src;
    QPointF pointf;
    QSoundEffect *soundeffect = nullptr;
    QGraphicsPixmapItem *pixItem = nullptr;
    QGraphicsScene *scene = nullptr;
    QGraphicsView *view = nullptr;


    const QRectF def_scene_rect = {0, 0, 6480, 4320};
    const int tr_constant = 3;
    int tr_speed = 2;
    int tr_width = 0;
    int tr_height = 0;
    int zoom_t = 0;
    float zoom_scale = 1.5;
    int zoom_lim_t = 3;
    virtual bool update() {return false;};

public slots:
    virtual void move(QPointF) {};
    virtual void zoom(bool, QPointF) {};
    virtual void reset() {};
};

#endif // UNIVERS_H
