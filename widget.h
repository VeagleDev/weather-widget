#ifndef WIDGET_H
#define WIDGET_H


#include <QWidget>
#include "QLineEdit"
#include "QNetworkReply"
#include "QLabel"

class Window : public QWidget
{
  Q_OBJECT

public:
        Window(QString val = "0°");
public slots:
        void replyFinished(QNetworkReply *resp);
        void getNewInfos();
        void searchCity();
        void textRefresh(QString newText);
        void findCity(QNetworkReply *resp);
        void seeMoreInformations();
private:
        QLabel * value;

        QLabel * name = new QLabel("Weather");
        QLabel * icon = new QLabel;
        QLabel  * station = new QLabel("Station : - ");
        QLabel  * date = new QLabel("Date : - ");
        QLabel  * temp = new QLabel("Température : ");
        QLabel  * wind_dir = new QLabel("Direction du vent : - ");
        QLabel  * wind_speed = new QLabel("Vitesse du vent : - ");
        QLabel  * visibility = new QLabel("Visibilité : - ");
        QLabel  * cover = new QLabel("Couverture : - ");
        QLineEdit * city = new QLineEdit;
        QLabel * c_label = new QLabel("Ville : ");
        QVector<QString> infos;
        QVector<QString> names;

        QString cityName = "Nantes";

        QString stationCode = "LFRS";

        QString correctTS(QString input);

        QNetworkAccessManager *manager = new QNetworkAccessManager(this);


};


#endif // WIDGET_H
