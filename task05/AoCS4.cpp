#include <set>
#include <omp.h>
#include <algorithm>
#include<fstream>

// краткие имена типов для удобства
using set = typename std::set<size_t>;
using It = typename std::set<size_t>::iterator;
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
/// <param name="out">поток, в который пишется множество</param>
void print_set(set set, std::ofstream& out)
{
	out << "{ ";
	for (auto i = set.begin(); i != set.end(); i++)
	{
		out << *i << " ";
	}

	out << "}" << std::endl;
	out << std::endl;
}



int main(int argc, char* argv[])
{
	// открываем файл на запись
	std::ofstream out;
	out.open(argv[2]);
	size_t const threads_num = 4;
	set A;
	set B;
	set C;
	// инициализируем множества
	read_sets(argv[1], A, B, C);
	// пишем полученные множества в файл
	out << "A = ";
	print_set(A, out);
	out << "B = ";
	print_set(B, out);
	out << "C = ";
	print_set(C, out);
	// функции операций над множествами 
	Function functions[threads_num];
	functions[0] = std::set_union<It, It, Inserter>;
	functions[1] = std::set_intersection<It, It, Inserter>;
	functions[2] = std::set_difference<It, It, Inserter>;
	functions[3] = std::set_difference<It, It, Inserter>;
	// имена операций над множествами
	std::string names[threads_num];
	names[0] = "union of A and B";
	names[1] = "intersection of A and B";
	names[2] = "difference between A and B";
	names[3] = "difference between B and A";
	// задаем 4 потока
#pragma omp parallel num_threads(threads_num)
	{
		// номер потока
		auto num = omp_get_thread_num();

		set result;
		// если нужно посчитать разность В и А, то меняем аргументы местами
		if (num == 3)
			functions[num](B.begin(), B.end(), A.begin(), A.end(), std::inserter(result, result.begin()));
		else
			functions[num](A.begin(), A.end(), B.begin(), B.end(), std::inserter(result, result.begin()));
		// критическая секция для записи
#pragma omp critical
		{
			if (C == result)
			{
				out << "C is " << names[num] << std::endl;
			}
			else
			{
				out << "C is not " << names[num] << std::endl;
			}
			out << names[num] << " = ";
			print_set(result, out);
		}
	}

	return 0;
}
