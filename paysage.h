#ifndef PAYSAGE_H
#define PAYSAGE_H

#include <QScrollBar>

#include "univers.h"

class Paysage : public Univers
{
    Q_OBJECT

public:
    explicit Paysage(QString src, QGraphicsScene* scene, QGraphicsView *view);
    virtual ~Paysage();

public:
    bool update();

public slots:
    void move(QPointF);
    void zoom(bool, QPointF);
};

#endif // PAYSAGE_H
