#include "mechants.h"

Mechants::Mechants(QString src, QGraphicsScene *scene, QGraphicsView *view) : Personnage(view->parent())
{
    this->src = QString(SRC_ME) + src;
    this->scene = scene;
    this->view = view;

    std::srand(std::time(nullptr));
    pixdead[0].setPixmap(QString(SRC_ME) + "d_arme.png");
    pixdead[1].setPixmap(QString(SRC_ME) + "d_main-1.png");
    pixdead[2].setPixmap(QString(SRC_ME) + "d_main-2.png");
    pixdead[3].setPixmap(QString(SRC_ME) + "d_pied-1.png");
    pixdead[4].setPixmap(QString(SRC_ME) + "d_pied-2.png");
    pixdead[5].setPixmap(QString(SRC_ME) + "d_dos.png");
    pixdead[6].setPixmap(QString(SRC_ME) + "d_tete.png");
    snipers(MAX);
    this->connect(this, SIGNAL(action_shot()), SLOT(shot()));


    soundeffect = new QSoundEffect[5];
    soundeffect[0].setSource(QUrl::fromLocalFile(":/effect/Sounds/c2"));
    soundeffect[1].setSource(QUrl::fromLocalFile(":/effect/Sounds/c3"));
    soundeffect[2].setSource(QUrl::fromLocalFile(":/effect/Sounds/t2"));
    soundeffect[3].setSource(QUrl::fromLocalFile(":/effect/Sounds/t3"));
    soundeffect[4].setSource(QUrl::fromLocalFile(":/effect/Sounds/t4"));
    soundeffect[0].setVolume(1);
    soundeffect[1].setVolume(1);
    soundeffect[2].setVolume(1);
    soundeffect[3].setVolume(1);
    soundeffect[4].setVolume(1);

}

Mechants::~Mechants()
{
    delete[] soundeffect;
    delete[] pixItem;
}

bool Mechants::update()
{
    if(pixItem == nullptr || scene == nullptr)
        return false;

    for(int i = 0, k = 0; i < nbr; k++, i++) {
        if(k < 7) {
            scene->addItem(&pixdead[k]);
            pixdead[k].setScale(0);
        }
        scene->addItem(&pixItem[i]);
    }

    return true;
}

void Mechants::snipers(int n)
{
    if(pixItem != nullptr)
        del_ptab(pixItem);

    pixItem = new QGraphicsPixmapItem[n];
    for(int i = 0; i < n; i++) {
        int rand = std::rand() % MAX;
        pixItem[i].setPixmap(this->src);
        pixItem[i].setPos(ptfs[rand]);
        pixItem[i].setScale(scale[rand]);
    }

    nbr = n;
}

void Mechants::reset()
{
    snipers(MAX);
    failed = false;
}

void Mechants::sel_sniper(int id)
{
    QString src_shoot = QString(SRC_ME) + "2.png";
    pixItem[id].setPixmap(src_shoot);
    pixItem[id_sniper].setPixmap(this->src);
    id_sniper = id;
}

void Mechants::dead(int n)
{
    // effet souffrir
    int rok = std::rand() % 2;
    if(rok == 0) {
        if(!soundeffect[0].isPlaying())
            soundeffect[0].play();
    } else {
        if(!soundeffect[1].isPlaying())
            soundeffect[1].play();
    }

    scene->removeItem(&pixItem[n]);
    who_dead = n;
    ntime = 0;
    startTimer(10);

    int n_deads = 0;
    for(int i = 0; i < nbr; i++)
        if(pixItem[i].scene() == nullptr)
            n_deads++;

    if(n_deads == nbr) {
        failed = true;
        emit mechants_failed();
    }
}

void Mechants::zone(QPointF ptf)
{
    for(int i = 0; i < nbr; i++) {
        pointf = pixItem[i].pos();
        if(fabs(pointf.x() - ptf.x()) < 100 && fabs(pointf.y() - ptf.y()) < 100)
            dead(i);
    }
}

void Mechants::setlevel(int n)
{
    level = n;
}

void Mechants::timerEvent(QTimerEvent *event)
{
    if(ntime > 100)
        killTimer(event->timerId());
    ntime++;
    for(int i = 0; i < 7; i++) {
        double dx =  pow(-1, i) * 100;
        double dy =  ntime;
        pixdead[i].setScale(pixItem[who_dead].scale());
        pixdead[i].setPos(pixItem[who_dead].pos().x() + dx, pixItem[who_dead].pos().y() + dy);
    }
}

void Mechants::shot()
{
    if(failed)
        return;

    int rok = std::rand() % 3;
    if(rok == 0) {
        if(!soundeffect[2].isPlaying())
            soundeffect[2].play();
    }
    else if (rok == 1) {
        if(!soundeffect[3].isPlaying())
            soundeffect[3].play();
    }
    else if (rok == 2) {
        if(!soundeffect[4].isPlaying())
            soundeffect[4].play();
    }

    sel_sniper(std::rand() % nbr);
    int rand = std::rand() % (3 - level);
    if(rand == 0) {
        emit shot_principal();
    }

}
