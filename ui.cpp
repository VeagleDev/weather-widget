#include "widget.hpp"


Window::Window(QString val)
{
  icon->setPixmap(QPixmap(QDir::currentPath() + "/icon.png").scaled(43,43));

  setBaseSize(245,315);
  setMinimumSize(220, 270);
  setMaximumSize(350,440);
  resize(245,315);

  QHBoxLayout * titleLayout = new QHBoxLayout;
  QHBoxLayout * cityLayout = new QHBoxLayout;
  QVBoxLayout * contentLayout = new QVBoxLayout;
  QVBoxLayout * mainLayout = new QVBoxLayout;
  QPushButton * seeMore = new QPushButton("...");
  seeMore->setFixedSize(20,20);

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
  contentLayout->addWidget(seeMore, Qt::AlignRight);

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

  lookForICAO("Nantes");

  QTimer *timer = new QTimer;
  connect(timer, &QTimer::timeout, this, &Window::getNewInfos);
  timer->singleShot(0, this, &Window::getNewInfos);
  timer->start(1000*60);
}
