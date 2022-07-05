#include "widget.h"
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
#include "QMessageBox"

#include <string>
#include "QTimer"
#include "QNetworkAccessManager"
#include "QNetworkRequest"
#include "QNetworkReply"
#include "QPushButton"


void Window::replyFinished(QNetworkReply *resp){
  bool ok;
  bool test;
  int code = resp->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(&ok);
  if(code == 200 && ok)
  {
      int i(0), j(0), k(0), l(0), w(0);
      QString toAdd(""), lfrs(""), name("");
      QVector<QString> lines;
      infos.clear();
      names.clear();
      QByteArray buffer = resp->read(20971520);      

      while(i < buffer.size())
        {
          if(QString(buffer[i]) == "\n")
            {
              lines.push_back(toAdd);
              toAdd = "";
            }
          else
              toAdd += QString(buffer[i]);
          i++;
        }

      for(QString k : lines)
        {
          if(k.startsWith(stationCode, Qt::CaseInsensitive))
            {
              qDebug() << "Ligne de " + stationCode +" : " << k;
              lfrs += k;
            }
          if(k.startsWith("raw_text", Qt::CaseSensitive))
              name += k;
        }
       if(lfrs == "")
        {
          station->setText("Station : N/A");
          date->setText("Date : N/A");
          temp->setText("Température : N/A");
          wind_dir->setText("Direction du vent : N/A");
          wind_speed->setText("Vitesse du vent : N/A");
          visibility->setText("Visibilité : N/A");
          cover->setText("Couverture : N/A");
          return;
        }
      toAdd = "";
      while(w < name.length())
        {
          if(name[w] == ',')
            {
              names.push_back(toAdd);
              toAdd = "";
            }
          else
              toAdd += name[w];
          w++;
        }
      names.push_back(toAdd);
      toAdd = "";
      while(j < lfrs.length())
        {
          if(lfrs[j] == ',')
            {
              if(toAdd == "")
                toAdd = "Inconnu";
              infos.push_back(toAdd);
              toAdd = "";
            }
          else
              toAdd += lfrs[j];
          j++;
        }
      if(toAdd == "")
        toAdd = "Inconnu";
      infos.push_back(toAdd);
      toAdd = "";



      station->setText("Station : " + infos[1]);
      date->setText("Date : " + correctTS(infos[2]));
      temp->setText("Température : " + infos[5] + "°C");
      wind_dir->setText("Direction du vent : " + infos[7] + "°");
      wind_speed->setText("Vitesse du vent : " + QString::number(round(infos[8].toDouble()*1.852*10)/10) + " km/h");
      visibility->setText("Visibilité : " + QString::number((round((infos[10].toDouble())*1.609344*10))/10) + " km");
      cover->setText("Couverture : " + ((infos[22] == "CAVOK") ? "Ciel + Visibilité OK" : infos[22]));

  }
  else
      qDebug() << "Erreur de lecture";
}

QString Window::lookForICAO(QString nameOfCity)
{
  QFile * airports = new QFile(QDir::currentPath() + "/airports.csv");
  QVector<QString> lines, infos;
  QString toAdd(""), icao(""), iata(""), name("");

  airports->open(QIODeviceBase::ReadOnly);
  QString content = QString::fromUtf8(((airports->readAll())));
  airports->close();

  for(int i = 0; i < content.size(); i++)
    {
      if(content[i] == '\n')
        {
           lines.push_back(toAdd);
           toAdd = "";
        }
      else
        {
          toAdd += content[i];
        }
    }
  toAdd = "";
  for(int j = 0; j < lines.size(); j++)
    {
      if(lines[j].endsWith(nameOfCity + '\r', Qt::CaseInsensitive))
        {
          for(int k = 0; k < lines[j].size(); k++)
            {
              if(lines[j][k] == ';' || lines[j][k] == '\0' || lines[j][k] == '\n' || lines[j][k] == '\r')
                {
                  infos.push_back(toAdd);
                  toAdd = "";
                }
              else
                {
                  toAdd += lines[j][k];
                }
            }
        }
    }
  if(infos.size() < 9)
    {
      qDebug() << "Infos non collectée (" << infos.size() << ")";
      return "Error";
    }
  else if(infos.size() > 9)
    {
      int factor = infos.size() / 10;
      qDebug() << factor << " aéroports trouvés (" << infos.size() << " infos)";
      for(QString m : infos)
        {
          qDebug() << m;
        }

      bool hasFound = false;
      for(int l = 0; l < factor; l++)
        {
          if(infos[0+(l*10)].startsWith("LF"))
            {
              icao = infos[0+(l*10)];
              iata = infos[1+(l*10)];
              name = infos[2+(l*10)];
              hasFound = true;
            }
        }
      if(!hasFound)
        {
          icao = infos[0];
          iata = infos[1];
          name = infos[2];
        }
      qDebug() << "Aéroport de " << nameOfCity << " (" << name << ") - ICAO : " << icao << " - IATA : " << iata;
      return icao;
    }
  else
    {
      icao = infos[0];
      iata = infos[1];
      name = infos[2];
      qDebug() << "Aéroport de " << nameOfCity << " (" << name << ") - ICAO : " << icao << " - IATA : " << iata;
      return icao;
    }


}

