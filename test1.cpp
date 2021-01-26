#include <iostream>
#include <random>
#include <cmath>
#include <memory>
#include <typeinfo>
#include <algorithm>

using namespace std;

//===============================================================>
constexpr double eps = 1e-6;
//===============================================================>
struct Point3D { double x{}, y{}, z{}; };   // точка в 3-х мерном пространстве

ostream& operator<<(ostream& os, const Point3D& p)
{   // выводим точку в поток
	return os << "P{ " << p.x << ", " << p.y << ", " << p.z << " }";
}
//===============================================================>
struct Vector3D { double x{}, y{}, z{}; };  // вектор в 3-х мерном пространстве

ostream& operator<<(ostream& os, const Vector3D& v)
{   // выводим вектор в поток
	return os << "V{ " << v.x << ", " << v.y << ", " << v.z << " }";
}
//===============================================================>

//==================== базовый класс ============================>
class Shape3D   
{
protected:
	Point3D c{}; // "центр"(опорная точка)

	Shape3D() = default;
	Shape3D(const Point3D& center) : c{ center } {}

public:
	virtual ~Shape3D() {}

	virtual Point3D curve_point(double t) const = 0;    // точка кривой по параметру t
	virtual Vector3D curve_vector(double t) const = 0;  // вектор(первая производная) кривой по параметру t
};
//===============================================================>

class Circle3D : public Shape3D // окружность в плоскости XY
{
	double radi{};

public:
	Circle3D() = default;
	Circle3D(const Point3D& center, double radius)
		: Shape3D(center), radi{ radius } {}

	virtual ~Circle3D() {}

	double radius() const { return radi; }  // для сортировки и суммирования

	virtual Point3D curve_point(double t) const override
	{
		return Point3D
		{
			radi * cos(t) + c.x,
			radi * sin(t) + c.y,
			c.z
		};
	}
	virtual Vector3D curve_vector(double t) const override
	{
		return Vector3D
		{
			-1 * radi * sin(t),
			radi * cos(t)
		};
	}
};
//===============================================================>
class Ellipse3D : public Shape3D    // эллипс в плоскости XY
{
	double radi_x{};
	double radi_y{};

public:
	Ellipse3D() = default;
	Ellipse3D(const Point3D& center, double radius_x, double radius_y)
		: Shape3D(center), radi_x{ radius_x }, radi_y{ radius_y } {}

	virtual ~Ellipse3D() {}

	virtual Point3D curve_point(double t) const override
	{
		return Point3D
		{
			radi_x * cos(t) + c.x,
			radi_y * sin(t) + c.y,
			c.z
		};
	}
	virtual Vector3D curve_vector(double t) const override
	{
		return Vector3D
		{
			-1 * radi_x * sin(t),
			radi_y * cos(t)
		};
	}
};
//===============================================================>
class Helix3D : public Shape3D  // спираль с осью, параллельной оси Z
{
	double radi{};
	double step{};

public:
	Helix3D() = default;
	Helix3D(const Point3D& center, double radius, double step_z)
		: Shape3D(center), radi{ radius }, step{ step_z } {}

	virtual ~Helix3D() {}

	virtual Point3D curve_point(double t) const override
	{
		return Point3D
		{
			radi * t * cos(t) + c.x,
			radi * t * sin(t) + c.y,
			step * t + c.z
		};
	}
	virtual Vector3D curve_vector(double t) const override
	{
		return Vector3D
		{
			radi * (cos(t) - t * sin(t)),
			radi * (sin(t) + t * cos(t)),
			step
		};
	}
};
//===============================================================>
using PShape = shared_ptr<Shape3D>; // псевдоним типа - умный указатель на базовый класс

class Shape_generator   // генератор объектов - потомков Shape3D
{
	enum shape_t { circle_t, ellipse_t, helix_t };   // типы генерируемых объектов

	static random_device rd;

	uniform_real_distribution<double> udd{ -1, 1 };    // генератор случайных вещественных чисел(ГСВЧ)
	uniform_int_distribution<> usd{ circle_t, helix_t };   // генератор случайных типов объектов

public:

