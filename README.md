# weather-widget
 A simple application which search weather informations for the entered city with airport's weather informations.
 I use [METARS](https://aviationweather-cprk.ncep.noaa.gov/adds/dataserver_current/current/metars.cache.csv) weather informations and [airports informations](https://github.com/davidmegginson/ourairports-data) (btw thanks to [@davidmegginson](https://github.com/davidmegginson)).
<hr />

### Here are some pictures of the application : 
<ol>
    <img src="https://github.com/Mysterious-Developers/weather-widget/blob/78ee06931ab0126b622a7380adc2fdd5bfb703d5/img/image0.png">
    <img src="https://github.com/Mysterious-Developers/weather-widget/blob/78ee06931ab0126b622a7380adc2fdd5bfb703d5/img/image1.png">
</ol>

<hr />



### Files

All .hpp files are only header matching with their source code .cpp
Here are the important .cpp files : 

- `main.cpp` : Main file which creates the window and call the version checking
- `tools.cpp` : Set of tools used everywhere in the code
- `ui.cpp` : Code that creates the User Interface
- `update.cpp` : File that totally handles the auto-update of the application
- `widget.cpp` : The most important file that fetch, analyze the METARs, save the informations and search the weather and name information of the airport and displays it


<hr />

### How to install WeathGet ?

* Download the [latest release](https://github.com/mysterious-Developers/weather-widget/releases/latest)
* Extract the .zip archive
* Enter the folder and run WeathGet.exe
* If an warning message appear, just ignore it (don't be frightened, the source code is [here](https://github.com/Mysterious-Developers/weather-widget) and it's just because I'm not approved by Microsoft
* If you have an error, you can report it [here](https://github.com/Mysterious-Developers/weather-widget/issues)

<hr />

### How to contribute to the WeathGet project ?

##### To contribute, there is lot of ways, you can : 

* Report *bugs* or *suggest* improvements on the [issue page](https://github.com/Mysterious-Developers/weather-widget/issues).
* React with the **already existing** [issues](https://github.com/Mysterious-Developers/weather-widget/issues).
* **Share the project** to your friends or people which could be interessed.
* Come in the [discussions](https://github.com/Mysterious-Developers/weather-widget/discussions) to talk about problems, features or others.
* Write *articles* in the open [Wiki](https://github.com/Mysterious-Developers/weather-widget/wiki)
* **Submit pull requests**, for that, you can check the [CONTRIBUTING.md](https://github.com/Mysterious-Developers/weather-widget/blob/main/CONTRIBUTING.md) file.
