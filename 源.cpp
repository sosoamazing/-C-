#include<iostream>
#include<time.h>
#include<stdlib.h>
#include<cmath>
#include<algorithm>
#include<fstream>
#include<vector>

#define N 0xfffffffe

using namespace std;

struct Point {
	double x, y;
	Point() :x(0), y(0) {}
	Point(double a, double b) :x(a), y(b) {}

};

bool cmp1(const Point* p1, const Point* p2)
{
	return p1->x < p2->x;
}

bool cmp2(const Point* p1, const Point* p2)
{
	return p1->y < p2->y;
}

double len(Point* p1, Point* p2) {
	return sqrt((p1->x - p2->x) * (p1->x - p2->x) + (p1->y - p2->y) * (p1->y - p2->y));
}

pair<Point*, Point*>ans;
double min_len1, min_len2;	

vector<Point*> get_array(int size)
{
	time_t timep;
	time(&timep);
	srand(timep);
	vector<Point*> a;
	for (int i = 0; i < size; i++)
	{
		double x = double(rand() % 20000) + double(rand() % 100) * 0.01;
		double y = double(rand() % 20000) + double(rand() % 100) * 0.01;
		a.push_back(new Point(x, y));
	}
	return a;
}

void Delete(vector<Point*>& a)
{
	for (auto& p : a)
	{
		delete p;
	}
}

//**********************************************暴力法

void violent_function(vector<Point*> a)
{
	ans = { a[0], a[1] };
	min_len1 = len(a[0], a[1]);		//初始化
	int n = a.size();
	for (int i = 0; i < n; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			double d = len(a[i], a[j]);
			if (d < min_len1)
			{
				ans = { a[i],a[j] };
				min_len1 = d;
			}
		}
	}
}

//*************************************************二分法

int x_search_l(int l, int r, vector<Point*>& a, double target)
{
	while (l < r)
	{
		int mid = (l + r) / 2;
		if (a[mid]->x < target)l = mid + 1;
		else r = mid;
	}
	return l;
}

int x_search_r(int l, int r, vector<Point*>& a,double target)
{
	while (l < r)
	{
		int mid = (l + r + 1) / 2;
		if (a[mid]->x > target)r = mid - 1;
		else l = mid;
	}
	return l;
}

void two_devide_better(vector<Point*>& a, int l, int r) {
	if (l >= r)return;
	if (r - l == 1)
	{
		double d = len(a[l], a[r]);
		if (d < min_len2)
		{
			min_len2 = d;
			ans = { a[l],a[r] };
		}
		return;
	}
	if (r - l == 2)
	{
		double d = len(a[l], a[r]);
		if (d < min_len2)
		{
			min_len2 = d;
			ans = { a[l],a[r] };
		}
		d = len(a[l], a[l + 1]);
		if (d < min_len2)
		{
			min_len2 = d;
			ans = { a[l],a[l + 1] };
		}
		d = len(a[l + 1], a[r]);
		if (d < min_len2)
		{
			min_len2 = d;
			ans = { a[l + 1],a[r] };
		}
		return;
	}
	int mid = (l + r) / 2;
	two_devide_better(a, l, mid);
	two_devide_better(a, mid + 1, r);
	double midline = a[mid + 1]->x;
	l = x_search_l(l, mid, a, midline - min_len2);		//求左边界
	r = x_search_r(mid + 1, r, a, midline + min_len2);	//求右边界
	/*for (int i = l; i <= r; i++)
	{
		if (fabs(a[i]->x - midline) > min_len)cout << "yes" << endl;
	}
	if (l>0&&fabs(a[l]->x - midline) > min_len)cout << "YES" << endl;
	if (r < a.size()-1 && fabs(a[r]->x - midline) > min_len)cout << "YES" << endl;*/
	vector<Point*> v(a.begin() + l, a.begin() + r + 1);
	r = r - l;
	sort(v.begin(), v.end(), cmp2);
	for (int i = 0; i < r; i++)
	{
		for (int j = 1; j < 6 && i + j <= r; j++)
		{
			if (v[i + j]->y > v[i]->y + min_len2)break;
			if ((v[i]->x - midline) * (v[i + j]->x - midline) > 0)continue;//排除两个点在L的同一侧的情况
			double d = len(v[i], v[i + j]);
			if (d < min_len2)
			{
				min_len2 = d;
				ans = { v[i],v[i + j] };
			}
		}
	}
}

