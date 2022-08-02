#include "principal.h"

Principal::Principal(QString src, QGraphicsScene *scene, QGraphicsView *view) : Personnage(view->parent())
{
    transform.scale(0.25, 0.25);
    this->src = QString(SRC_PR) + src;
    this->scene = scene;
    this->view = view;
    pixItem = new QGraphicsPixmapItem;
    QWidget *widget_parent = static_cast <QWidget*> (view->parent());

    progressbar = new QProgressBar(widget_parent);
    progressbar->resize(200, 5);
    progressbar->setTextVisible(false);
    progressbar->setStyleSheet("QProgressBar { background-color: rgba(0, 0, 0, 0.5); border: 0px }"
                               "QProgressBar::chunk { background-color: rgba(255, 0, 0, 1) }");
    progressbar->move((widget_parent->width() - 200)/2, 20);

    this->connect(this, SIGNAL(action_shot(bool)), SLOT(shot(bool)));
    this->connect(this, SIGNAL(activate_sniper(bool)), SLOT(mode_sniper(bool)));
    this->connect(this, SIGNAL(adjust_sniper()), SLOT(move_gun()));

    soundeffect = new QSoundEffect[7];
    soundeffect[0].setSource(QUrl::fromLocalFile(":/effect/Sounds/c1"));
    soundeffect[1].setSource(QUrl::fromLocalFile(":/effect/Sounds/ch"));
    soundeffect[2].setSource(QUrl::fromLocalFile(":/effect/Sounds/ro"));
    soundeffect[3].setSource(QUrl::fromLocalFile(":/effect/Sounds/t1"));
    soundeffect[4].setSource(QUrl::fromLocalFile(":/effect/Sounds/mv"));
    soundeffect[5].setSource(QUrl::fromLocalFile(":/effect/Sounds/d"));
    soundeffect[6].setSource(QUrl::fromLocalFile(":/effect/Sounds/&"));
    soundeffect[0].setVolume(1);
    soundeffect[1].setVolume(1);
    soundeffect[2].setVolume(1);
    soundeffect[3].setVolume(1);
    soundeffect[4].setVolume(1);
    soundeffect[5].setVolume(1);
    soundeffect[6].setVolume(1);

    soundeffect[6].play(); // effet opening
}

Principal::~Principal()
{
    delete[] soundeffect;
    delete pixItem;
    delete progressbar;
}

void Principal::reset()
{
    if(ntime != 0)
        view->setTransform(transform);
    scene->removeItem(pixItem);
    scene->removeItem(&pixblood);
    pixblood.setScale(0);
    failed = false;
    life = 100;
}

bool Principal::update()
{
    if(pixItem == nullptr || scene == nullptr)
        return false;
    scene->addItem(&pixblood);
    scene->addItem(pixItem);
    return true;
}

void Principal::move(QPointF ptf)
{
    int rok = std::rand() % 10;
    if(rok == 5)
        if(!soundeffect[4].isPlaying())
            soundeffect[4].play();  // effet movement

    QRectF rectf = pixItem->boundingRect();
    QPointF ptf_center_item = { rectf.width()/2, rectf.height()/2 };
    if(state_sniper) {
        QPointF dptf = view->mapToScene(view->viewport()->width(), view->viewport()->height());
        if(dptf.x() >= scene->width() - 1 || dptf.y() >= view->scene()->height() - 1)
            return;
        pointf = view->mapToScene(view->viewport()->width()/2 - ptf_center_item.x(), view->viewport()->height() - ptf_center_item.y()*2);
        pixItem->setPos(pointf + ptf_adjust);
    }

    else if(!state_sniper) {
        QPointF dptf = view->mapToScene(view->viewport()->width(), view->viewport()->height());
        if(dptf.x() >= scene->width() - 1 || dptf.y() >= view->scene()->height() - 1)
            return;

        pointf = view->mapToScene(view->viewport()->width()/2 - ptf_center_item.x(), view->viewport()->height() - ptf_center_item.y()*1.5);
        pixItem->setPos(pointf);
        ptf_cursor = view->mapToScene(ptf.toPoint());
        target_area(ptf_cursor);
    }

}

void Principal::target_area(QPointF ptf, int r)
{

    if(circle != nullptr)
        scene->removeItem(circle);
    QBrush red(Qt::transparent);
    QPen pen(Qt::red, 5);
    circle = scene->addEllipse(ptf.x() - r/2, ptf.y() - r/2, r, r, pen, red);

}

