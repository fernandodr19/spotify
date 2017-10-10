#include "mainwindow.h"

#include <QDebug>
#include <QtNetworkAuth>
#include <QJsonDocument>
#include <QJsonObject>
#include <QGridLayout>
#include <QLabel>
#include <QPlainTextEdit>


MainWindow::MainWindow(QWidget *parent) :
    QScrollArea(parent),
    isGranted(false)
{
    setWidgetResizable(true);
    setFrameShape(QFrame::NoFrame);

    QWidget *proxyWidget = new QWidget;
    setWidget(proxyWidget);

    m_gridLayout = new QGridLayout;
    proxyWidget->setLayout(m_gridLayout);

    m_gridLayout->addWidget(new QLabel("123"),0,0);
    m_gridLayout->addWidget(new QPlainTextEdit(), 1, 0);

    spotify.setReplyHandler(new QOAuthHttpServerReplyHandler(8080, this));
    spotify.setAuthorizationUrl(QUrl("https://accounts.spotify.com/authorize"));
    spotify.setAccessTokenUrl(QUrl("https://accounts.spotify.com/api/token"));
    spotify.setClientIdentifier("31a72eb6056441a090521075d2166716");
    spotify.setClientIdentifierSharedKey("79552b59f96a4bc5b9234d24618e6b89");
    spotify.setScope("user-read-private user-top-read playlist-read-private playlist-modify-public playlist-modify-private");

    connect(&spotify, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser,
             &QDesktopServices::openUrl);

    connect(&spotify, &QOAuth2AuthorizationCodeFlow::granted,
            this, &MainWindow::granted);

    spotify.grant();//fernandodr19;
}

MainWindow::~MainWindow()
{
}


void MainWindow::granted ()
{
    QString token = spotify.token();
    isGranted = true;
    getUserInfo();
}

void MainWindow::getUserInfo()
{
    QUrl u("https://api.spotify.com/v1/me");

    auto reply = spotify.get(u);

    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << reply->errorString();
            return;
        }
        const auto data = reply->readAll();
        qDebug() << data;

        const auto document = QJsonDocument::fromJson(data);
        const auto root = document.object();
        userName = root.value("id").toString();

        qDebug() << "Username: " << userName;

        reply->deleteLater();
        getPlayLists();
    });
}

void MainWindow::getPlayLists()
{
    if (userName.length() == 0) return;

    QUrl u("https://api.spotify.com/v1/users/" + userName + "/playlists");

    auto reply = spotify.get(u);

    connect (reply, &QNetworkReply::finished, [=]() {
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << reply->errorString();
            return;
        }

        const auto data = reply->readAll();

        const auto document = QJsonDocument::fromJson(data);
        const auto root = document.object();

        qDebug() << data;

        reply->deleteLater();
    });

}
