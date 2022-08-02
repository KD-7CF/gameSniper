#include "game.h"

game::game(QWidget *parent) : QWidget(parent)
{

    scene = new QGraphicsScene(this);
    view = new QGraphicsView(this);
    view->setCursor(Qt::CrossCursor);

    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setEnabled(false);
    view->setBackgroundBrush(Qt::black);

    paysage = new Paysage("paysage_1.jpg", scene, view);
    avatar = new Principal("1.png", scene, view);
    mechants = new Mechants("1.png", scene, view);

    paysage->connect(this, SIGNAL(posxy(QPointF)), SLOT(move(QPointF)));
    paysage->connect(this, SIGNAL(wheel(bool, QPointF)), SLOT(zoom(bool, QPointF)));

    avatar->connect(this, SIGNAL(posxy(QPointF)), SLOT(move(QPointF)));
    avatar->connect(this, SIGNAL(wheel(bool, QPointF)), SLOT(zoom(bool, QPointF)));
    avatar->connect(this, SIGNAL(new_part()), SLOT(reset()));
    avatar->connect(mechants, SIGNAL(shot_principal()), SLOT(setlife()));
    avatar->connect(mechants, SIGNAL(mechants_failed()), SLOT(win()));

    mechants->connect(this, SIGNAL(new_part()), SLOT(reset()));
    mechants->connect(avatar, SIGNAL(shot_mechant(QPointF)), SLOT(zone(QPointF)));

    this->connect(avatar, SIGNAL(game_over()), SLOT(GAME_OVER()));


    paysage->update();
    avatar->update();
    mechants->update();

    emit avatar->activate_sniper(false);

    view->setScene(scene);
    view->scale(0.25, 0.25);
    view->setSceneRect(paysage->def_scene_rect);

    this->installEventFilter(this);
    view->viewport()->setMouseTracking(true);
    view->viewport()->installEventFilter(this); /// filtres des evenements

    combobox = new QComboBox(this);
    btn_state = new QPushButton(this);
    btn_quit = new QPushButton(this);


    btn_state->setGeometry(
                (this->width() - 200)/2, (this->height() - 200)/2,
                200, 200
                );
    btn_quit->setGeometry(
                this->width() - 120, 20,
                50, 50
                );
    combobox->setGeometry(
                (this->width() - 80)/2, this->height()/2 + 150,
                80, 40
                );

    btn_state->setText("Play");
    btn_quit->setText("X");
    btn_state->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0.5); border-radius: 50px; color: white; font-family: Impact; font-size: 25px }"
                             "QPushButton:hover { background-color: rgba(30, 30, 30, 0.5); font-size: 50px }"
                             "QPushButton:pressed { color: cyan; }");
    btn_quit->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0.5); border-radius: 50px; color: red; font-family: Arial Black; font-size: 25px }"
                            "QPushButton:hover { background-color: rgba(30, 30, 30, 0.5); font-size: 50px }");
    combobox->setStyleSheet("QComboBox { background-color: rgba(0, 0, 0, 0.5); border: 0px; color: white; font-family: Impact; font-size: 12px; }");

    combobox->addItem("facile");
    combobox->addItem("moyen");
    combobox->addItem("difficile");
    combobox->setCurrentIndex(1);

    btn_state->setFocusPolicy(Qt::NoFocus);
    btn_quit->setFocusPolicy(Qt::NoFocus);
    combobox->setFocusPolicy(Qt::NoFocus);
    this->connect(btn_state, SIGNAL(clicked()), SLOT(play()));
    this->connect(btn_quit, SIGNAL(clicked()), SLOT(quit()));
    mechants->connect(combobox, SIGNAL(currentIndexChanged(int)), SLOT(setlevel(int)));
}

game::~game()
{
    delete avatar;
    delete mechants;
    delete paysage;
    delete scene;
    delete view;

    delete combobox;
    delete btn_state;
    delete btn_quit;
}

void game::quit()
{
    this->close();
}