QString Window::correctTS(QString input)
{
  input = input.remove(input.size()-4, 4);

      QString year = input.sliced(0, 4);

      QString month = input.sliced(5,2);
      QString day = input.sliced(8,2);



      input = input.remove(0, 11);
      QString time = QString::number(((input.sliced(0,2).toInt()+2) > 24 ? input.sliced(0,2).toInt()+2-24 : input.sliced(0,2).toInt()+2)) + input.sliced(2,input.size()-2);
      qDebug() << year << month << day << time;
      QDate *messDate = new QDate;
      messDate->setDate(year.toInt(), month.toInt(), day.toInt());
      if(messDate->toString() == QDate::currentDate().toString())
          input = "Aujourd'hui à " + time;
      else if((messDate->year() == QDate::currentDate().year())   &&
              (messDate->month() == QDate::currentDate().month()) &&
              (messDate->day() == ((QDate::currentDate().day())-1)))
                  input = "Hier à " + time;
      else
           input = "Le " + day + "/" + month + "/" + year + " à " + time;

      return input;
}

Window::Window(QString val)
{



  icon->setPixmap(QPixmap(QDir::currentPath() + "/icon.png").scaled(43,43));



  setBaseSize(200,120);

  QHBoxLayout * titleLayout = new QHBoxLayout;
  QHBoxLayout * cityLayout = new QHBoxLayout;
  QVBoxLayout * contentLayout = new QVBoxLayout;
  QVBoxLayout * mainLayout = new QVBoxLayout;
  QPushButton * seeMore = new QPushButton("...");
  seeMore->setFixedSize(20,20);

  titleLayout->addWidget(icon, Qt::AlignRight);
  titleLayout->addWidget(name, Qt::AlignCenter);

  mainLayout->addLayout(titleLayout, Qt::AlignTop);


  city->setFixedSize(100,20);
  cityLayout->addWidget(c_label, Qt::AlignJustify);
  cityLayout->addWidget(city, Qt::AlignLeft);

  contentLayout->addLayout(cityLayout);

  contentLayout->addWidget(station);
  contentLayout->addWidget(date);
  contentLayout->addWidget(temp);
  contentLayout->addWidget(wind_dir);
  contentLayout->addWidget(wind_speed);
  contentLayout->addWidget(visibility);
  contentLayout->addWidget(cover);
  contentLayout->addWidget(seeMore, Qt::AlignRight);

  city->setText("Nantes");



  mainLayout->addSpacing(10);
  mainLayout->addLayout(contentLayout, Qt::AlignCenter);

  setLayout(mainLayout);


  connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
  QObject::connect(city, SIGNAL(editingFinished()), this, SLOT(searchCity()));
  QObject::connect(city, SIGNAL(textEdited(QString)), this, SLOT(textRefresh(QString)));
  QObject::connect(seeMore, SIGNAL(clicked()), this, SLOT(seeMoreInformations()));

  lookForICAO("Nantes");

  QTimer *timer = new QTimer;
  connect(timer, &QTimer::timeout, this, &Window::getNewInfos);
  timer->singleShot(0, this, &Window::getNewInfos);
  timer->start(1000*60);


}

void Window::seeMoreInformations()
{
  QString txt;
  int index = 1;
  for(int i = 0; i < infos.size(); ++i)
    {
    if(infos[i] != "Inconnu" && infos[i] != "")
      {
      txt += "Info n°" + QString::number(index) + " (" + names[i] + ") : " + infos[i] + "\n";
      ++index;
      }
    }
  QMessageBox::information(this, "Informations complètes", txt);
}

void Window::textRefresh(QString newText)
{
  cityName = newText;
}
void Window::searchCity()
{
  if(cityName == "")
    return;
  stationCode = lookForICAO(cityName);
  getNewInfos();
}

void Window::getNewInfos()
{
  manager->get(QNetworkRequest(QUrl("https://aviationweather-cprk.ncep.noaa.gov/adds/dataserver_current/current/metars.cache.csv")));
}
