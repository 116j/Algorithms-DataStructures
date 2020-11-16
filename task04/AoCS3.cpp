#include <set>
#include <thread>
#include <mutex>
#include <algorithm>
#include <string>
#include<fstream>

std::mutex mutex;
std::ofstream out;
// краткие имена типов для удобства
using set = typename std::set<int>;
using It = typename std::set<int>::iterator;
// итератор для заполнения результата функции
using Inserter = std::insert_iterator<set>;
// тип функции для вычесления множества(пересечение, объединение или разность)
typedef Inserter(*Function)(It, It, It, It, Inserter);

/// <summary>
/// Считываем множества из файла
/// </summary>
/// <param name="path">путь к файлу</param>
/// <param name="A">множество А</param>
/// <param name="B">множество В</param>
/// <param name="C">множество С</param>
void read_sets(std::string path, set& A, set& B, set& C)
{
	std::ifstream in(path);

	if (in.is_open())
	{
		size_t n;
		in >> n;
		for (size_t i = 0; i < n; i++)
		{
			int el;
			in >> el;
			A.insert(el);
		}

		in >> n;
		for (size_t i = 0; i < n; i++)
		{
			int el;
			in >> el;
			B.insert(el);
		}

		in >> n;
		for (size_t i = 0; i < n; i++)
		{
			int el;
			in >> el;
			C.insert(el);
		}
	}
}
/// <summary>
/// Пишем множество в файл
/// </summary>
/// <param name="set">множество для записи</param>
void print_set(set set)
{
	out << "{ ";
	for (auto i = set.begin(); i != set.end(); i++)
	{
		out << *i << " ";
	}

	out << "}" << std::endl;
	out << std::endl;
}
/// <summary>
/// Функция для потока, которая проверяет множества
/// </summary>
/// <param name="f">функция для вычесления множества(пересечение, объединение или разность)</param>
/// <param name="name">название функции(пересечение, объединение или разность)</param>
/// <param name="A">Первое множество</param>
/// <param name="B">Второе множество</param>
/// <param name="C">Требуеиый результат функции</param>
void func(Inserter(*f)(It, It, It, It, Inserter), std::string name, set A, set B, set C)
{
	set result;
	f(A.begin(), A.end(), B.begin(), B.end(), std::inserter(result, result.begin()));
	// для записи блокируем потоки, чтобы ничего не перемешалось
	mutex.lock();
	if (C == result)
	{
		out << "C is " << name << std::endl;
	}
	else
	{
		out << "C is not " << name << std::endl;
	}
	out << name << " = ";
	print_set(result);
	// после записи освобождаем блокировку
	mutex.unlock();

}



int main(int argc, char* argv[])
{
	// открываем файл на запись
	out.open(argv[2]);

	set A;
	set B;
	set C;
	// инициализируем множества
	read_sets(argv[1], A, B, C);
	// пишем полученные множества в файл
	out << "A = ";
	print_set(A);
	out << "B = ";
	print_set(B);
	out << "C = ";
	print_set(C);
	// запускаем потоки с разными функциями 
	std::thread thread_union(static_cast<void(*)(Function, std::string, set, set, set)>(&func),
		static_cast<Function>(&std::set_union<It, It, Inserter>), "union of A and B", A, B, C);
	std::thread thread_intersection(static_cast<void(*)(Function, std::string, set, set, set)>(&func),
		static_cast<Function>(&std::set_intersection<It, It, Inserter>), "intersection of A and B", A, B, C);
	std::thread thread_difference_A(static_cast<void(*)(Function, std::string, set, set, set)>(&func), 
		static_cast<Function>(&std::set_difference<It, It, Inserter>), "difference between A and B", A, B, C);
	std::thread thread_difference_B(static_cast<void(*)(Function, std::string, set, set, set)>(&func), 
		static_cast<Function>(&std::set_difference<It, It, Inserter>), "difference between B and A", B, A, C);
	// ждем потоки
	thread_union.join();
	thread_intersection.join();
	thread_difference_A.join();
	thread_difference_B.join();

	return 0;
}
