#pragma once

#include "stdafx.h"
#include <vector>
#include <memory>

typedef float ElementType;
struct Node;
typedef std::shared_ptr<Node> NodePtr;
struct Block
{
	Block( ElementType _w, ElementType _h ) :w( _w ), h( _h ), fit(nullptr)
	{};

	~Block()
	{
		fit = nullptr;
	};

	ElementType w;
	ElementType h;
	NodePtr fit;
};

typedef std::vector<Block> BlockVector;

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
	{
	};
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
	Packer():Packer( 0, 0 )
	{
	};

	Packer(ElementType w, ElementType h);
	
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

private:
	NodePtr root;

};