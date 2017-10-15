#include "musicplayer.h"

MusicPlayer::MusicPlayer(User user) :
    m_user(user)
{
    setWidgetResizable(true);
    setMinimumWidth(800);
    setMinimumHeight(600);
    setFrameShape(QFrame::NoFrame);

    QWidget *proxyWidget = new QWidget;
    setWidget(proxyWidget);

    m_gridLayout = new QGridLayout;
    proxyWidget->setLayout(m_gridLayout);


    m_gridLayout->addWidget(new QLabel("Spotify"),0,0);
}
