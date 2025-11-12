# S3-Go!-radio

На создания этой программы меня вдохновил замечательный проект Ёрадио https://4pda.to/stat/go?u=https%3A%2F%2Fgithub.com%2Fe2002%2Fyoradio%2F&e=101867730&f=https%3A%2F%2F4pda.to%2Fforum%2Findex.php%3Fshowtopic%3D1010378%26st%3D1560
Основная цель этого проекта — создание интернет-радио с графическим оформлением (фон фото). Он построен на чипе ESP32S3 N16R8 и экране ST7796(320*480)//другие экраны будут добавляться по мере запроса, библиотека позволяет добавить быстро//
- добавлен 7789--172*320
- добавлен 7789--76*284 (подсветка на землю, мне такой пришел, абсолютно весь инвертированный)

Если проблемы будут (а они как обычно есть), ссылка в телеграм канал https://t.me/radioS3Go

Комплектующие:
- esp32s3 n16r8 44 pin
- https://www.aliexpress.com/item/1005005592730189.html
- dac5102/5100
- https://www.aliexpress.com/item/1005006104038963.html
- ![photo_2025-11-07_15-34-59](https://github.com/user-attachments/assets/2cce3f1e-06b2-4b7a-8eae-dc9c1c50d812)
- https://www.aliexpress.com/item/1005008144198547.html
- пины подключения 5100 (со встроенным унч)
- ![5100](https://github.com/user-attachments/assets/29752642-1eb8-4b01-8574-87e5249ebbde)
- spi tft ST7796
- кнопки, 5 штук.

Характеристики радио:
- количество плейлистов ограничено памятью платы. В плейлисте не более 100 потоков в каждом.
- читает все форматы mp3, aac, flac...
- для потоков Flac больше 1000кбит обновить файлы как написано в инструкции здесь :
https://4pda.to/forum/index.php?showtopic=1010378&view=findpost&p=125839228

Путь к папке для VSC C:\Users\User\.platformio\packages\framework-arduinoespressif32-libs\esp32s3\lib\

Для Ардуино  C:\Users\User\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-33fbade6\esp32s3\lib\
- смена фона, настройка положения и цвета текста с веба без перезагрузки
- настройка размера, цвета, логики движения и положения стрелок вуметра с веба
- загрузка и удаление файлов с веб
- настройка пинов платы, поворот и выбор типа дисплея (может потребоваться коректировка кода) в файле S3_Go_radio\src\config.h
-  управление 5 кнопок(планируется добавить энкодер)


Прошивать с помощью программы VSC.
https://code.visualstudio.com/
Запустить программу, установить PlatformIO IDE.
Установить https://git-scm.com/install/windows

Скачать архив, распаковать в корень диска. Путь к папке (и имя папки) не должен содержать кириллицу.
Открыть папку в программе VSC.
Подождать скачивание библиотек и ядра.
Прошить код.
Прошить файловую систему (data). нажать "upload filesystem image". В папку data можно положить плейлисты и фото. Или же потом с веба загрузить.

<img src="https://github.com/user-attachments/assets/c880c923-1d6f-4cac-8eb3-e780807a8c41" width="25%"/>

При первом запуске подключится к wi-fi платы имя сети "S3-Go!-light-Setup" пароль "12345678", ввести адрес http://192.168.4.1, прописать имя и пароль вашей сети.

<img src="https://github.com/user-attachments/assets/a50fae03-4f8d-49fb-80dc-2f2b9dc3c7cc" width="25%"/>
<img src="https://github.com/user-attachments/assets/7b07264d-4a27-44dd-9381-0d08c0295fd9" width="25%"/>



Библиотеки:
- https://github.com/Bodmer/TJpg_Decoder.git
- https://github.com/schreibfaul1/ESP32-audioI2S/archive/refs/tags/3.4.3.zip
- https://github.com/moononournation/Arduino_GFX.git
- бонус русские шрифты https://github.com/immortalserg/AdafruitGFXRusFonts
Как поменять шрифт: Копируем новый шрифт в папку "src". Прописываем в начале файла main.cpp имя нового шрифта например #include "Bahamas8.h", потом ищем в коде старый шрифт через ctrl+f, и меняем его имя. ВНИМАНИЕ!!! Внутри файла шрифта может быть прописано по другому, "const uint8_t Bahamas8pt8bBitmaps[] PROGMEM = {", значит нам надо взять "Bahamas8pt8b", получится так:
gfx->setFont(&Bahamas8pt8b);


Фото:

https://github.com/user-attachments/assets/048a29b1-905f-432f-9c62-cb85d12a1eee

<img src="https://github.com/user-attachments/assets/a839ecb1-c02e-46b1-a70b-7a8718b49b34" width="25%"/>
<img src="https://github.com/user-attachments/assets/4b673357-a6c2-4fbe-9c64-25f2e437e369" width="25%"/>
<img src="https://github.com/user-attachments/assets/2f54f73e-4fb3-40d6-aec2-c51426016941" width="25%"/>
<img src="https://github.com/user-attachments/assets/8c40a43a-6a98-4bf8-a2d5-7fa698b6132e" width="25%"/>
<img src="https://github.com/user-attachments/assets/a18ccc0e-9de5-4f83-abfb-bbf644187b30" width="25%"/>

Скриншоты веб страницы:

![01](https://github.com/user-attachments/assets/2b8884d7-b283-4f9a-9ffc-d75f902ad585)
![02](https://github.com/user-attachments/assets/bc287604-3835-4c68-a652-606ff465b79f)
![03](https://github.com/user-attachments/assets/bc5a6f4d-8627-41ed-a867-315c0fbf3149)
![04](https://github.com/user-attachments/assets/098a05d3-560c-4181-8db2-37521802417c)
![05](https://github.com/user-attachments/assets/86564563-d4c6-4002-a90e-f54a770f3fcd)
![06](https://github.com/user-attachments/assets/4e6cac1f-0dad-4284-9ab5-52e9590482e1)
Фото на фон создавать соответственно разрешению экрана.
При сохранении фото в фотошопе выбрать настройку "базовый оптимизированный"

![photoshop](https://github.com/user-attachments/assets/e95ff6d8-e2c6-4c6e-b7c7-53681563412b)
