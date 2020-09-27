# Домашнее задание №2. Вариант 9
Данное домашнее задание представляет из себя написание программы на FASM. Пользователь вводит массив А, после чего программа создает массив В из нечетных элементов массива А и выводит его.

## Тесты

* При корректном вводе программа работает так:

![Image alt](https://github.com/116j/HSE_FCS_SE-ADS/blob/master/task02/images/norm.png)

* При вводе размера массива больше 100 или меньше 1 выводится сообщение об ошибке и завершается программа:

![Image alt](https://github.com/116j/HSE_FCS_SE-ADS/blob/master/task02/images/errorL.png)

![Image alt](https://github.com/116j/HSE_FCS_SE-ADS/blob/master/task02/images/errorG.png)

* При вводе массива, в котором нет нечетных элементов, ничего не выводится:

![Image alt](https://github.com/116j/HSE_FCS_SE-ADS/blob/master/task02/images/none.png)

* При вводе массива больше указанной длины считывается только указаное количество элементов:

![Image alt](https://github.com/116j/HSE_FCS_SE-ADS/blob/master/task02/images/moretS.png)

* При вводе длины массива некорректного формата появляется неопределенное поведение. Программа может расчитать это как какое - то число и либо выдасть ошибку, либо перевести это в число от 1 - 100 (что вряд ли):

![Image alt](https://github.com/116j/HSE_FCS_SE-ADS/blob/master/task02/images/wordsErrorG.png)

![Image alt](https://github.com/116j/HSE_FCS_SE-ADS/blob/master/task02/images/wordsErrorL.png)

* При вводе в консоль программа будет игнорировать пробелы и переводы строки:

![Image alt](https://github.com/116j/HSE_FCS_SE-ADS/blob/master/task02/images/strangeInput.png)

* При вводе массива, если вы запишите неккоректное значение, то вне зависимости от размера массива, в него будут записаны корректно только значения до неккоректного значения, а дальше может пойти неопределенное поведение:

![Image alt](https://github.com/116j/HSE_FCS_SE-ADS/blob/master/task02/images/strangeInput2.png)

![Image alt](https://github.com/116j/HSE_FCS_SE-ADS/blob/master/task02/images/strangeInput3.png)
