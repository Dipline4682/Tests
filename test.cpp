// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <cstring>
#include <vector>
#include <ctime>

using namespace std;

//=========Базовый класс=======================
class BaseClass {
	double Ox, Oy, Oz;
	char* name;
public:
	// Конструктор класса
	BaseClass(){ Ox = Oy = Oz = 0; }
	//методы класса
	double setOx(double x1) { Ox = x1; }
	double setOy(double y1) { Oy = y1; }
	double setOz(double z1) { Oz = z1; }
	char *getName() { return name; }
	double getOx() { return Ox; }
	double getOy() { return Oy; }
	double getOz() { return Oz; }

	BaseClass(double x1, double y1, double z1, char *name1) {
		Ox = x1;
		Oy = y1;
		Oz = z1;
		name = (char*)name1;
	}
	void show(){ cout << name << "\n"; }
};
//=========================================

//============кривая оружности=============
class Circle : public BaseClass {
	double radius;
public:
	Circle(){ radius = 0.0; }

	Circle(double x1, double y1, double r) :BaseClass(x1, y1, 0.0, "Circle") {
		radius = r;
	}
};
//=====================================================

//====================Кривая элипса===================
class Ellipse : public BaseClass {
	double radius1, radius2;
public:
	Ellipse() { radius1 = radius2 = 0.0; }

	Ellipse(double x1, double y1, double r1, double r2) : BaseClass(x1, y1, 0.0, "Ellipse") {
	radius1 = r1;
	radius2 = r2;
	}
};
//=========================================================

//===============Кривая спирали============================
class Helix : public BaseClass {
	double radius, step;
public:
	Helix() { radius = step = 0.0; }

	Helix(double x, double y, double z, double r, double s) : BaseClass(x, y, z, "Helix") {
	radius = r;
	step = s;
	}
};
//============================================================

struct srandom{ srandom(){ srand(time(NULL)); } }orandom;
int random(int min, int max){
	return min + rand() % (max - min);
}

int main()
{
	Ellipse ellipse;
	Circle circle;
	Helix helix;
	vector<BaseClass> ListObject;
	cout << "Size do zapolneniya: " << ListObject.size() << endl;
	vector<BaseClass> object;
	
	ListObject.push_back(ellipse);
	ListObject.push_back(circle);
	ListObject.push_back(helix);
	
	cout << "Size posle zapolneniya: " << ListObject.size() << endl;

	/*2. Заполните контейнер(например, вектор или список) объектов этих типов, созданных случайным образом, с помощью
		случайные параметры.Убедитесь, что контейнер будет содержать объекты всех поддерживаемых типов.*/
	for (int i = 0; i <= 10; i++){
		int ran = 0 + rand() % 3;
		object.push_back(ListObject[ran]);
		cout << "Size: " <<  << " RUND: " << ran << endl;
	}

	srand(time(0)); // автоматическая рандомизация
	cout << 1 + rand() % 10 << endl;



	system("PAUSE");
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.

/*Назначение C ++
Разработайте небольшую программу на C ++, которая будет реализовывать следующее:
1. Поддержка нескольких типов трехмерных геометрических кривых - окружностей, эллипсов и трехмерных спиралей. (Упрощенный
определения приведены ниже). Каждая кривая должна иметь возможность возвращать 3D-точку и первую производную (3D
вектор) на параметр t вдоль кривой.
2. Заполните контейнер (например, вектор или список) объектов этих типов, созданных случайным образом, с помощью
случайные параметры. Убедитесь, что контейнер будет содержать объекты всех поддерживаемых типов.
3. Вывести координаты точек и производные всех кривых в контейнере при t = PI / 4.
4. Заполните второй контейнер, который будет содержать только круги из первого контейнера. Убедитесь, что
второй контейнер разделяет (т.е. не клонирует) круги первого, например с помощью указателей.
5. Отсортируйте второй контейнер в порядке возрастания радиусов кружков. То есть первый элемент имеет
наименьший радиус, последний - наибольший.
6. Вычислите общую сумму радиусов всех кривых во втором контейнере.
Дополнительные необязательные требования:
7. Разделите реализацию на библиотеку кривых и исполняемый файл, который использует API этой библиотеки.
Ожидания от реализации:
1. Реализация должна использовать виртуальные методы.
2. Не имеет ни явного освобождения памяти, ни утечек памяти.
3. Кривые должны быть физически правильными (например, радиусы должны быть положительными).
4. Контейнеры и сортировка должны быть реализованы с использованием STL (стандартной библиотеки шаблонов C ++).
5. В реализации могут использоваться конструкции C ++ 11 и выше.
6. Код должен компилироваться любым компилятором по вашему выбору (gcc, Visual C ++ и т. Д.).
7. Результат должен быть отправлен по электронной почте или размещен на github. Доставка должна содержать только исходный код.
(без скомпилированных двоичных файлов): файлы * .h и * .cpp и файлы проекта, чтобы мы могли скомпилировать результат
мы сами.
Определения кривых:
- Все кривые задаются параметрически, т.е. точка вычисляется по некоторой формуле C (t).
- Окружность плоская в плоскости XoY (т.е. все координаты Z равны 0) и определяется своим радиусом.
- Эллипс плоский в плоскости XoY и определяется двумя его радиусами по осям X и Y.
- Спираль пространственная и определяется своим радиусом и шагом (см. Рисунок ниже). Требуется 2 * PI в
параметрическое пространство для обхода, т.е. любая точка спирали удовлетворяет условию C (t + 2 * PI) = C (t) +
{0, 0, шаг}. */


/*

Для окружности производная
C++Выделить код
1
Vector3D{ -1 * radi * sin(t), radi * cos(t) };

*/