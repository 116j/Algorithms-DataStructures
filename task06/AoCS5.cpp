#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <string>

int database_size; //размер базы данных
int* database; //база данных

sem_t full; //семафор, отображающий, работает ли какой - либо поток
/// <summary>
/// Читатели могут писать в базу данных, только если она не задействована ни у одного писателя или читателя
/// </summary>
/// <param name="param">номер потока - писателя</param>
/// <returns>ничего</returns>
void* Writer(void* param) {
	int pNum = *((int*)param);
	while (1) {
		// ждем, пока все потоки, раьотабщие с базой данных не закончат свою оаботу (пока значение семафора не станет 1)
		sem_wait(&full);
		// после того, как дождались, блокируем доступ для других писателей( уменьшаем значение на 1 (семафор равен 0))
		// вычисляем индекс ячейки и ее новое значение
		int data = rand() % database_size;
		int number = rand() % database_size;
		database[number] = data;
		// освобождаем доступ для других писателей (увеличиваем значение на 1(семафор равен 1))
		sem_post(&full);
		// выводим результат операции
		printf("Writer %d: Writes value = %d to cell [%d]\n", pNum, data, number);
	}
	return nullptr;
}
/// <summary>
/// Читатели всегда могут читать значения из базы данных
/// </summary>
/// <param name="param">номер потока - читателя</param>
/// <returns>ничего</returns>
void* Reader(void* param) {
	int cNum = *((int*)param);
	int result;
	while (1) {
		int number = rand() % database_size;
		if (sem_trywait(&full) == 0)
		{
			// если база данных еще не занята (семафор равен 1), то занимаем ее для блокировки доступа писателем ( уменьшаем значение на 1 (семафор равен 0))
			result = database[number];
			// освобождаем доступ для писателей (увеличиваем значение на 1(семафор равен 1))
			sem_post(&full);
		}
		else
			// если база данных уже занята, то просто обращаемся к ней без взаимодействия с семафором, т.к. нам не нужно ничего ждать
			result = database[number];
		// выводим результат операции
		printf("Reader %d: Reads value = %d from cell [%d]\n", cNum, result, number);
	}
	return nullptr;
}

int main(int argc, char* argv[]) {

	int num_of_threads;
	// инициализируем размер базы данных и количество потоков
	if (argc == 2)
	{
		try
		{
			num_of_threads = std::stoi(argv[1]);
			database_size = std::stoi(argv[2]);
			if (num_of_threads <= 0 || database_size <= 0)
			{
				std::cout << "Number of threads and size of database should be greater than 0" << std::endl;
				return -1;
			}
		}
		catch (...)
		{
			std::cout << "Number of threads and size of database should be integers" << std::endl;
			return -1;
		}
	}
	else
	{
		num_of_threads = 5;
		database_size = 100;
	}

	database = new int[database_size];
	// инициализируем базу данных
	for (int i = 0; i < database_size; i++) {
		database[i] = i;
	}
	//инициализация семафора
	sem_init(&full, 0, 1); //1 - нет работающих потоков

	//массив писателей
	pthread_t* threadW = new pthread_t[num_of_threads];
	// массив номеров писателей
	int* writers = new int[num_of_threads];
	// запускаем писателей
	for (int i = 0; i < num_of_threads; i++) {
		writers[i] = i + 1;
		pthread_create(&threadW[i], nullptr, Writer, (void*)(writers + i));
	}

	//массив читателей
	pthread_t* threadR = new pthread_t[num_of_threads];
	// массив номеров читателей
	int* readers = new int[num_of_threads];
	// запускаем читателей
	for (int i = 0; i < num_of_threads; i++) {
		readers[i] = i + 1;
		pthread_create(&threadR[i], nullptr, Reader, (void*)(readers + i));
	}

	//делаем главный читателем
	int mNum = 0;
	Reader((void*)&mNum);
	// освобождаем данные
	delete[] threadW;
	delete[] writers;
	delete[] threadR;
	delete[] readers;
	return 0;
}


