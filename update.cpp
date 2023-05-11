#include "widget.hpp"


void Window::updateReplyFinished(QNetworkReply *resp){

  bool ok;
  int code = resp->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(&ok);
  try
  {
    if(code == 200 && ok)
    {
          QByteArray buffer = resp->read(20971520);
        float latestVersion = buffer.toFloat(&ok);
        if(ok)
        {
          float currentVersion = appVersion.toFloat(&ok);
          if(ok)
          {
              if(latestVersion > currentVersion)
              {
                  goForUpdate(latestVersion);
              }
              else
              {
                  return;
              }
          }
          else
          {
              throw 1; // Ficher de version invalide
          }
        }
        else
        {
            throw 2; // Dernière version invalide
        }
    }
    else
    {
        throw 3; // Le fichier de la dernière version est indisponible
    }
   }
  catch(const int err)
  {
      QMessageBox msgBox;
      msgBox.setWindowIcon(QIcon(QDir::currentPath() + "/img/icon.png"));
      switch(err)
      {
        case 1:
          msgBox.setText("Le fichier de version est invalide, veuillez retélécharger l'application.");
          msgBox.setStandardButtons(QMessageBox::Ok);
          msgBox.setDefaultButton(QMessageBox::Ok);
          msgBox.exec();
          break;
      case 2:
          msgBox.setText("La dernière version est invalide, veuillez avertir un administrateur.");
          msgBox.setStandardButtons(QMessageBox::Ok);
          msgBox.setDefaultButton(QMessageBox::Ok);
          msgBox.exec();
          break;
      case 3:
          msgBox.setText("Le fichier contenant le tag de la dernière version est inaccessible.");
          msgBox.setStandardButtons(QMessageBox::Ok);
          msgBox.setDefaultButton(QMessageBox::Ok);
          msgBox.exec();
          break;
      }

      return;


  }
  return;

}

void Window::goForUpdate(float newVersion)
{
    QMessageBox msgBox;
    msgBox.setWindowIcon(QIcon(QDir::currentPath() + "/img/icon.png"));
    msgBox.setText("La nouvelle version " + QString::number(newVersion) + " est disponible !");
    msgBox.setInformativeText("Voulez vous l'installer ?");
    msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();
    if(ret == QMessageBox::No)
        return;

    QDesktopServices::openUrl(QUrl::fromLocalFile(QDir::currentPath() + "/update.bat"));
    qApp->exit(0);

}
