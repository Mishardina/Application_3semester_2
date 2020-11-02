#include <iostream>
#include <string>
#include <fstream>
#include <conio.h>
#include <vector>
#include <stdio.h>
#include <iomanip>

void SeekInfo();


int main() {
	setlocale(LC_ALL, "Russian");

	SeekInfo();

	std::cout << "\nПрограмма завершила работу\n";
}

void SeekInfo() {
	std::string inpDiscFile; 
	std::string inpPerfFile;

	struct str_music_disc {    //структура, содержащая данные о дисках
		char album[255];       //альбом
		char singer[255];      //исполнитель
		char year[5];          //год выпуска альбома
	} disc;

	struct str_music_performer {  //структура, содержащая данные об исполнителях
		char singer[255];         //исполнитель
		char country[255];        //страна происхождения
		char style[255];          //музыкальный стиль
	} perf;

	bool isInp = true;  //флаг для бесконечного цикла

	while (isInp == true) {

		std::vector<str_music_disc> discVec; //вектор дисков

		std::vector<str_music_performer> perfVec; //вектор исполнителей

		std::cout << "Введите по очереди названия файлов для открытия\n";

		std::getline(std::cin, inpDiscFile);

		std::getline(std::cin, inpPerfFile);


		std::ifstream discFile(inpDiscFile + ".bin", std::ios::binary);
		std::ifstream perfFile(inpPerfFile + ".bin", std::ios::binary);

		if (!discFile || !perfFile) {  //проверка на корректность открытия файлов
			bool isOpen = false;

			while (isOpen == false) {
				std::cout << "\nНе удалось открыть файл, повторите ввод!\n";

				discFile.close();
				perfFile.close();

				std::getline(std::cin, inpDiscFile);
				std::getline(std::cin, inpPerfFile);


				discFile.open(inpDiscFile + ".bin", std::ios::binary);
				perfFile.open(inpPerfFile + ".bin", std::ios::binary);
				
				if (discFile && perfFile) { isOpen = true; }

			}
		}

		while (!discFile.eof()) {       //парсинг файлов в векторы
			discFile.read((char*)& disc, sizeof(disc));
			discVec.push_back(disc);
		}
		discFile.close();
		discVec.pop_back();

		while (!perfFile.eof()) {
			perfFile.read((char*)& perf, sizeof(perf));
			perfVec.push_back(perf);
		}
		perfFile.close();
		perfVec.pop_back();

		bool isInfo = false;   //изначально считаем, что информации в файлах нет


		std::cout << "\nНажмите 1, если хотите получить информацию выборочно на экран\n\nНажмите 2, если хотите получить по запросу полную сгруппированную информацию из обоих файлов в виде файла. Файл будет перезаписан!\n";

		char key;      //выбор режима получения информации
		key = _getch(); //защита от случайного нажатия другой клавиши
		if (!(key == 49) || !(key == 50)) {
			while (!(key == 49) && !(key == 50)) {
				key = _getch();
			}
		}

		if (key == 49) {

			system("cls");

			std::cout << "Введите информацию для поиска\n";

			std::string inp; //запрошенная информация
			std::getline(std::cin, inp);

			std::string buf; //буфер для сравнения C-строк

			std::cout << "Исполнитель" << std::setw(15) << "Альбом" << std::setw(30-strlen("Альбом")) << "Год выпуска" << std::setw(27 - strlen("Год выпуска")) << "Стиль" << std::setw(28 - strlen("Стиль")) << "Страна" << '\n';

			for (unsigned int i = 0; i < discVec.size(); i++) {

				buf = discVec[i].singer;

				for (unsigned int j = 0; j < perfVec.size(); j++) {

					if ((discVec[i].album == inp || discVec[i].singer == inp || discVec[i].year == inp || perfVec[j].singer == inp || perfVec[j].country == inp || perfVec[j].style == inp) && (buf == perfVec[j].singer)) {

						isInfo = true; //информация нашлась

						std::cout << discVec[i].singer << std::setw(25-strlen(discVec[i].singer)) << discVec[i].album << std::setw(25 - strlen(discVec[i].album)) << discVec[i].year << std::setw(25 - strlen(discVec[i].year)) << perfVec[j].style << std::setw(25 - strlen(perfVec[i].style)) << perfVec[j].country <<'\n';

					}

				}

				
			}

			if (isInfo == false) {
				std::cout << "\nВведенная информация неверна или ее не существует!\n";
			}
		}
		else if (key == 50) { //получение информации из двух файлов 

			std::string buf;
			std::cout << "Введите название выходного файла\n";
			std::string outName;
			std::cin >> outName;
			outName = outName;
			std::string outcsvName = outName + ".csv";

			std::ofstream out(outName + ".txt");  //файл с итоговой информацией
			std::ofstream outcsv(outcsvName);  //файл с итоговой информацией, подготовленный для парсинга другими программами

			out  << "Исполнитель" << std::setw(15) << "Альбом" << std::setw(30 - strlen("Альбом")) << "Год выпуска" << std::setw(27 - strlen("Год выпуска")) << "Стиль" << std::setw(28 - strlen("Стиль")) << "Страна" << '\n';

			for (int i = 0; i < discVec.size(); i++) {  //перебор векторов

				for (int j = 0; j < perfVec.size(); j++) {

					buf = discVec[i].singer;

					if (perfVec[j].singer == buf) {

						out << discVec[i].singer << std::setw(25 - strlen(discVec[i].singer)) << discVec[i].album << std::setw(25 - strlen(discVec[i].album)) << discVec[i].year << std::setw(25 - strlen(discVec[i].year)) << perfVec[j].style << std::setw(25 - strlen(perfVec[i].style)) << perfVec[j].country << '\n';
						outcsv << discVec[i].singer << ';' << discVec[i].album << ';' << discVec[i].year << ';' << perfVec[j].style << ';' << perfVec[j].country << ';' <<  '\n';

						isInfo = true; //информация нашлась

					}
				}
			}
			if (isInfo == false) { 
				std::cout << "Ошибка при записи файла!\n";
				out << "Введенная информация неверна или ее не существует!\n";
			}
			else{ std::cout << "\nИнформация успешно записана в файл information.txt!\n"; }
		}	
		

		std::cout << "Хотите ли вы продолжить поиск? Y/N\n"; //начать заново или завершить работу
		char ch;
		ch = _getch();
		if ((ch == 78) || (ch == 110)) {
			isInp = false;
		}

		else if ((ch == 89) || (ch == 121)) { isInp = true; }
		
		else { 
			std::cout << "Введите клавишу заново!\n";
			while (!((ch == 78) || (ch == 110)|| (ch == 89) || (ch == 121)))
			{
				ch = _getch();
				std::cout << "Введите клавишу заново!\n";
			}
			if(ch == 78 || ch == 110){ isInp = false; }
			else if ((ch == 89) || (ch == 121)){isInp = true;}
		}
		system("cls");
	}
}