void game::play() {
    btn_state->setText("Play");
    btn_state->setVisible(false);
    combobox->setVisible(false);
    view->viewport()->setEnabled(true);
    if(state)
        return;
    center_mouse();
    emit new_part();
    mechants->update();
    avatar->update();
    state = true;
    startTimer(300);
}

void game::stop() {
    btn_state->setVisible(true);
    combobox->setVisible(true);
    view->viewport()->setEnabled(false);
    state = false;
}

void game::center_mouse()
{
    QPoint ptcenter = mapToGlobal(QPoint(this->width()/2, this->height()/2));
    this->cursor().setPos(ptcenter);
}

void game::GAME_OVER()
{
    if(avatar->failed)
        btn_state->setText("Failed");
    else {
        btn_state->setText("Win");
        //        avatar->playsound("win");
    }
    stop();
    this->setCursor(Qt::CrossCursor);
}


bool game::eventFilter(QObject *object, QEvent *event)
{

    if (event->type() == QEvent::KeyPress) {

        QKeyEvent *keyEv = static_cast <QKeyEvent*> (event);

        switch(keyEv->key()) {

        case (Qt::Key_Enter):
        case (Qt::Key_Return):
            if(!state)
                play();

            break;

        case (Qt::Key_Escape):
            stop();

            break;
        case (Qt::Key_Space):
            if(state)
                stop();
            else
                play();

            break;

        case (Qt::Key_Up):
            if(avatar->state_sniper)
                avatar->ptf_adjust.ry()-=15;
            emit avatar->adjust_sniper();

            break;
        case (Qt::Key_Down):
            if(avatar->state_sniper)
                avatar->ptf_adjust.ry()+=15;
            emit avatar->adjust_sniper();

            break;
        case (Qt::Key_Left):
            if(avatar->state_sniper)
                avatar->ptf_adjust.rx()-=15;
            emit avatar->adjust_sniper();

            break;
        case (Qt::Key_Right):
            if(avatar->state_sniper)
                avatar->ptf_adjust.rx()+=15;
            emit avatar->adjust_sniper();

            break;

        }
    }

    if(state)
        if(object == view->viewport())
        {
            if(event->type() == QEvent::MouseMove) {
                QMouseEvent *mouseEv = static_cast <QMouseEvent*> (event);
                int mv_limit = 30;
                if(avatar->state_sniper)
                    mv_limit = 250;

                if(mouseEv->position().x() > (view->viewport()->width() - mv_limit)
                        || mouseEv->position().x() < mv_limit
                        || mouseEv->position().y() > (view->viewport()->height() - mv_limit)
                        || mouseEv->position().y() < mv_limit)
                    center_mouse();

                else
                    emit posxy(mouseEv->position());
                update();

                mechants->move(mouseEv->position());
            }

            if(event->type() == QEvent::Wheel)  {
                QWheelEvent *wheelEv = static_cast<QWheelEvent *>(event);
                if(wheelEv->angleDelta().y() > 0)
                    emit wheel(true, wheelEv->position());
                else
                    emit wheel(false, wheelEv->position());
            }

            if(event->type() == QEvent::MouseButtonPress) {
                QMouseEvent *mouseEv = static_cast <QMouseEvent*> (event);

                if(mouseEv->button() == Qt::LeftButton)
                    emit avatar->action_shot(true);
                if(mouseEv->button() == Qt::RightButton) {
                    if(!avatar->state_sniper)
                        emit avatar->mode_sniper(true);
                    else if(avatar->state_sniper)
                        emit avatar->mode_sniper(false);
                    emit posxy(mouseEv->position());
                }

            }

        }


    if(event->type() == QEvent::Resize)
        view->resize(this->width(), this->height());

    return false;
}

void game::timerEvent(QTimerEvent *event)
{
    if(state) {
        emit avatar->action_shot(false);
        emit mechants->shot();
    } else
        killTimer(event->timerId());

}
