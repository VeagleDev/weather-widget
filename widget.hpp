/*
* Mysterious Developers © 2022
* Language : C++
* Author : pierrbt
* Name : WeathGet (weather-widget)
*/

#ifndef WIDGET_H
#define WIDGET_H


#include <QWidget>
#include "QLineEdit"
#include "QNetworkReply"
#include "QStringList"
#include "QLabel"
#include "QLabel"
#include "QVBoxLayout"
#include "QHBoxLayout"
#include "QDir"
#include "QPixmap"
#include "QDialog"
#include "QUrl"
#include "QUrlQuery"
#include "QVariant"
#include "QString"
#include "QFile"
#include <limits>
#include "QTime"
#include "QMessageBox"
#include "QCompleter"
#include "QStringList"
#include <string>
#include "QTimer"
#include "QNetworkAccessManager"
#include "QNetworkRequest"
#include "QNetworkReply"
#include "QPushButton"

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
        void seeMoreInformations();
        QString lookForICAO(QString nameOfCity, bool updateDate);
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

        QString airportName = "";
        QString airportIATA = "";

        QString latitude = "";
        QString longitude = "";
        QString country = "";

        QString cityName = "Nantes";

        QString stationCode = "LFRS";

        QString correctTS(QString input);

        QNetworkAccessManager *manager = new QNetworkAccessManager(this);

          QStringList cityList;
          QCompleter * completer;



};




#endif // WIDGET_H
