#include "widget.hpp"
#include "QMenu"

Window::Window(QString val, QString version)
{
  airportsDB.setDatabaseName(QDir::currentPath() + "/airports.sqlite3");


  if(!airportsDB.open())
      qDebug() << airportsDB.lastError();

  QSqlQuery * query = new QSqlQuery(airportsDB);
  if(!query->exec("CREATE TEMP TABLE metars ( "
              "id INTEGER PRIMARY KEY AUTOINCREMENT, "
              "icao TEXT,"
              "raw TEXT,"
              "timest TEXT,"
              "temperature REAL,"
              "dewpoint REAL,"
              "winddir  INTEGER,"
              "windspeed REAL,"
              "visibility REAL,"
              "cover TEXT )"))
  {
      qDebug() << "Erreur création table metar";
  }

  qDebug() << "Path : " << QDir::currentPath() + "/airports.sqlite3";

  appVersion = version;
  icon->setPixmap(QPixmap(QDir::currentPath() + "/img/icon.png").scaled(43,43));
  setWindowIcon(QIcon(QPixmap(QDir::currentPath() + "/img/icon.png")));

  connect(updateManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(updateReplyFinished(QNetworkReply*)));
  updateManager->get(QNetworkRequest(QUrl("https://www.mysteriousdev.fr/wg.version")));

  name = new QLabel("Weather <br>&nbsp;&nbsp;<font color=\"#666666\">v" + version + "</font>");

  setBaseSize(330,330);
  setMinimumSize(220, 270);
  setMaximumSize(450,440);
  resize(330,330);

  QHBoxLayout * titleLayout = new QHBoxLayout;
  QHBoxLayout * cityLayout = new QHBoxLayout;
  QVBoxLayout * contentLayout = new QVBoxLayout;
  QHBoxLayout * buttons = new QHBoxLayout;
  QHBoxLayout * covers = new QHBoxLayout;
  QVBoxLayout * mainLayout = new QVBoxLayout;
  QPushButton * license = new QPushButton("WeathGet v" + version);
  license->setStyleSheet("color: #5555FF;\n"
                         "text-decoration: underline;"
                         "border: 0px solid");
  license->setFixedSize(85,20);




  QStringList liste;
  QFile * favorites = new QFile(QDir::currentPath() + "/favorites.txt");
  if(favorites->open(QFile::ReadOnly))
  {
      liste << "";
      while(!favorites->atEnd())
      {
          QString line = QString(favorites->readLine());
          line.remove('\r');
          line.remove('\n');
          if(!line.isEmpty())
              liste << line;
      }
      menu->addItems(liste);
      mainLayout->addWidget(menu);
  }






  titleLayout->addWidget(icon, Qt::AlignRight);
  titleLayout->addWidget(name, Qt::AlignCenter);
  mainLayout->addLayout(titleLayout, Qt::AlignTop);

  city->setFixedSize(200,25);
  cityLayout->addWidget(c_label, Qt::AlignJustify);
  cityLayout->addWidget(city, Qt::AlignLeft);

  contentLayout->addLayout(cityLayout);
  contentLayout->addWidget(station);
  contentLayout->addWidget(date);
  contentLayout->addWidget(temp);
  contentLayout->addWidget(wind_dir);
  contentLayout->addWidget(wind_speed);
  contentLayout->addWidget(visibility);

  covers->addWidget(cover);
  covers->addWidget(coverImage);

  contentLayout->addLayout(covers);

  buttons->addWidget(license);
  contentLayout->addSpacing(5);
  contentLayout->addLayout(buttons);

  station->setOpenExternalLinks(true);
  station->setTextInteractionFlags(Qt::LinksAccessibleByMouse);

  city->setText("Nantes");
  mainLayout->addSpacing(10);
  mainLayout->addLayout(contentLayout, Qt::AlignCenter);






  setLayout(mainLayout);

  connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));


  QObject::connect(city, SIGNAL(editingFinished()), this, SLOT(searchCity()));
  QObject::connect(city, SIGNAL(textChanged(QString)), this, SLOT(textRefresh(QString)));
  QObject::connect(license, SIGNAL(clicked()), this, SLOT(displayLicense()));
  QObject::connect(menu, SIGNAL(currentTextChanged(QString)), this, SLOT(setFavorityCity(QString)));


  manager->get(QNetworkRequest(QUrl("https://aviationweather-cprk.ncep.noaa.gov/adds/dataserver_current/current/metars.cache.csv")));
  lookForICAO("Nantes");

  QTimer *timer = new QTimer;
  connect(timer, &QTimer::timeout, this, &Window::getNewInfos);
  timer->singleShot(0, this, &Window::getNewInfos);
  timer->start(5000*60);
}




void Window::setFavorityCity(QString m_city)
{
    if(m_city != "" && hasLoaded)
    {
        city->setText(m_city);
        cityName = m_city;
        stationCode = lookForICAO(m_city);
        updateInformations();
    }
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


void Window::displayLicense()
{
    QString txt;
    txt = "<h2>WeathGet</h2>\n"
          "<h6>Release v" + appVersion + "</h6>\n"
          "<p>Application open-source (voir <a href =\"https://github.com/mysterious-developers/weather-widget\">projet GitHub</a>)</p>\n"
          "<p>Développé par <a href=\"https://github.com/pierrbt\">@pierrbt</a> et les contributeurs</p>\n"
          "<p>License GPL-3.0</p>";
    QMessageBox::information(this, "Licence", txt);
}
