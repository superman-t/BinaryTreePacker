//// demo2.cpp : 定义控制台应用程序的入口点。
////
//
#include "stdafx.h"
#include <iostream>
#include <cstddef>

#include <functional>
#include "kdtree.h"
#include "BinPack.h"

using namespace std;
 //#pragma pack(4)
//默认按照类中最大类型长度来对齐
class TestClassPtr {
public:
	char mName;
	float mValue;
	int mIndex;
	double mDouble;
	short mShortName;

	TestClassPtr()
	{
		cout << "constructor" << endl;
	};

	~TestClassPtr()
	{
		cout << "destructor" << endl;
	}

	void TestClassPtr::SetName( char name )
	{
		mName = name;
	};

	void TestClassPtr::SetDouble( double _double )
	{
		mDouble = _double;
	};

	TestClassPtr TestClassPtr::operator() ()
	{
		cout << "operator" << endl;
		return *this;
	};
};

template <typename T>
//typename std::enable_if_t<std::is_same<T, int>::value>
void func( const std::function<void( T )> & _func )
{
	//cout << &_func::operator() << endl;
}

template<class T>
class A
{};

template<int i>
class B
{};

class C
{
public:
	int x;
};

class D
{
public:
	C y;
	int z;

	void test()
	{
		cout << "test" << endl;
	}
};

template<class T> void f( T )
{
	cout << "T" << endl;
};
template<class T> void f1( const T )
{
	cout << "const T" << endl;
};
template<class T> void f2( volatile T )
{
	cout << "volatile T" << endl;
};
template<class T> void g( T* )
{
	cout << "T*" << endl;
};
template<class T> void g( T& )
{
	cout << "T&" << endl;
};
template<class T> void g1( T[10] )
{
	cout << "T[10]" << endl;
};
template<class T> void h1( A<T> )
{
	cout << "A<T>" << endl;
};

//void test_1()
//{
//	A<char> a;
//	C c;
//
//	f( c );   f1( c );   f2( c );
//	g( c );   g( &c );   g1( &c );
//	h1( a );
//}

template<class T>          void j( C( *)(T) )
{
	cout << "C(*) (T)" << endl;
};
template<class T>          void j( T( *)() )
{
	cout << "T(*) ()" << endl;
}
template<class T, class U> void j( T( *)(U) )
{
	cout << "T(*) (U)" << endl;
};

//void test_2()
//{
//	C( *c_pfunct1 )(int);
//	C( *c_pfunct2 )(void);
//	int( *c_pfunct3 )(int);
//	j( c_pfunct1 );
//	j( c_pfunct2 );
//	j( c_pfunct3 );
//}


template<class T>
void k( T C::* )
{
	cout << "T C::*" << endl;
};

template<class T>
void k( C T::* )
{
	if( std::is_same<T, D>::value )
	{
		cout << "U D" << endl;
	}
	cout << "C T::*" << endl;
};

//类的成员函数or成员变量指针
using pFun = void (D::*)(void);
template<class T, class U>
void k( T U::* )
{
	if( std::is_same<T, int>::value )
	{
		cout << "T int" << endl;
	}
	if( std::is_same<T, void>::value )
	{
		cout << "T void " << endl;
	}
	if( std::is_same<U, D>::value )
	{
		cout << "U D" << endl;
	}
	cout << "T U::*" << endl;
};

//void test_3()
//{
//	k( &C::x );
//	k( &D::y );
//	k( &D::z );//T int, U D
//	auto a = []( int )->float
//	{};
//	//k( &D::test );
//	//lamdba对象的类型为闭包类型，operator() 函数调用运算符，返回类型为lamdba的返回类型，参数为lamdba的形参
//	cout << std::is_same<decltype(a), decltype(&decltype(a)::operator())>::value << endl;
//}

template<class T>     void m( T( C::* )() )
{
	cout << "T (C::*)()" << endl;
};
template<class T>     void m( C( T::* )() )
{
	cout << "C (T::*)()" << endl;
};
template<class T>     void m( D( C::* )(T) )
{
	cout << "D (C::*)(T)" << endl;
};
template<class T, class U>  void m( C( T::* )(U) )
{
	cout << "C (T::*)(U)" << endl;
};
template<class T, class U>  void m( T( C::* )(U) )
{
	cout << "T (C::*)(U)" << endl;
};
template<class T, class U>  void m( T( U::* )() )
{
	cout << "T (U::*)()" << endl;
};
template<class T, class U, class V> void m( T( U::* )(V) )
{
	cout << "T (U::*)(V)" << endl;
};

