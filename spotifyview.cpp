#include "spotifyview.h"
#include "userview.h"
#include "tracksview.h"

SpotifyView::SpotifyView(User user, QWidget *parent) :
    m_user(user), FancyTabWidget(parent)
{
    insertTab(new UserView(m_user, this), tr("My account"), "user");
    insertTab(new TracksView(m_user, this), tr("All tracks"), "tracks");
}
