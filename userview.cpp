#include "userview.h"

#include <QGridLayout>
#include <QLabel>
#include <QTableWidget>
#include <QDebug>
#include <QPushButton>
#include <QDesktopServices>
#include <QUrl>

UserView::UserView(User user, QWidget *parent) :
    m_user(user), QScrollArea(parent)
{
    setWidgetResizable(true);
    setFrameShape(QFrame::NoFrame);

    QWidget *proxyWidget = new QWidget;
    setWidget(proxyWidget);

    QVBoxLayout *verticalLayout = new QVBoxLayout;
    proxyWidget->setLayout(verticalLayout);

    QGridLayout *gridLayout = new QGridLayout;
    verticalLayout->addLayout(gridLayout);

    verticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    int row = 0;
    gridLayout->addWidget(new QLabel("User id: " + m_user.getId()), row++, 0);
    gridLayout->addWidget(new QLabel("Country: " + m_user.getCountry()), row++, 0);
    gridLayout->addWidget(new QLabel("Playlists"), row++, 0);

    m_playlists = new QTableWidget(0, 2);
    m_playlists->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_playlists->setHorizontalHeaderLabels({"Name", "Number of tracks"});
    connect(m_playlists, &QTableWidget::cellClicked, [this](int row) {
        m_playlistName->setText(m_playlists->itemAt(row, 0)->text() + "'s tracks");
        m_playlistName->setVisible(true);
        loadPlaylistTracks(m_playlists->itemAt(row, 0)->data(Qt::UserRole).toString());
    });

    m_playlists->blockSignals(true); //this population should be in some 'updateFromProject' method, not in the constructor..
    m_playlists->setRowCount(m_user.getPlaylists().size());
    int playlistRow = 0;
    for(Playlist playlist : m_user.getPlaylists()) {
        int playlistCol = 0;

        QTableWidgetItem *nameItem = new QTableWidgetItem(playlist.getName());
        nameItem->setData(Qt::UserRole, playlist.getId());
        nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
        m_playlists->setItem(playlistRow, playlistCol++, nameItem);

        QTableWidgetItem *nTracksItem = new QTableWidgetItem(QString::number(playlist.getTracks().size()));
        nTracksItem->setFlags(nTracksItem->flags() & ~Qt::ItemIsEditable);
        m_playlists->setItem(playlistRow, playlistCol++, nTracksItem);
    }
    m_playlists->blockSignals(false);

    gridLayout->addWidget(m_playlists, row++, 0);

    m_playlistName = new QLabel();
    m_playlistName->setVisible(false);

    m_playlistTracks = new QTableWidget(0, 5);
    m_playlistTracks->setVisible(false);
    m_playlistTracks->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_playlistTracks->setHorizontalHeaderLabels({"Name", "Duration", "Popularity", "Added at", "Preview"});

    gridLayout->addWidget(m_playlistName, row++, 0);
    gridLayout->addWidget(m_playlistTracks, row++, 0);
}

void UserView::loadPlaylistTracks(const QString &playlistId)
{
    Playlist playlist = m_user.getPlayListById(playlistId);
    if(playlist.getId().isEmpty())
        return;

    int row = 0;
    m_playlistTracks->setRowCount(playlist.getTracks().size());

    for(Track track : playlist.getTracks()) {
        int col = 0;

        QTableWidgetItem *nameItem = new QTableWidgetItem(track.getName());
        nameItem->setData(Qt::UserRole, track.getId());
        nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
        m_playlistTracks->setItem(row, col++, nameItem);

        QTableWidgetItem *durationItem = new QTableWidgetItem(QString::number(track.getDuration()));
        durationItem->setFlags(durationItem->flags() & ~Qt::ItemIsEditable);
        m_playlistTracks->setItem(row, col++, durationItem);

        QTableWidgetItem *popularityItem = new QTableWidgetItem(QString::number(track.getPopularity()));
        popularityItem->setFlags(popularityItem->flags() & ~Qt::ItemIsEditable);
        m_playlistTracks->setItem(row, col++, popularityItem);

        QTableWidgetItem *addedAtItem = new QTableWidgetItem(track.getAddedAt());
        addedAtItem->setFlags(addedAtItem->flags() & ~Qt::ItemIsEditable);
        m_playlistTracks->setItem(row, col++, addedAtItem);

        QWidget* widget = new QWidget();
        QPushButton* playButton = new QPushButton("Play");
        QString previewUrl = track.getPreviewUrl();
        connect(playButton, &QPushButton::clicked, [previewUrl](){
            QDesktopServices::openUrl(QUrl(previewUrl));
        });

        QHBoxLayout* pLayout = new QHBoxLayout(widget);
        pLayout->addWidget(playButton);
        pLayout->setAlignment(Qt::AlignCenter);
        pLayout->setContentsMargins(0, 0, 0, 0);
        widget->setLayout(pLayout);
        m_playlistTracks->setCellWidget(row, col++, widget);
    }
    m_playlistTracks->setVisible(true);
}