//void test_4()
//{
//	int (C::*f_membp1)(void);
//	C( D::*f_membp2 )(void);
//	D( C::*f_membp3 )(int);
//	m( f_membp1 );
//	m( f_membp2 );
//	m( f_membp3 );
//
//	C( D::*f_membp4 )(int);
//	int (C::*f_membp5)(int);
//	int (D::*f_membp6)(void);
//	m( f_membp4 );
//	m( f_membp5 );
//	m( f_membp6 );
//
//	int (D::*f_membp7)(int);
//	m( f_membp7 );
//}

template<int i> void n( C[10][i] )
{
	cout << "E[10][i]" << endl;
};
template<int i> void n( B<i> )
{
	cout << "B<i>" << endl;
};

void test_5()
{
	C array[10][20];
	n( array );
	B<20> b;
	n( b );
}

template<template<class> class TT, class T> void p1( TT<T> )
{
	cout << "TT<T>" << endl;
};
template<template<int> class TT, int i>     void p2( TT<i> )
{
	cout << "TT<i>" << endl;
};
template<template<class> class TT>          void p3( TT<C> )
{
	cout << "TT<C>" << endl;
};

void test_6()
{
	A<char> a;
	B<20> b;
	A<C> c;
	p1( a );
	p2( b );
	p3( c );
}


void kdtree_test()
{
	double datas[100] = { 
		1.3, 1.3, 1.3,
		8.3, 8.3, 8.3,
		2.3, 2.3, 2.3,
		1.2, 1.2, 1.2,
		7.3, 7.3, 7.3,
		9.3, 9.3, 9.3,
		15, 15, 15,
		3, 3, 3,
		1.1, 1.1, 1.1,
		12, 12, 12,
		4, 4, 4,
		5, 5, 5 };
	double labels[100];
	for( size_t i = 0; i < 12; ++i )
		labels[i] = (float)i;

	KdTree* pKdTree = new KdTree( datas, labels, 12, 3, 2 );
	pKdTree->Log();
	const double coor[3] = { 4.1, 4.1, 4.1};
	std::vector<KdTree::Neighbor> ret = pKdTree->FindKNearests( coor, 2 );
	for( auto& neighbor : ret )
	{
		std::cout << std::get<0>( neighbor ) << " " << std::get<1>( neighbor ) << std::endl;
	}
}

void vector_test()
{
	std::vector<int> vec;
	for( auto i = 10; i > 0;--i )
	{
		vec.push_back( i );
	}
	struct
	{
		bool operator()( const int& lhs, const int& rhs ) const
		{
			return lhs < rhs;
		}
	} lesscmp;
	std::sort( vec.begin(), vec.end(), lesscmp );
	for( auto& i : vec )
	{
		std::cout << i << " ";
	}
	std::cout << std::endl;
	std::sort( vec.begin(), vec.end(), std::greater<int>() );
	for( auto& i : vec )
	{
		std::cout << i << " ";
	}
	std::cout << std::endl;
}

void align_test()
{
	int i[] = { 8, 4, 2, 1 };
	///指针大小：64位8字节，32位4字节，统一的
	///char* 为每次指针偏移一个字节读取，int* 偏移4个字节读取
	///指针类型主要区分：在内存中的解释方式不一样
	unsigned char *p = reinterpret_cast<unsigned char*> (i);

	printf( "%d %d %d\n", p, i, i[0] );// address address 8

									   /// big-endian
									   // 8000 4000 2000 1000
	int a = reinterpret_cast<char *>(p)[4];
	int b = 1;
	cout << a << " " << b << endl;//4 1
	*reinterpret_cast<char *>(i) = b;
	a = reinterpret_cast<char *>(p)[0];
	cout << a << " " << b << endl;//1 1

								  /// 将p的值赋值给了*p所指向的地址
	*reinterpret_cast<unsigned char **> (p) = p;
	//p += sizeof(int);
	cout << *reinterpret_cast<int*>(p) << " " << i[0] << " " << i[1] << endl;
	cout << sizeof( int* ) << " " << sizeof( void** ) << " " << sizeof( char* ) << " " << sizeof( double* ) << endl;
	cout << sizeof( TestClassPtr** ) << " " << sizeof( TestClassPtr ) << " " << offsetof( TestClassPtr, mValue ) << " " << offsetof( TestClassPtr, mIndex ) << " " << offsetof( TestClassPtr, mDouble ) << endl;
	cout << sizeof( float ) << " " << sizeof( double ) << " " << sizeof( int ) << " " << sizeof( short ) << endl;

	printf( "%d %d\n", &TestClassPtr::operator(), &TestClassPtr::SetName );

}

