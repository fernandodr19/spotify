#include "spotifyview.h"
#include "userview.h"
#include "playlistview.h"

SpotifyView::SpotifyView(QWidget *parent) :
    FancyTabWidget(parent)
{
    insertTab(new UserView(this), tr("My account"), "user");
    insertTab(new PlaylistView(this), tr("Playlists"), "playlist");
}