double two_devide(vector<Point*>& a, int l, int r)
{
	if (l >= r)return len(a[0], a[1]);
	if (r - l == 1)return len(a[l], a[r]);
	if (r - l == 2)return min(len(a[l], a[r]), 
		min(len(a[l], a[l + 1]), len(a[l + 1], a[r])));
	int mid = (l + r) / 2;
	double dl = two_devide(a, l, mid);
	double dr = two_devide(a, mid + 1, r);
	double dm = min(dl, dr);
	double midline = a[mid + 1]->x;
	l = x_search_l(l, mid, a, midline - dm);		//求左边界
	r = x_search_r(mid + 1, r, a, midline + dm);	//求右边界
	vector<Point*> ll(a.begin() + l, a.begin() + mid + 1);
	vector<Point*> rr(a.begin() + mid + 1, a.begin() + r + 1);
	sort(ll.begin(), ll.end(), cmp2);
	sort(rr.begin(), rr.end(), cmp2);
	int tail = 0;
	for (int i = 0; i < ll.size(); i++)
	{
		while (tail < rr.size() && rr[tail]->y < ll[i]->y + dm)tail++;
		for (int j = 1; j < 7; j++)
		{
			if (tail - j < 0 || rr[tail - j]->y < ll[i]->y - dm)break;
			double d = len(ll[i], rr[tail - j]);
			if (d < dm)
			{
				dm = d;
			}
		}
	}
	return dm;
}

void devide_and_conquer(vector<Point*> v)					//预处理
{
	sort(v.begin(), v.end(), cmp1);			//x升序排序
	//ans = { v[0], v[1] };										//向前探查和初始化
	//min_len2 = len(v[0], v[1]);
	//two_devide_better(v, 0, v.size() - 1);					//优化算法
	min_len2 = two_devide(v, 0, v.size() - 1);		//原算法
}

//****************************************************

const char* func_name[2] = { "violent_function","devide_and_conquer" };
void (*func[2])(vector<Point*>) = { violent_function,devide_and_conquer };

void experiment()
{
	//FILE* file;
	//freopen_s(&file, "final10.txt", "w", stdout);					//实验
	/*for (int i = 1; i < 2; i++)
	{
		cout << func_name[i] << endl;
		for (int n = 100000; n <= 10000000; n += 100000)
		{
			vector<Point*> a = get_array(n);
			clock_t now = clock();
			func[i](a);
			cout << "while n==" << n << " : " << clock() - now << endl;
			Delete(a);
		}
	}return;*/
	//if(file)fclose(file);
	for (int n = 100000; n <= 1000000; n += 100000)			//增大实验数据范围，找到耗时趋于稳定的数据量
	{
		int sum = 0;
		for (int t = 0; t < 20; t++)
		{
			vector<Point*> a = get_array(n);
			clock_t now = clock();
			func[1](a);
			sum += clock() - now;
			Delete(a);
		}
		cout << "while n==" << n << " : " << sum / 20 << endl;
	}
}

void test()
{
	for (int n = 10000; n <= 50000; n += 000)
	{
		vector<Point*> a = get_array(n);
		func[0](a);
		func[1](a);
		if (min_len1 != min_len2)cout << "NO" << endl;
		Delete(a);
	}
}

int main()
{
	//test();		//死循环，可自定义一段时间内测试算法正确性
	experiment();
	return 0;
}