	void set_diapason(double min, double max)   // если нужно изменить диапазон
	{
		if ((abs(max) + abs(min)) < 2 * eps)  // если значения диапазона одновременно слишко близки к нулю,
			return; // ничего не меняем
		udd = decltype(udd){ min, max }; // меняем параметры ГСВЧ
	}

	PShape circle()
	{   // метод генерирует и размещает в динамической памяти объект класса Circle3D
		// и возвращает умный указатель на базовый класс
		double radius{};
		while (radius < eps)    // проверяем, что бы радиус был не слишком маленьким
			radius = abs(udd(rd)); // и > 0

		return make_shared<Circle3D>(Point3D{ udd(rd), udd(rd) }, radius);
	}

	PShape ellipse()
	{   // метод генерирует и размещает в динамической памяти объект класса Ellipse3D
		// и возвращает умный указатель на базовый класс
		double radius_x{};
		while (radius_x < eps)  // проверяем, что бы первый радиус был не слишком маленьким
			radius_x = abs(udd(rd));   // и > 0

		double radius_y{};
		while (radius_y < eps)  // проверяем, что бы второй радиус был не слишком маленьким
			radius_y = abs(udd(rd));   // и > 0

		return make_shared<Ellipse3D>(Point3D{ udd(rd), udd(rd) }, radius_x, radius_y);
	}

	PShape helix()
	{   // метод генерирует и размещает в динамической памяти объект класса Helix3D
		// и возвращает умный указатель на базовый класс
		double radius{};
		while (radius < eps)    // проверяем, что бы радиус был не слишком маленьким
			radius = abs(udd(rd)); // и > 0

		double step{};
		while (abs(step) < eps)    // проверяем, что бы шаг был не слишком маленьким
			step = udd(rd);

		return make_shared<Helix3D>(Point3D{ udd(rd), udd(rd), udd(rd) }, radius, step);
	}

	PShape random_shape()
	{   // метод генерирует случайную фигуру из доступного набора
		switch (usd(rd))
		{
		case circle_t:
			return circle();
		case ellipse_t:
			return ellipse();
		case helix_t:
			return helix();
		default:
			return circle(); // мало ли что
		}
	}
};
//---------------------------------------------------------------
random_device Shape_generator::rd;
//===============================================================>

//=================Тело программы================================>
int main()
{
	constexpr size_t sz = 10;   // размер первого массива
	const double pi = acos(-1);    // пи****

	vector<PShape> shape_vec;  // массив указателей на объекты базового класса

	{
		Shape_generator s_gen;  // генератор
		s_gen.set_diapason(-10, 10);    // инициализируем диапазон
		for (size_t i{}; i < sz; ++i)
			shape_vec.emplace_back(s_gen.random_shape());   // заполняем массив
	}

	using PCircle = shared_ptr<Circle3D>;  // псевдоним типа - умный указатель на Circle3D

	vector<PCircle> circle_vec;    // массив указателей на Circle3D из первого контейнера

	double radius_sum{};    // сумма радиусов Circle3D

	for (auto& ps : shape_vec)
	{
		const type_info& ti{ typeid(*ps) };    // информация о типе объекта, на который указывает ps из первого контейнера

		cout << ti.name() << ' '   // выводим имя типа
			<< ps->curve_point(pi / 4) << ' '   // точку кривой, заданную параметром
			<< ps->curve_vector(pi / 4) << endl;   // вектор(первая производная - касательная) в предыдущей точке

		if (ti.hash_code() == typeid(Circle3D).hash_code()) // если текущий объект - указатель на Circle3D
		{
			circle_vec.emplace_back(dynamic_pointer_cast<Circle3D>(ps));   // кастим его к указателю на Circle3D
			radius_sum += circle_vec.back()->radius();  // плюсуем радиус
			cout << "to another vector -->\n"; // debug (можно удалить эту строку)
		}
		cout << endl;
	}
	sort(circle_vec.begin(), circle_vec.end(), [](const auto& lh, const auto& rh)  // сортируем по радиусу
	{
		return lh->radius() < rh->radius();
	});

	for (auto& pc : circle_vec) // демонстрируем отсортированное
		cout << pc->radius() << endl;
	cout << "sum = " << radius_sum << endl;   // и сумму радиусов всех Circle3D

	return 0;
}
