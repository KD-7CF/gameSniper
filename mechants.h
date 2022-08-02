#ifndef MECHANTS_H
#define MECHANTS_H

#define MAX 12
#include <QTimerEvent>
#include "personnage.h"

class Mechants : public Personnage
{
    Q_OBJECT

private:
    int ntime = 0;
    int nbr = MAX;
    float scale[MAX] = {
        2.5, // porte_1
        2,   // porte_2
        1.5, // fenetre
        1,   // arbre cachette_1
        1,   // arbre cachette_2
        1,   // arbre cachette_3
        0.8, // arbre loin cachette_1
        0.8, // arbre loin cachette_2
        0.9, // cachette gauche
        0.9, // à gauche cabane 1
        1,   // sur cabane 1&2
        1.5, // sur cabane 3
    };
    QPointF ptfs[MAX] = {
        {3185, 2396}, // porte_1
        {5365, 2372}, // porte_2
        {2350, 2565}, // fenetre

        {3250, 600},  // arbre cachette_1
        {3230, 840},  // arbre cachette_2
        {2850, 1088}, // arbre cachette_3

        {3620, 1450}, // arbre loin cachette_1
        {4150, 1270}, // arbre loin cachette_2

        {1550, 2390}, // cachette gauche
        {1950, 2350}, // à gauche cabane 1
        {4010, 1790}, // sur cabane 1&2
        {4810, 1670}, // sur cabane 3
    };

    int id_sniper = 0;
    void sel_sniper(int);
    void dead(int);
    int who_dead;
    QGraphicsPixmapItem pixdead[7];

public:
    Mechants(QString src, QGraphicsScene* scene, QGraphicsView *view);
    virtual ~Mechants();
    void snipers(int);
    bool update();

signals:
    void action_shot();
    void shot_principal();
    void mechants_failed();

public slots:
    void reset();
    void shot();
    void zone(QPointF);
    void setlevel(int);

protected:
    void timerEvent(QTimerEvent *event);
};

#endif // MECHANTS_H
