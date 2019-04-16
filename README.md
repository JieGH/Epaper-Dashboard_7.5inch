# Epaper-Dashboard_7.5inch
A epaper dashboard based on Waveshare 7.5 inch epaper display and ESP8266 to display news headlines and weather

Version 0.1 

- Issues: 1) Poorly RAM managenment, code is poorly wrriten 2) Most of the Code has no comments 
- New Issues: news API fetching fail since 04/2019

This project received great help from Kevin Darrah https://www.youtube.com/watch?v=0IuqL-4UftA&t=233s http://www.kevindarrah.com/wiki/index.php?title=EPaperBoard

The library used can be found in Relevant_documents folder

The ESP8266 will fetch the weather information and news headlines from internet by accessing metoffice weather api https://www.metoffice.gov.uk/datapoint and https://newsapi.org/ api. Then the information will display by the 7.5 inch waveshare eink diaplay. 

The outcome of the set up is shown in below
![overview](https://raw.githubusercontent.com/JieGH/Epaper-Dashboard_7.5inch/master/pics/20190225_203022.png)

The wiring of this setup is simply using SPI by a ribben cable attached on a waveshare esp8266 eink driver board https://www.waveshare.com/wiki/E-Paper_ESP8266_Driver_Board
