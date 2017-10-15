#ifndef SPOTIFYVIEW_H
#define SPOTIFYVIEW_H

#include "fancytabwidget.h"
#include "user.h"

class SpotifyView : public FancyTabWidget
{
public:
    explicit SpotifyView(User user, QWidget *parent = 0);

private:
    User m_user;
};

#endif // SPOTIFYVIEW_H
