#include "paysage.h"

Paysage::Paysage(QString src, QGraphicsScene *scene, QGraphicsView *view) : Univers(view->parent())
{
    this->src = QString(SRC_PY) + src;
    this->scene = scene;
    this->view = view;

    pixItem = new QGraphicsPixmapItem;
    pixItem->setPixmap(this->src);

    soundeffect = new QSoundEffect;
    soundeffect->setSource(QUrl::fromLocalFile(":/effect/Sounds/na"));
    soundeffect->setLoopCount(QSoundEffect::Infinite);
    soundeffect->setVolume(0.8);
    soundeffect->play();
}

Paysage::~Paysage()
{
    delete soundeffect;
    delete pixItem;
}

bool Paysage::update()
{
    if(pixItem == nullptr || scene == nullptr)
        return false;

    scene->addItem(pixItem);
    return true;
}

void Paysage::move(QPointF ptf)
{
    QPointF dptf = ptf - pointf;
    pointf = ptf;

    if(dptf.x() > 0)
        tr_width = view->horizontalScrollBar()->value() + tr_constant * tr_speed;
    else if(dptf.x() < 0)
        tr_width = view->horizontalScrollBar()->value() - tr_constant * tr_speed;
    if(dptf.y() > 0)
        tr_height = view->verticalScrollBar()->value() + tr_constant * tr_speed;
    else if(dptf.y() < 0)
        tr_height = view->verticalScrollBar()->value() - tr_constant * tr_speed;

    if(tr_width > view->horizontalScrollBar()->maximum())
        tr_width = view->horizontalScrollBar()->maximum();
    if(tr_width < view->horizontalScrollBar()->minimum())
        tr_width = view->horizontalScrollBar()->minimum();
    if(tr_height > view->verticalScrollBar()->maximum())
        tr_height = view->verticalScrollBar()->maximum();
    if(tr_height < view->verticalScrollBar()->minimum())
        tr_height = view->verticalScrollBar()->minimum();

    view->horizontalScrollBar()->setValue(tr_width);
    view->verticalScrollBar()->setValue(tr_height);

}

void Paysage::zoom(bool ok, QPointF ptf)
{

    view->centerOn(view->mapToScene(ptf.toPoint()));

    float scale = 1;

    if(ok) {
        scale = zoom_scale;
        zoom_t++;
    }
    else {
        scale = 1/zoom_scale;
        zoom_t--;
    }

    if(zoom_t > zoom_lim_t) {
        zoom_t = zoom_lim_t;
        return;
    }

    if(zoom_t < 0) {
        zoom_t = 0;
        return;
    }

    view->scale(scale, scale);
    view->setSceneRect(def_scene_rect);
    view->translate(1, 1);
    tr_width = view->horizontalScrollBar()->value();
    tr_height = view->verticalScrollBar()->value();

}