void Principal::zoom(bool ok, QPointF ptf)
{
    soundeffect[1].play(); // effet sniper
    move(ptf);
}

void Principal::shot(bool ok)
{

    if(ok) {
        soundeffect[3].play(); // effet coup de feu
        if(state_sniper) {
            QString src_sniper_png = SRC_SNIPER_IMG_SHOT;
            pixItem->setPixmap(src_sniper_png);
            zoom_scale = 6;
            pixItem->setScale(zoom_scale);
            QRectF rectf = pixItem->boundingRect();
            QPointF ptf_center_item = { zoom_scale*rectf.width()/2, zoom_scale*rectf.height()/2 };
            QPointF ptf_target = { pixItem->pos().x() + ptf_center_item.x()*0.9, pixItem->pos().y() + ptf_center_item.y()*0.6 };
            emit shot_mechant(ptf_target);
        } else {
            QString src_sniper_png = SRC_NOSNIPER_IMG_SHOT;
            pixItem->setPixmap(src_sniper_png);
            zoom_scale = 3;
            pixItem->setScale(zoom_scale);
            QPointF ptf_target = ptf_cursor - QPointF(185, 102);
            emit shot_mechant(ptf_target);
        }
    }
    else {
        if(state_sniper) {
            QString src_sniper_png = SRC_SNIPER_IMG;
            pixItem->setPixmap(src_sniper_png);
            zoom_scale = 6;
            pixItem->setScale(zoom_scale);
        } else {
            QString src_sniper_png = SRC_NOSNIPER_IMG;
            pixItem->setPixmap(src_sniper_png);
            zoom_scale = 3;
            pixItem->setScale(zoom_scale);
        }
    }
}

void Principal::setlife()
{
    life-=level;
    if(life < 30) {

        int rok = std::rand() % 3;
        if(rok == 1)
            if(!soundeffect[0].isPlaying())
                soundeffect[0].play();  // effet souffrir

        pixblood.setPixmap(QString(SRC_PR) + "sang.png");
        pixblood.setScale(6);
        pixblood.setPos(view->mapToScene(-180, view->viewport()->height()-300));

    }

    if(life < 0) {
        soundeffect[5].play();  // effet mourrir
        view->viewport()->setEnabled(false);
        failed = true;
        view->horizontalScrollBar()->setValue(view->horizontalScrollBar()->maximum()/2);
        view->verticalScrollBar()->setValue(view->verticalScrollBar()->maximum()/2);
        scene->removeItem(circle);
        scene->removeItem(pixItem);
        pixblood.setPos(view->mapToScene(-180, view->viewport()->height()-300));
        ntime = 0;
        startTimer(150);
        emit game_over();
    }

    progressbar->setValue(life);
}

void Principal::win()
{
    failed = false;
    emit game_over();
    soundeffect[6].play(); // effet opening
}

void Principal::mode_sniper(bool ok)
{
    soundeffect[1].play(); // effet sniper
    if(ok) {
        if(circle != nullptr)
            scene->removeItem(circle);
        QString src_sniper_png = SRC_SNIPER_IMG;
        pixItem->setPixmap(src_sniper_png);
        zoom_scale = 6;
        pixItem->setScale(zoom_scale);
        QWidget *widget_parent = static_cast <QWidget*> (view->parent());
        widget_parent->setCursor(Qt::BlankCursor);
        state_sniper = ok;
    }
    else {
        pixItem->setPixmap(this->src);
        zoom_scale = 3;
        pixItem->setScale(zoom_scale);
        QWidget *widget_parent = static_cast <QWidget*> (view->parent());
        widget_parent->setCursor(Qt::CrossCursor);
        ptf_adjust = {0, 0};
        state_sniper = ok;
    }

}

void Principal::move_gun()
{
    if(!soundeffect[2].isPlaying())
        soundeffect[2].play(); // effet rouler

    int limit_adjust = 1000;
    if(ptf_adjust.x() > limit_adjust)
        ptf_adjust.rx() = limit_adjust;
    if(ptf_adjust.y() > limit_adjust)
        ptf_adjust.ry() = limit_adjust;
    if(ptf_adjust.x() < -limit_adjust)
        ptf_adjust.rx() = -limit_adjust;
    if(ptf_adjust.y() < -limit_adjust)
        ptf_adjust.ry() = -limit_adjust;

    pixItem->setPos(pointf + ptf_adjust);

}

void Principal::timerEvent(QTimerEvent *event)
{
    ntime++;
    if(ntime > 11)
        killTimer(event->timerId());
    view->rotate(ntime);
}


