void lambda_template_test()
{
	std::function<void( int )> lamda1 = []( int i )
	{};
	std::function<void( float )> lamda2 = []( float i )
	{};
	func( lamda1 );
	func( lamda2 );
}

void template_test()
{
	//test_1();
	//test_2();
	//test_3();
	//test_4(); 
	test_5();
	test_6();
}

#include <SDL.h>
#include <random>
const int RUN_GAME_LOOP = 1;
SDL_Renderer* renderer = nullptr;
SDL_TimerID timer;
BlockVector blockVector;
auto packer = new Packer();
size_t GameLoopTimer( Uint32 interval, void* param )
{
	// Create a user event to call the game loop.
	SDL_Event event;

	event.type = SDL_USEREVENT;
	event.user.code = RUN_GAME_LOOP;
	event.user.data1 = 0;
	event.user.data2 = 0;

	SDL_PushEvent( &event );

	return interval;
}

void GameLoop()
{
	SDL_SetRenderDrawColor( renderer, 75, 75, 75, 255 );
	SDL_RenderClear( renderer );

	std::queue<NodePtr> que;
	auto root = packer->getRoot();
	que.emplace( root );
	while( !que.empty() )
	{
		auto node = que.front();
		que.pop();
		SDL_Rect r;
		r.x = node->x;
		r.y = node->y;
		r.w = node->w;
		r.h = node->h;

		SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
		SDL_RenderDrawRect( renderer, &r );

		if( node->down ) que.emplace( node->down );
		if( node->right ) que.emplace( node->right );

	}

	for( auto i = 0; i < blockVector.size(); ++i )
	{
		auto block = blockVector[i];
		if( block.fit )
		{
			SDL_Rect r;
			r.x = block.fit->x;
			r.y = block.fit->y;
			r.w = block.w;
			r.h = block.h;

			SDL_SetRenderDrawColor( renderer, 0, 0, 255, 255 );
			SDL_RenderFillRect( renderer, &r );
		}
	}
	SDL_RenderPresent( renderer );
}

void HandleUserEvents( SDL_Event* event )
{
	switch( event->user.code )
	{
		case RUN_GAME_LOOP:
			GameLoop();
			break;

		default:
			break;
	}
}

void bin_pack_test()
{
	random_device random;
	default_random_engine el( random() );
	uniform_int_distribution<int> uniform_dist( 10, 100 );
	

	size_t threshold = 20;
	for( auto i = 0; i < threshold; ++i )
	{
		int w = uniform_dist( el );
		int h = uniform_dist( el );
		blockVector.emplace_back( Block( w, h ) );
	}

	std::sort( blockVector.begin(), blockVector.end(), []( const Block& lhs, const Block& rhs )
	{
		return max(lhs.w, lhs.h) > max(rhs.w, rhs.h);
	} );

	for( auto& block : blockVector )
	{
		cout << block.w << "x" << block.h << endl;
	}

	packer->fit( blockVector );

	for( auto i = 0; i < blockVector.size(); ++i )
	{
		auto block = blockVector[i];
		if( block.fit )
		{
			cout << block.fit->x << " " << block.fit->y << " " << block.w << " " << block.h << endl;
		}
	}

	SDL_Window* window = nullptr;
	window = SDL_CreateWindow( "BinPack", SDL_WINDOWPOS_UNDEFINED,
							   SDL_WINDOWPOS_UNDEFINED,
							   1280, 780, SDL_WINDOW_SHOWN );


	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
	timer = SDL_AddTimer( 60, GameLoopTimer, nullptr);


	auto quit = false;

	SDL_Event event;

	while( !quit && SDL_WaitEvent(&event ))
	{
		switch (event.type)
		{
			case SDL_USEREVENT:
				HandleUserEvents( &event );
				break;
			case SDL_QUIT:
				quit = true;
				SDL_DestroyWindow( window );
				SDL_Quit();
				break;
			default:
				break;
		}
	}
}


int main(int argc, char *argv[])
{
	
	/*align_test();
	lambda_template_test();
	template_test();
	kdtree_test();
	vector_test();*/
	bin_pack_test();

	system("pause");
	
    return 0;
}
