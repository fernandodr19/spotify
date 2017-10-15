#include "tracksview.h"
#include <QGridLayout>
#include <QLabel>
#include <QTableWidget>
#include "playlist.h"
#include "track.h"
#include <QDesktopServices>
#include <QPushButton>
#include <QUrl>
#include <QDebug>

TracksView::TracksView(User user, QWidget *parent) :
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

    gridLayout->addWidget(new QLabel("Tracks"), 0, 0);

    m_tracks = new QTableWidget(0, 6);
    gridLayout->addWidget(m_tracks, 1, 0);
    m_tracks->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_tracks->setHorizontalHeaderLabels({"Name", "Duration", "Popularity", "Added at", "Playlist", "Preview"});

    int row = 0;
//    m_tracks->setRowCount(1);
    for(Playlist playlist : m_user.getPlaylists()) {
        for(Track track : playlist.getTracks()) {
            int col = 0;

            qDebug() << "track";
            m_tracks->insertRow(row);
            QTableWidgetItem *nameItem = new QTableWidgetItem(track.getName());
            nameItem->setData(Qt::UserRole, track.getId());
            nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
            m_tracks->setItem(row, col++, nameItem);

            QTableWidgetItem *durationItem = new QTableWidgetItem(QString::number(track.getDuration()));
            durationItem->setFlags(durationItem->flags() & ~Qt::ItemIsEditable);
            m_tracks->setItem(row, col++, durationItem);

            QTableWidgetItem *popularityItem = new QTableWidgetItem(QString::number(track.getPopularity()));
            popularityItem->setFlags(popularityItem->flags() & ~Qt::ItemIsEditable);
            m_tracks->setItem(row, col++, popularityItem);

            QTableWidgetItem *addedAtItem = new QTableWidgetItem(track.getAddedAt());
            addedAtItem->setFlags(addedAtItem->flags() & ~Qt::ItemIsEditable);
            m_tracks->setItem(row, col++, addedAtItem);

            QTableWidgetItem *playlistItem = new QTableWidgetItem(playlist.getName());
            playlistItem->setFlags(playlistItem->flags() & ~Qt::ItemIsEditable);
            m_tracks->setItem(row, col++, playlistItem);

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
            m_tracks->setCellWidget(row, col++, widget);
        }
    }
}
