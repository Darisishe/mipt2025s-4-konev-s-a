## Отчет по Задаче 1

Был написан код для генерации изображений (`samples_generator.cpp`) со всеми возможными комбинациями из: типов (`[CV_8U, CV_8S, CV_16U, CV_16S, CV_32S, CV_32F, CV_64F]`), каналов (`[1, 3, 4]`) и расширений (`[jpg, png, tiff]`). Сгенерированные картинки попадают в `task01/samples`. Для сгенерированных изображений был получен следующий протокол соответствия имен файлов формату (прогон таргета `task01` - на вход программе можно передать путь до директории с картинками. По умолчанию это `task01/samples`):
```
0087x0083.4.uint08.jpg  bad, should be 0087x0083.3.uint08
0087x0083.4.uint08.png  good
0087x0083.4.uint08.tiff good
0017x0071.3.uint08.jpg  good
0017x0071.3.uint08.png  good
0017x0071.3.uint08.tiff good
0088x0056.1.uint08.jpg  good
0088x0056.1.uint08.png  good
0088x0056.1.uint08.tiff good
0025x0064.4.sint08.jpg  bad, should be 0025x0064.3.uint08
0025x0064.4.sint08.png  bad, should be 0025x0064.4.uint08
0025x0064.4.sint08.tiff bad, should be 0025x0064.4.uint08
0042x0076.3.sint08.jpg  bad, should be 0042x0076.3.uint08
0042x0076.3.sint08.png  bad, should be 0042x0076.3.uint08
0042x0076.3.sint08.tiff bad, should be 0042x0076.3.uint08
0092x0080.1.sint08.jpg  bad, should be 0092x0080.1.uint08
0092x0080.1.sint08.png  bad, should be 0092x0080.1.uint08
0092x0080.1.sint08.tiff bad, should be 0092x0080.1.uint08
0086x0053.4.uint16.jpg  bad, should be 0086x0053.3.uint08
0086x0053.4.uint16.png  good
0086x0053.4.uint16.tiff good
0058x0096.3.uint16.jpg  bad, should be 0058x0096.3.uint08
0058x0096.3.uint16.png  good
0058x0096.3.uint16.tiff good
0017x0013.1.uint16.jpg  bad, should be 0017x0013.1.uint08
0017x0013.1.uint16.png  good
0017x0013.1.uint16.tiff good
0069x0039.4.sint16.jpg  bad, should be 0069x0039.3.uint08
0069x0039.4.sint16.png  bad, should be 0069x0039.4.uint08
0069x0039.4.sint16.tiff bad, should be 0069x0039.4.uint08
0089x0058.3.sint16.jpg  bad, should be 0089x0058.3.uint08
0089x0058.3.sint16.png  bad, should be 0089x0058.3.uint08
0089x0058.3.sint16.tiff bad, should be 0089x0058.3.uint08
0054x0078.1.sint16.jpg  bad, should be 0054x0078.1.uint08
0054x0078.1.sint16.png  bad, should be 0054x0078.1.uint08
0054x0078.1.sint16.tiff bad, should be 0054x0078.1.uint08
0081x0024.4.sint32.jpg  bad, should be 0081x0024.3.uint08
0081x0024.4.sint32.png  bad, should be 0081x0024.4.uint08
0081x0024.4.sint32.tiff bad, should be 0081x0024.4.uint08
0055x0073.3.sint32.jpg  bad, should be 0055x0073.3.uint08
0055x0073.3.sint32.png  bad, should be 0055x0073.3.uint08
0055x0073.3.sint32.tiff bad, should be 0055x0073.3.uint08
0067x0096.1.sint32.jpg  bad, should be 0067x0096.1.uint08
0067x0096.1.sint32.png  bad, should be 0067x0096.1.uint08
0067x0096.1.sint32.tiff bad, should be 0067x0096.1.uint08
0012x0017.4.real32.jpg  bad, should be 0012x0017.3.uint08
0012x0017.4.real32.png  bad, should be 0012x0017.4.uint08
0012x0017.4.real32.tiff good
0087x0067.3.real32.jpg  bad, should be 0087x0067.3.uint08
0087x0067.3.real32.png  bad, should be 0087x0067.3.uint08
0087x0067.3.real32.tiff good
0079x0064.1.real32.jpg  bad, should be 0079x0064.1.uint08
0079x0064.1.real32.png  bad, should be 0079x0064.1.uint08
0079x0064.1.real32.tiff good
0020x0088.4.real64.jpg  bad, should be 0020x0088.3.uint08
0020x0088.4.real64.png  bad, should be 0020x0088.4.uint08
0020x0088.4.real64.tiff bad, should be 0000x0000.1.uint08
0073x0093.3.real64.jpg  bad, should be 0073x0093.3.uint08
0073x0093.3.real64.png  bad, should be 0073x0093.3.uint08
0073x0093.3.real64.tiff bad, should be 0000x0000.1.uint08
0051x0061.1.real64.jpg  bad, should be 0051x0061.1.uint08
0051x0061.1.real64.png  bad, should be 0051x0061.1.uint08
0051x0061.1.real64.tiff good
```

Сообщения о несоответствии объясняются следующим: `.jpg` поддерживает только `1`- и `3`-канальный `uint08`, `.png` только `uint08` и `uint16`. При этом `.tiff` по сравнению с `.png` поддерживает `real32`.
