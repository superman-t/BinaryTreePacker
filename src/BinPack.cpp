#include "stdafx.h"
#include "BinPack.h"

Packer::Packer(ElementType w, ElementType h)
{
	this->root = NodePtr( new Node( 0, 0, w, h ) );
	this->root->used = false;
}

Packer::~Packer()
{

}

void Packer::fit( BlockVector& blockVector )
{
	auto len = blockVector.size();
	auto w = len > 0 ? blockVector[0].w : 0;
	auto h = len > 0 ? blockVector[0].h : 0;
	this->root->w = w;
	this->root->h = h;

	for( auto i = 0; i < blockVector.size(); ++i )
	{
		auto& block = blockVector[i];
		auto w = block.w;
		auto h = block.h;
		if( auto node = findNode( this->root, w, h ) )
		{
			block.fit = splitNode( node, w, h);
		}
		else
		{
			block.fit = this->growNode( w, h );
		}
	}
}

NodePtr Packer::findNode( NodePtr root, ElementType w, ElementType h )
{
	if( root->used )
	{
		auto ret = this->findNode( root->right, w, h );
		if (!ret ) ret = this->findNode( root->down, w, h );
		return ret;
	}
	else if( w <= root->w && h <= root->h )
	{
		return root;
	}
	else
	{
		return nullptr;
	}
}

NodePtr Packer::splitNode( NodePtr node, ElementType w, ElementType h )
{
	node->used = true;
	node->down = NodePtr( new Node( node->x, node->y + h, node->w, node->h - h ) );
	node->right = NodePtr( new Node( node->x + w, node->y, node->w - w, h ) );

	return node;
}

NodePtr Packer::growNode( ElementType w, ElementType h )
{
	auto canGrowDown = w <= this->root->w;
	auto canGrowRight = h <= this->root->h;

	auto shouldGrowRight = canGrowRight && (this->root->h >= this->root->w + w);
	auto shouldGrowDown = canGrowDown && (this->root->w >= this->root->h + h);

	if( shouldGrowDown )
		return this->growDown( w, h );
	else if( shouldGrowRight )
		return this->growRight( w, h );
	else if( canGrowRight )
		return this->growRight( w, h );
	else if( canGrowDown )
		return this->growDown( w, h );
	else
		return nullptr;
}

NodePtr Packer::growDown( ElementType w, ElementType h )
{
	auto ow = this->root->w;
	auto oh = this->root->h;
	auto oroot = this->root;
	this->root = NodePtr( new Node( 0, 0, ow, oh + h ) );
	this->root->used = true;
	this->root->right = oroot;
	this->root->down = NodePtr( new Node( 0, oh, ow, h ) );

	if( auto node = this->findNode( this->root, w, h ) )
		return this->splitNode( node, w, h );
	else
		return nullptr;
}

NodePtr Packer::growRight( ElementType w, ElementType h )
{
	auto ow = this->root->w;
	auto oh = this->root->h;
	auto oroot = this->root;
	this->root = NodePtr( new Node( 0, 0, ow + w, oh ) );
	this->root->used = true;
	this->root->down = oroot;
	this->root->right = NodePtr( new Node( ow, 0, w, oh ) );

	if( auto node = this->findNode( this->root, w, h ) )
		return this->splitNode( node, w, h );
	else
		return nullptr;
}