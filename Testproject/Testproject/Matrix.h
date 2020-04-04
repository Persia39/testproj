#define HAVE_STRUCT_TIMESPEC
#include <mutex>
#include <pthread.h>
#include <windows.h>
#include <string>
#include <fstream>
template<typename T>
class Matrix {
public:
	struct dop{
		Matrix<T>* main_matrix;
		T**second_array;
		Matrix<T>*result_matrix;
	};
	class Exept {
	public:
		Exept(T v1, T v2) :val1(v1), val2(v2) {}
		T val1;
		T val2;

	};
	Matrix(int m, int n,T val=0,std::string fName="testfile.txt");
	Matrix(Matrix const &);
	~Matrix();
	void inputMatrix();
	void setFilename(std::string str);
	template<typename U>
	friend std::ostream& operator<< (std::ostream&, Matrix<U> const&);
	Matrix<T> operator* (Matrix const& m) ;
	std::pair<int,int> getTekPos();
	std::pair<int, int> getNewPos(Matrix<T>* mt);
	static void *multipl(void* arg);
	std::string filename;
private:
	int M, N;
	T** matrix = nullptr;
	std::pair<int, int> tekPos;
	pthread_t tid1, tid2;
	pthread_mutex_t mutex;
};

template<typename T>
inline Matrix<T>::Matrix(int m, int n, T val, std::string fName):M(m),N(n),filename(fName)
{
	matrix = new T*[M];
	for (auto i = 0; i < M; i++)
		matrix[i] = new T [N];
	for (auto i = 0; i < M; i++)
		for (auto j = 0; j < N; j++)
			matrix[i][j] = val;
	pthread_mutex_init(&mutex, NULL);
}

template<typename T>
inline Matrix<T>::Matrix(Matrix const &m)
{
	M = m.M;
	N = m.N;
	filename = m.filename;
	matrix = new T*[M];
	for (auto i = 0; i < M; i++)
		matrix[i] = new T[N];

	for (auto i = 0; i < M; i++)
		for (auto j = 0; j < N; j++)
			matrix[i][j] = m.matrix[i][j];
	pthread_mutex_init(&mutex, NULL);
}

template<typename T>
inline Matrix<T>::~Matrix()
{
	for (auto i = 0; i < M; i++)
		delete[] matrix[i];
	delete[] matrix;
	pthread_mutex_destroy(&mutex);
}

template<typename T>
inline void Matrix<T>::inputMatrix()
{
	for (auto i = 0; i < M; i++)
		for (auto j = 0; j < N; j++)
			std::cin>>matrix[i][j];
}

template<typename T>
inline void Matrix<T>::setFilename(std::string str)
{
	filename = str;
}

template<typename T>
inline Matrix<T> Matrix<T>::operator*(Matrix const & m) 
{
	if (N != m.M)
		throw Exept(N, m.M);
	Matrix<T> mtr(M, m.N);
	dop* str=new dop;
	str->main_matrix = this;
	str->second_array = m.matrix;
	str->result_matrix = &mtr;
	pthread_create(&tid1, NULL,multipl, str);
	pthread_create(&tid2, NULL, multipl, str);
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	delete str;
	std::ofstream fout(filename,std::ios::app);
	fout << mtr;
	fout.close();
	return mtr;
}

template<typename T>
inline std::pair<int, int> Matrix<T>::getTekPos()
{
	return tekPos;
}

template<typename T>
inline std::pair<int, int> Matrix<T>::getNewPos(Matrix<T>* mt)
{
	pthread_mutex_lock(&mutex);
	std::pair<int, int> savePos = tekPos;
	if (tekPos.second + 1 == mt->N)
	{
		tekPos.first++;
		tekPos.second = 0;
	}
	else
		tekPos.second++;
	pthread_mutex_unlock(&mutex);
	return savePos;
}

template<typename T>
inline void * Matrix<T>::multipl(void * arg)
{
	dop* str = (dop*)arg;
	std::pair<int, int > endPos = { str->result_matrix->M,str->result_matrix->N - 1 };
	while (str->main_matrix->getTekPos() < endPos)
	{
		Sleep(400);
		std::pair<int, int> pr = str->main_matrix->getNewPos(str->result_matrix);
		int m = pr.first;
		int n = pr.second;
		if (m >= str->result_matrix->M)
			return NULL;
		for (auto k = 0; k < str->main_matrix->N; k++)
			str->result_matrix->matrix[m][n] += str->main_matrix->matrix[m][k] * str->second_array[k][n];
	}
	return NULL;
}

template<typename U>
inline std::ostream & operator<<(std::ostream & out, Matrix<U> const & matr)
{
	for (auto i = 0; i < matr.M; i++)
	{
		for (auto j = 0; j < matr.N; j++)
			out << matr.matrix[i][j] << "  ";
		out << std::endl;
	}
	out << std::endl;
	return out;
}

