## Отчет

- `task03` - утилита из условия

 - `test_contrast.cpp` (тагрет - `task03_test_contrast`) - запуск обоих вариантов алгоритма и построение гистограмм для данного изображения.
(кладет сгенерированные изображения в папку `testing/`. В названии файлов содержится: вид использованного алгоритма, использованные квантили, является ли изображение гистограммой и для какого канала)

В рамках тестов были использованы следующие изображения (находятся в `testing/originals`):

<img src="./testing/originals/gray.png" height="150"/>
<img src="./testing/originals/bad_1.jpg" height="150"/>
<img src="./testing/originals/bad_2.jpg" height="150"/>
<img src="./testing/originals/good.jpg" height="150"/>

---

### Серые изображения (функция `autocontrast`)

Описание алгоритма:

- Строится гистограмма яркостей всего изображения.
- Находятся пороговые уровни интенсивности, соответствующие квантилиям q_black (нижний) и q_white (верхний).
- Все значения ниже нижнего обрезаются в 0, выше верхнего — в 255.
= Остальные пиксели линейно масштабируются из диапазона [black_thresh…white_thresh] в полный [0…255], повышая контраст.

Прогонки алгоритма:

|  Квантиль черного | Квантиль белого | Результат | Гистограмма |
|---|---|---|---|
| Исходное изображение | - | <img src="./testing/originals/gray.png" height="180"/> | <img src="./testing/hist_original_gray.png" height="180"/>                |
| 0.05 | 0.95 | <img src="./testing/autocont_5-95_gray.png" height="180"/> | <img src="./testing/hist_autocont_mono_5-95_gray.png" height="180"/>                |
| 0.2 | 0.8 | <img src="./testing/autocont_20-80_gray.png" height="180"/> | <img src="./testing/hist_autocont_mono_20-80_gray.png" height="180"/> |
| 0.05 | 0.7 | <img src="./testing/autocont_5-70_gray.png" height="180"/> | <img src="./testing/hist_autocont_mono_5-70_gray.png" height="180"/> |
| 0.3 | 0.95 | <img src="./testing/autocont_30-95_gray.png" height="180"/> | <img src="./testing/hist_autocont_mono_30-95_gray.png" height="180"/> |

Заметим, что у исходного изображения распределение яркостей было примерно равномерным между всеми значениями. При автоконтрастировании большая часть пикселей становится белыми/черными. Изменение квантилей (увеличение q_black и уменьшение q_white) ведет к увеличению числа соответствующих пикселей.

---

### Цветные изображения (наивный подход)

Пример хорошего результата:

|  Квантиль черного | Квантиль белого | Результат | Гистограмма (для зеленого цвета) |
|---|---|---|---|
| Исходное изображение | - | <img src="./testing/originals/good.jpg" height="180"/> | <img src="./testing/hist_green_original_good.jpg" height="180"/>                |
| 0.05 | 0.95 | <img src="./testing/autocont_5-95_good.jpg" height="180"/> | <img src="./testing/hist_green_autocont_mono_5-95_good.jpg" height="180"/>                |

В результате получили контрастную картинку, без "негативных эффектов"

Примеры плохих результатов:

|  Квантиль черного | Квантиль белого | Результат | Гистограмма (для синего цвета) |
|---|---|---|---|
| Исходное изображение | - | <img src="./testing/originals/bad_1.jpg" height="180"/> | <img src="./testing/hist_blue_original_bad_1.jpg" height="180"/>                |
| 0.05 | 0.95 | <img src="./testing/autocont_5-95_bad_1.jpg" height="180"/> | <img src="./testing/hist_blue_autocont_mono_5-95_bad_1.jpg" height="180"/>                |

(при автоконтрастировании исходного изображения появилось слишком много синего цвета) <br><br>



|  Квантиль черного | Квантиль белого | Результат | Гистограмма (для красного цвета) |
|---|---|---|---|
| Исходное изображение | - | <img src="./testing/originals/bad_2.jpg" height="180"/> | <img src="./testing/hist_red_original_bad_2.jpg" height="180"/>                |
| 0.05 | 0.95 | <img src="./testing/autocont_5-95_bad_2.jpg" height="180"/> | <img src="./testing/hist_red_autocont_mono_5-95_bad_2.jpg" height="180"/>                |

(т.к. красного было мало, то после контрастирования его стало слишком много) <br><br>

---

### Ограниченное автоконтрастирование цветных изображений

Принцип работы функции:

- Собирается общая гистограмма всех трёх каналов (B, G, R) вместе.
-  По квантилиям q_black/q_white из этой объединённой гистограммы вычисляются единые пороги чёрного и белого.
- Для каждого канала отдельно применяют линейное растяжение: всё ниже нижнего порога → 0, выше верхнего → 255, а значения между → линейно в [0…255].
- В результате сохраняется цветовой баланс, а контраст усиливается согласовано по всем каналам.

Примеры:

| Алгоритм |  Квантиль черного | Квантиль белого | Результат | Гистограмма (для синего цвета) |
|---|---|---|---|---|
| Исходное изображение | - | - | <img src="./testing/originals/good.jpg" height="180"/> | <img src="./testing/hist_blue_original_good.jpg" height="180"/>                |
| Наивный | 0.05 | 0.95 | <img src="./testing/autocont_5-95_good.jpg" height="180"/> | <img src="./testing/hist_blue_autocont_mono_5-95_good.jpg" height="180"/>                |
| RGB | 0.05 | 0.95 | <img src="./testing/autocont_colored_5-95_good.jpg" height="180"/> | <img src="./testing/hist_blue_autocont_colored_5-95_good.jpg" height="180"/>                |

(можно заметить, что результат немного улучшился - меньше "лишнего" синего) <br><br>

| Алгоритм |  Квантиль черного | Квантиль белого | Результат | Гистограмма (для синего цвета) |
|---|---|---|---|---|
| Исходное изображение | - | - | <img src="./testing/originals/bad_1.jpg" height="180"/> | <img src="./testing/hist_blue_original_bad_1.jpg" height="180"/>                |
| Наивный | 0.05 | 0.95 | <img src="./testing/autocont_5-95_bad_1.jpg" height="180"/> | <img src="./testing/hist_blue_autocont_mono_5-95_bad_1.jpg" height="180"/>                |
| RGB | 0.05 | 0.95 | <img src="./testing/autocont_colored_5-95_bad_1.jpg" height="180"/> | <img src="./testing/hist_blue_autocont_colored_5-95_bad_1.jpg" height="180"/>                |

(Стало заметно лучше, при этом улучшилась и контрастность исходной картинки) <br><br>


| Алгоритм |  Квантиль черного | Квантиль белого | Результат | Гистограмма (для красного цвета) |
|---|---|---|---|---|
| Исходное изображение | - | - | <img src="./testing/originals/bad_2.jpg" height="180"/> | <img src="./testing/hist_red_original_bad_2.jpg" height="180"/>                |
| Наивный | 0.1 | 0.9 | <img src="./testing/autocont_10-90_bad_2.jpg" height="180"/> | <img src="./testing/hist_red_autocont_mono_10-90_bad_2.jpg" height="180"/>                |
| RGB | 0.1 | 0.9 | <img src="./testing/autocont_colored_10-90_bad_2.jpg" height="180"/> | <img src="./testing/hist_red_autocont_colored_10-90_bad_2.jpg" height="180"/>                |

Из гистограмм заметно, что RGB подход лучше наивного учтитывает баланс цветов цветов
