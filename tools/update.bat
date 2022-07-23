@ECHO OFF
echo Bienvenue dans l'assistant de mise a jour WeathGet :
echo 1. Nous telechargeons la nouvelle version ...
wget.exe -q --tries=3 --show-progress https://www.mysteriousdev.fr/downloads/weathget_update_latest.zip
move weathget_update_latest.zip ..
cd ..
echo 2. Telechargement termine !
tar -xf weathget_update_latest.zip
echo 3. Extraction terminee !
del weathget_update_latest.zip
echo 4. Mise a jour finie, vous pouvez relancer l'application
PAUSE
