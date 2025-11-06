# S3-Go!-radio
Этот код результат работы с замечательным проектом Ёрадио https://4pda.to/stat/go?u=https%3A%2F%2Fgithub.com%2Fe2002%2Fyoradio%2F&e=101867730&f=https%3A%2F%2F4pda.to%2Fforum%2Findex.php%3Fshowtopic%3D1010378%26st%3D1560
Основная цель этого проекта — создание интернет-радио с графическим оформлением (фон фото). Он построен на чипе ESP32S3 N16R8 и экране ST7796(320*480)
Если проблемы будут (а они как обычно есть), ссылка в телеграм канал https://t.me/radioS3Go
Характеристики радио:
- количество плейлистов ограничено памятью платы. В плейлисте не более 100 потоков в каждом.
- читает все форматы mp3, aac, flac...
- Для потоков Flac больше 1000кбит обновить файлы как написано в инструкции здесь 
https://4pda.to/forum/index.php?showtopic=1010378&view=findpost&p=125839228
- смена фона, настройка положения и цвета текста с веба без перезагрузки
- настройка размера, цвета, логики движения и положения стрелок вуметра с веба
- загрузка и удаление файлов с веб
- настройка пинов платы в файле S3_Go_radio\src\config.h
-  выбор дисплея в файле (может потребоваться коректировка кода)

Прошивать с помощью программы VSC.
https://code.visualstudio.com/
Запустить программу, установить PlatformIO IDE.

Скачать архив, распаковать. Путь к папке (и имя папки) не должен содержать кириллицу.
Открыть папку в программе VSC.
Подождать скачивание библиотек и ядра.
Прошить код.
Прошить файловую систему(data).
При первом запуске подключится к wi-fi платы имя сети "S3-Go!-light-Setup" пароль "12345678", прописать имя и пароль вашей сети.
Фото

https://github.com/user-attachments/assets/048a29b1-905f-432f-9c62-cb85d12a1eee


![IMG_20251106_193919](https://github.com/user-attachments/assets/a839ecb1-c02e-46b1-a70b-7a8718b49b34)width="500"
![IMG_20251106_193906](https://github.com/user-attachments/assets/4b673357-a6c2-4fbe-9c64-25f2e437e369)
![IMG_20251106_193535](https://github.com/user-attachments/assets/2f54f73e-4fb3-40d6-aec2-c51426016941)
![IMG_20251106_193506](https://github.com/user-attachments/assets/8c40a43a-6a98-4bf8-a2d5-7fa698b6132e)
![IMG_20251106_193926](https://github.com/user-attachments/assets/a18ccc0e-9de5-4f83-abfb-bbf644187b30)

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
