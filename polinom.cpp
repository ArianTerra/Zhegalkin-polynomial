/**
* @brief  Находжение полинома Жегалкина методом треугольника
* @author Артем Середюк
* @date   23.03.2019
* @file   Polinom.cpp
*
* Программа для нахождения полинома Жегалкина методом треугольника
* "Паскаля". Необходимо ввести таблицу истинности функции для N
* количества переменных.
*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
/**
* @brief  Определение корректности ввода целочисленных данных
* @param  min Минимальное допустимое значение для ввода
* @param  max Максимальное допустимое значение для ввода
* @return Корректное int число в пределах min-max
* При неправильном вводе будет запрошен повторный ввод.
*/
int getNumber(int min, int max);
/**
* @brief  Вычисление полинома
* @param  vars		  кол-во переменных
* @param  vector	  массив значений функции
* @param  MAX    	  кол-во строк в ТИ (размер вектора значений ф-ции)
* @param  truth_table таблица истинности в виде матрицы
* @param  LETTER	  набор символов в виде строки
*/
void polinom(int vars, int vector[64], const int MAX, int truth_table[64][6],
	const char* LETTER);
/**
* @brief  Генерация таблицы
* @param  vars		  кол-во переменных
* @param  MAX    	  кол-во строк в ТИ (размер вектора значений ф-ции)
* @param  table		  матрица для записи в нее ТИ
*/
void generateTable(int table[64][6], int vars, int MAX);
/**
* @brief  Парсер вектора значений из аргумента КС
* @param  vars		  кол-во переменных
* @return 0 или 1. -1 если некорректное число/символ (не 0 или 1)
*/
int parseInt(char c);
/**
* @brief  Плашка с названием программы
*/
void splash();

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "Rus");

	int vars;									 //<Количество переменных
	int truth_table[64][6];						 //<Таблица истинности
	int vector[64];								 //<Вектор функции
	const char LETTER[] = "ABCDEF";				 //<Набор переменных

	if (argc <= 1) {
		splash();

		printf("Введите количество переменных (2-6): ");
		vars = getNumber(2, 6);					     //<Получаем кол-во переменных
		const int MAX_TABLE = (int)pow(2, vars);     //<Константа макс. кол-ва строк в ТИ

		generateTable(truth_table, (int)vars, MAX_TABLE); //<Генерация Таблицы Истинности

		//!Заполняем вектор
		printf("Таблица истинности: \n N  ");
		for (int i = 0; i < vars; i++)
			printf("%c", LETTER[i]);
		printf("   F\n");
		for (int i = 0; i < MAX_TABLE; i++) {
			printf("%2i| ", i);
			for (int j = 0; j < vars; j++) {		 //<Вывод ТИ
				printf("%i", truth_table[i][j]);
			}
			printf(" = ");
			vector[i] = getNumber(0, 1);		 //<Записываем вектор функции
		}

		polinom((int)vars, vector, MAX_TABLE, truth_table, LETTER);
	}
	else if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "-?")
		|| !strcmp(argv[1], "--help")) {
		splash();
		//Вывод помощи
		printf(
			"Запустите программу в обычном режиме и введите значения для\n"
			"вектора булевой функции.\n"
			"Запустите программу из командной строки, в качестве аргумента\n"
			"выбрав вектор функции. Пример: Polinom.exe 1100\n"
		);
	}
	else {
		//Запуск с аргументом
		splash();

		const int MAX_TABLE = strlen(argv[1]);			//<Получаем длинну аргумента
		double temp = log2(MAX_TABLE);					//<Узнаем кол-во аргументов

		if ((int)temp != temp || (temp < 2 || temp>6)) {
			printf("\aОшибка: Неверный размер вектора булевой функции.");
			return 1;
		}
		vars = (int)temp;
		for (int i = 0; i < MAX_TABLE; i++) {
			vector[i] = parseInt(argv[1][i]);
			if (vector[i] == -1) {
				printf("\aОшибка: Некорректный ввод вектора булевой функции.");
				return 1;
			}
		}

		generateTable(truth_table, vars, MAX_TABLE);

		printf("Таблица истинности: \n N  ");
		for (int i = 0; i < vars; i++)
			printf("%c", LETTER[i]);
		printf("   F\n");
		for (int i = 0; i < MAX_TABLE; i++) {
			printf("%2i| ", i);
			for (int j = 0; j < vars; j++) {		 //<Вывод ТИ
				printf("%i", truth_table[i][j]);
			}
			printf(" = %i\n", vector[i]);
		}

		polinom(vars, vector, MAX_TABLE, truth_table, LETTER);

	}
}
void polinom(int vars, int vector[64], const int MAX, int truth_table[64][6],
	const char* LETTER) {
	/**
	* Заполнение и вывод треугольника "Паскаля"
	* Для вычисления полинома методом треугольника запишем
	* функцию в векторном виде (напр. 0110). Далее на след.
	* строке первый эл-т получим как сумму по модулю два
	* первого и следующего эл-та первой строки. Выполняем это
	* до тех пор, пока не останется один эл-т в строке.
	*
	*/
	int trinagle[64][64];						 //<Треугольник Паскаля

	printf("Расчет методом треугольника: \n");
	for (int i = 0; i < MAX; i++) {
		if (vector[i] > 1 || vector[i] < 0) {
			printf("\aОшибка преобразования вектора");
			exit(EXIT_FAILURE);
		}
		trinagle[0][i] = vector[i];
		printf("%i", trinagle[0][i]);
	}
	printf("\n");

	int trin_max = MAX - 1;					 //<Кол-во эл-тов в текущей строке

	for (int i = 1; i < MAX; i++) {
		for (int j = 0; j < trin_max; j++) {
			//!Используем XOR для выч-я эл-тов след. строки
			trinagle[i][j] = trinagle[i - 1][j] ^
				trinagle[i - 1][j + 1];
			printf("%i", trinagle[i][j]);
		}
		trin_max--;
		printf("\n");
	}

	/**
	* Вывод полинома Жегалкина
	* В результате вычислений мы получаем матрицу с треугольником.
	* Смотрим на первый столбец, если там 1, то выписываем
	* коньюнкцию соответствующих констант для данной строки.
	* Например, для строки 1 - это 001, т. е. C, для строки 3 - BC
	* для строки 0 это 1.
	*/
	printf("Результат: ");
	int check = 0;								 //<счетчик, если все миноры равны нулю
	if (trinagle[0][0]) printf("1");			 //<Проверка на отрицательность
	for (int i = 0; i < MAX; i++) {
		if (trinagle[i][0]) {					 //<Если 0 эл-т строки 1, то
			for (int j = 0; j < vars; j++) {	 //Перебор переменных
				if (truth_table[i][j]) {		 //Если переменная 1, то вывести ее
					printf("%c", LETTER[j]);
					check++;
				}
			}
			if (i != MAX) printf("+");
		}
	}
	if (!check && !trinagle[0][0]) printf("0 "); //Если все миноры равны нулю
	printf("\b \n");							 //Стирает последний плюс
	system("PAUSE");
}
void generateTable(int table[64][6], int vars, int MAX) {
	for (int i = 0; i < MAX; i++) {
		for (int j = 0; j < vars; j++) {
			table[i][vars - j - 1] =
				((i >> j) & 1) == 1;
		}
	}
}

int getNumber(int min, int max) {
	int check, number;
start:										 //START
	do {
		check = scanf_s("%i", &number);
		while (getchar() != '\n');
		if (check == 1 && number >= min && number
			<= max) goto exit;					 //GOTO EXIT
		else {
			printf("Введите число в правильном диапазоне (%i-%i): ",
				min, max);
			goto start;							 //GOTO START
		}
	} while (check != 1);
exit:										 //EXIT
	return number;
}

int parseInt(char c) {
	c -= '0';
	if (c > 1 || c < 0) return -1;
	return c;
}

void splash() {
	printf(
		"+------------------------------------+\n"
		"| Polinom.exe v1.0 by Artem Serediuk |\n"
		"|   Введите -h --help для справки    |\n"
		"+------------------------------------+\n"
	);
}