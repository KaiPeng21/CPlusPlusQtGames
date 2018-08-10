#include "player.h"
#include "game.h"

extern Game *g;


Player::Player(QGraphicsItem *parent): QGraphicsPixmapItem(parent)
{
    //Q_UNUSED(parent);

    setPixmap(QPixmap(":/sounds/pigletrunright.png"));
    setScale(0.1);
    //setRect(0,0,SIZE,SIZE);

    setPos(WIDTH*SIZE/2,HEIGHT*SIZE/3);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    gravity = 0.5;
    velocityY = 0;
    onGround = false;
    leftMove = false;
    rightMove = false;

    phs = false;
}

bool Player::touchRightWall()
{
    if (pos().x() >= SIZE*WIDTH-SIZE){
        return true;
    }
    return false;
}

bool Player::touchLeftWall()
{
    if (pos().x() <= 0){
        return true;
    }
    return false;
}

bool Player::touchGround()
{
    if (pos().y() >= HEIGHT*SIZE-SIZE){
        return true;
    }

    return false;
}

void Player::start_jump()
{
    if (onGround || g->is_on_ground_offset()){
        g->play_effect(JMP);
        velocityY = -SIZE/2;
        onGround = false;
    }
}

void Player::end_jump()
{
    if (velocityY < -SIZE/4){
        velocityY = -SIZE/4;
    }
}

void Player::down_jump()
{
    if (!onGround){
        velocityY = SIZE/2;
    }
}

void Player::move_left()
{
    rightMove = false;
    if (!touchLeftWall()){
        setPos(x() - 10, y());
    }
}

void Player::move_right()
{
    leftMove = false;
    if (!touchRightWall()){
        setPos(x() + 10, y());
    }
}

void Player::keyPressEvent(QKeyEvent *e)
{
    if (g->game_status != PLAY && e->key() == Qt::Key_Space){
        g->initGame();
    }
    if (g->game_status == PLAY && e->key() == Qt::Key_Escape){
        g->game_status = OVER;
        g->musiclist->setCurrentIndex(OVE);
        g->musiclist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
        g->musicplayer->play();
    }


    if (e->key() == Qt::Key_Left){
        leftMove = true;
    }
    if (e->key() == Qt::Key_Right){
        rightMove = true;
    }
    if (e->key() == Qt::Key_Space || e->key() == Qt::Key_Up){
        start_jump();
    }


    if (e->key() == Qt::Key_Down){
        down_jump();
    }

}

void Player::keyReleaseEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Left){
        leftMove = false;
    }
    if (e->key() == Qt::Key_Right){
        rightMove = false;
    }
    if (e->key() == Qt::Key_Space || e->key() == Qt::Key_Up){
        end_jump();
    }
}

void Player::jump_update()
{
    if (g->game_is_over()){
        return;
    }

    if (leftMove){
        move_left();
    }
    if (rightMove){
        move_right();
    }

    if (g->player_hit_stair_ceil()){
        down_jump();
    }

    if (!onGround){
        velocityY += gravity;
        setPos(x(), y()+velocityY);
    }

    if (g->touch_stair_ground() != -1){
        onGround = true;
        velocityY = 0;
        setPos(x(), g->touch_stair_ground() - SIZE);
    }
    else{
        onGround = false;
        velocityY += gravity;
        setPos(x(), y()+velocityY);
    }

    if (g->player_jump_on_enemy()){
        g->enemy->show_enemy();
    }

    if (g->game_is_over()){
        g->game_status = OVER;
    }

}

void Player::update_pixmap()
{
    if (g->game_is_over()){
        return;
    }

    if (!phs){
        if (rightMove){
            setPixmap(QPixmap(":/sounds/pigletrunright.png"));
        }
        else if (leftMove){
            setPixmap(QPixmap(":/sounds/pigletrunleft.png"));
        }
    }
    else{
        if (rightMove){
            setPixmap(QPixmap(":/sounds/pigletrunright_glow.png"));
        }
        else if (leftMove){
            setPixmap(QPixmap(":/sounds/pigletrunleft_glow.png"));
        }
    }

}

void Player::update_player_in_honey_state()
{
    if (g->game_is_over()){
        phs = false;
        update_pixmap();
        return;
    }

    if (g->player_get_honey()){
        phs = true;
        g->honey->set_honey_end_projected_score(g->score->getScore() + 20);
        tmp_styv = g->stair_Y_velocity;
        g->stair_Y_velocity = 50;
        g->score->setDefaultTextColor(Qt::darkYellow);
        update_pixmap();

        g->musiclist->setCurrentIndex(STA);
        g->musiclist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        g->musicplayer->play();
    }

    if (phs == true && g->score->getScore() >= g->honey->get_honey_end_projected_score()){
        phs = false;
        g->stair_Y_velocity = tmp_styv;
        g->score->setDefaultTextColor(Qt::darkGreen);
        update_pixmap();

        g->musiclist->setCurrentIndex(THM);
        g->musiclist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        g->musicplayer->play();
    }

}

void Player::keep_player_in_focus()
{
    setFocus();
}
