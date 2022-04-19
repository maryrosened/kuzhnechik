# Кузнечик 
Разработка прикладное ПО для зашифрования/расшифрования файлов с использованием алгоритма ГОСТ Р 34.13—2015 в режиме гаммирования с обратной связью по шифртексту. 

# Задание 
Разработать прикладное ПО для зашифрования/расшифрования файлов с использованием алгоритма ГОСТ Р 34.13—2015 в режиме гаммирования с обратной связью по шифртексту. Реализацию алгоритма можно заимствовать. ПО должно иметь графический пользовательский интерфейс (GUI) для указания зашифровываемых/расшифровываемых файлов, файла с ключом шифрования и каталога для сохранения зашифрованных/расшифрованных файлов. В качестве значений синхропосылки использовать значения полученные с помощью любого генератора псевдослучайной последовательности (например rand()). При разработке использовать язык программирования С++ в ООП стиле. Для создания GUI использовать библиотеку классов Qt. В приложении эталонная реализация криптоалгоритма. 

# Реализация:
За реализацию был взята [статья](https://habr.com/ru/post/313932/).

Реализация имеет следующие возможности:
* Алгоритм блочного шифрования Kuznyechik с длиной блока 128 бит и длиной ключа 256 бит
* Режим шифрования с обратной связью по шифротексту
