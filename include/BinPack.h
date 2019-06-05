#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>
#include <map>

using std::max;
using std::min;

namespace BP
{
	struct Node;
	struct Block;
	typedef float ElementType;
	typedef std::shared_ptr<Node> NodePtr;

	struct NodeColor
	{
		int r;
		int g;
		int b;
		int a;

		NodeColor() : NodeColor( 0, 0, 0, 0 )
		{};

		NodeColor( int _r, int _g, int _b, int _a ) :r( _r ), g( _g ), b( _b ), a( _a )
		{};
	};

	struct Block
	{
		Block( ElementType _w, ElementType _h, NodeColor _color ) :w( _w ), h( _h ), color( _color ), fit( nullptr )
		{};

		~Block()
		{
			fit = nullptr;
		};

		ElementType w;
		ElementType h;
		NodePtr fit;
		NodeColor color;
	};

	typedef std::vector<Block> BlockVector;

	struct BlockSort
	{
	private:
		using  SortMetaFunc = ElementType( BlockSort::* )(const Block& lhs, const Block& rhs);
		using  SortFunc = bool(BlockSort::*)(const Block& lhs, const Block& rhs);

		ElementType _random( const Block& lhs, const Block& rhs )
		{
			return rand() - 0.5f;
		};
		ElementType w( const Block& lhs, const Block& rhs )
		{
			return lhs.w - rhs.w;
		};
		ElementType h( const Block& lhs, const Block& rhs )
		{
			return lhs.h - rhs.h;
		};
		ElementType a( const Block& lhs, const Block& rhs )
		{
			return lhs.w*lhs.h - rhs.w*rhs.h;
		};

		ElementType _max( const Block& lhs, const Block& rhs )
		{
			return max( lhs.w, lhs.h ) - max( rhs.w, rhs.h );
		};

		ElementType _min( const Block& lhs, const Block& rhs )
		{
			return min( lhs.w, lhs.h ) - min( rhs.w, rhs.h );
		};

		std::map<std::string, SortMetaFunc> metaSortMap = {
			{ "random", &BlockSort::_random},
			{ "w", &BlockSort::w},
			{ "h", &BlockSort::h},
			{ "a", &BlockSort::a},
			{ "max", &BlockSort::_max},
			{ "min", &BlockSort::_min}
		};


		bool width( const Block& lhs, const Block& rhs )
		{
			return msort( lhs, rhs, { "w", "h" } );
		};

		bool height( const Block& lhs, const Block& rhs )
		{
			return msort( lhs, rhs, { "h", "w" } );
		};

		bool area( const Block& lhs, const Block& rhs )
		{
			return msort( lhs, rhs, { "a", "h", "w" } );
		};

		bool maxside( const Block& lhs, const Block& rhs )
		{
			return msort( lhs, rhs, { "max", "min", "h", "w" } );
		};
		std::map<std::string, SortFunc> sortMap = {
			{ "width", &BlockSort::width },
			{ "height", &BlockSort::height },
			{ "area", &BlockSort::area },
			{ "maxside", &BlockSort::maxside },
		};
		//以顺序集合的形势进行排序，按照集合的优先级排序，当遇到相等后继续以集合中指定的顺序排序
		bool msort( const Block& lhs, const Block& rhs, std::vector <std::string> criteria )
		{
			ElementType diff;
			for( auto i = 0; i < criteria.size(); ++i )
			{
				SortMetaFunc func = metaSortMap.at( criteria[i] );
				diff = (this->*func)(lhs, rhs);
				if( diff != 0 )
				{
					return diff > 0 ? true : false;
				}
			}
			return false;
		}
	public:
		SortFunc SortComp( const std::string& sortName )
		{
			return sortMap.at( sortName );
		};
	};

	struct Rect
	{
		ElementType x;
		ElementType y;
		ElementType w;
		ElementType h;

		Rect( ElementType _x, ElementType _y, ElementType _w, ElementType _h )
			:x( _x ), y( _y ), w( _w ), h( _h )
		{};

		Rect() :x( 0 ), y( 0 ), w( 0 ), h( 0 )
		{};
	};

	struct Node : public Rect
	{
		Node()
			:Rect( 0, 0, 0, 0 ), used( false ), down( nullptr ), right( nullptr )
		{};

		Node( ElementType _x, ElementType _y, ElementType _w, ElementType _h )
			:Rect( _x, _y, _w, _h ), used( false ), down( nullptr ), right( nullptr )
		{};

		~Node()
		{
			down = nullptr;
			right = nullptr;
		};

		Node( const Node& other )
		{
			x = other.x;
			y = other.y;
			w = other.w;
			h = other.h;
			used = other.used;
			down = other.down;
			right = other.right;
		};

		bool used;
		NodePtr down;
		NodePtr right;
	};

	/*(0,0)  _______________x
			|       |h
			|_______|
			|    w
			|
			y*/

	class Packer
	{
	public:
		Packer() :Packer( 0, 0 )
		{};

		Packer( ElementType w, ElementType h );

		~Packer();

		void fit( BlockVector& blockVector );

		NodePtr findNode( NodePtr node, ElementType w, ElementType h );

		NodePtr splitNode( NodePtr node, ElementType w, ElementType h );

		NodePtr growNode( ElementType w, ElementType h );

		NodePtr growDown( ElementType w, ElementType h );

		NodePtr growRight( ElementType w, ElementType h );

		NodePtr getRoot()
		{
			return root;
		};

		void reset();

	private:
		NodePtr root;

	};
}