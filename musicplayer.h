#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include "user.h"
#include "mainwindow.h"

class MusicPlayer : public QScrollArea
{
public:
    MusicPlayer(User user);

private:
    User m_user;

    QGridLayout *m_gridLayout;
};

#endif // MUSICPLAYER_H
