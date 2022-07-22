#include "widget.hpp"


Window::Window(QString val, QString version)
{
  appVersion = version;
  icon->setPixmap(QPixmap(QDir::currentPath() + "/img/icon.png").scaled(43,43));
  setWindowIcon(QIcon(QPixmap(QDir::currentPath() + "/img/icon.png")));

  name = new QLabel("Weather <br>&nbsp;&nbsp;<font color=\"#666666\">v" + version + "</font>");

  setBaseSize(255,330
              );
  setMinimumSize(220, 270);
  setMaximumSize(400,440);
  resize(255,330);

  QHBoxLayout * titleLayout = new QHBoxLayout;
  QHBoxLayout * cityLayout = new QHBoxLayout;
  QVBoxLayout * contentLayout = new QVBoxLayout;
  QHBoxLayout * buttons = new QHBoxLayout;
  QVBoxLayout * mainLayout = new QVBoxLayout;
  QPushButton * seeMore = new QPushButton("Info. brutes");
  QPushButton * license = new QPushButton("WeathGet v" + version);
  license->setStyleSheet("color: #5555FF;\n"
                         "text-decoration: underline;"
                         "border: 0px solid");
  license->setFixedSize(85,20);

  seeMore->setFixedSize(75,20);

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
  contentLayout->addWidget(cover);

  buttons->addWidget(seeMore);
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
  QObject::connect(seeMore, SIGNAL(clicked()), this, SLOT(seeMoreInformations()));
  QObject::connect(license, SIGNAL(clicked()), this, SLOT(displayLicense()));

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
