#include "nanogui.h"
#include <iostream>
#include <string>
#include <cstdint>
#include <memory>
#include <utility>

#include "BinPack.h"
#include <random>
#include <queue>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;
using std::pair;
using std::to_string;
using namespace BP;


class MyGLCanvas : public nanogui::GLCanvas
{
public:
	MyGLCanvas( Widget *parent ) : nanogui::GLCanvas( parent )
	{
		generateData(20);
		refresh( "maxside" );
	}

	~MyGLCanvas()
	{

	}

	void refresh( const std::string& sortname)
	{
		std::sort( mBlockVector.begin(), mBlockVector.end(), [&]( const Block& lhs, const Block& rhs )
		{
			auto func = mBlockSort.SortComp( sortname );
			return (mBlockSort.*func)(lhs, rhs);
		} );
		mPacker.reset();
		mPacker.fit( mBlockVector );
	}

	void generateData( int count)
	{
		std::random_device lrandom;
		std::default_random_engine el( lrandom() );
		std::uniform_int_distribution<int> uniform_dist( 10, 100 );

		mBlockVector.reserve( count );
		mBlockVector.clear();

		size_t threshold = count;
		for( auto i = 0; i < threshold; ++i )
		{
			int w = uniform_dist( el );
			int h = uniform_dist( el );
			auto block = Block( w, h, NodeColor( uniform_dist( el ), uniform_dist( el ), uniform_dist( el ), 255 ) );
			//std::cout << w << "x" << h << std::endl;
			mBlockVector.emplace_back( block );
		}
	}

	virtual void drawGL() override
	{
		renderWireframe();
		renderRect();
	}

	void renderWireframe()
	{
		using namespace nanogui;

		std::queue<NodePtr> que;
		que.emplace( mPacker.getRoot() );
		while( !que.empty() )
		{
			auto node = que.front();
			que.pop();

			auto x = node->x;
			auto y = node->y;
			auto w = node->w;
			auto h = node->h;
			nvgBeginPath( this->screen()->nvgContext() );
			nvgRect( this->screen()->nvgContext(), x, y, w, h );
			nvgStrokeColor( this->screen()->nvgContext(), Color( 0, 255 ) );
			nvgStroke( this->screen()->nvgContext() );

			if( node->down ) que.emplace( node->down );
			if( node->right ) que.emplace( node->right );
		}

	}

	void renderRect()
	{
		using namespace nanogui;
		for( auto i = 0; i < mBlockVector.size(); ++i )
		{
			auto block = mBlockVector[i];
			if( block.fit )
			{
				auto x = block.fit->x;
				auto y = block.fit->y;
				auto w = block.w;
				auto h = block.h;
				nvgBeginPath( this->screen()->nvgContext() );
				nvgRect( this->screen()->nvgContext(), x, y, w, h );
				auto color = Color( block.color.r, block.color.g, block.color.b, block.color.a );
				nvgFillColor( this->screen()->nvgContext(), color );
				nvgFill( this->screen()->nvgContext() );
				nvgStrokeColor( this->screen()->nvgContext(), Color( 0, 255 ) );
				nvgStroke( this->screen()->nvgContext() );
			}
		}
	}

private:
	Packer mPacker;
	
	BlockVector mBlockVector;

	BlockSort mBlockSort;
};


class ExampleApplication : public nanogui::Screen
{
public:
	ExampleApplication() : nanogui::Screen( Eigen::Vector2i( 1024, 768 ), "BinaryTreePacker", false )
	{
		using namespace nanogui;

		static int index = 0;
		mCanvas = new MyGLCanvas( this );
		//mCanvas->setBackgroundColor( { 73, 73, 73, 125 } );
		mCanvas->setSize( { 1024, 768 } );

		FormHelper* gui = new FormHelper( this );

		ref<Window> window = gui->addWindow( Eigen::Vector2i( 720, 0 ), "Control Algorithm" );
	
		enum AlgorithmEnum
		{
			maxside,
			area,
			width,
			height
		};

		gui->addGroup( "Generate" );
		auto label = std::string( "Count" );
		auto default = 20;
		auto countLabel = gui->addVariable( label, default );
		countLabel->setEditable( true );

		auto generate = gui->addButton( "Generate", [&, countLabel] {
			auto count = countLabel->value() > 0 ? countLabel->value() : 20;
			countLabel->setValue( count );
			mCanvas->generateData( count );
			switch( index )
			{
				case 0:
					mCanvas->refresh( "maxside" );
					break;
				case 1:
					mCanvas->refresh( "area" );
					break;
				case 2:
					mCanvas->refresh( "width" );
					break;
				case 3:
					mCanvas->refresh( "height" );
					break;
				default:
					break;
			}
		} );

		
		AlgorithmEnum algorithm = maxside;
		gui->addGroup( "Algorithm" );
		auto algoEnum = gui->addVariable( "Sort", algorithm, true );
		algoEnum->setItems( { "maxside", "area", "width", "height" } );
		algoEnum->setCallback( [&]( int id )
		{
			index = 0;
			switch( id )
			{
				case 0:
					mCanvas->refresh( "maxside" );
					break;
				case 1:
					mCanvas->refresh( "area" );
					break;
				case 2:
					mCanvas->refresh( "width" );
					break;
				case 3:
					mCanvas->refresh( "height" );
					break;
				default:
					break;
			}
		} );

		//Button *b0 = new Button( tools, "maxside" );
		//b0->setCallback( [this]()
		//{
		//	/*mCanvas->setBackgroundColor( Vector4i( rand() % 256, rand() % 256, rand() % 256, 255 ) );*/
		//	mCanvas->refresh("maxside");
		//} );

		//Button *b1 = new Button( tools, "area" );
		//b1->setCallback( [this]()
		//{
		//	mCanvas->refresh( "area" );
		//} );

		//Button *b2 = new Button( tools, "width" );
		//b2->setCallback( [this]()
		//{
		//	mCanvas->refresh( "width" );
		//} );

		//Button *b3 = new Button( tools, "height" );
		//b3->setCallback( [this]()
		//{
		//	mCanvas->refresh( "height" );
		//} );

		performLayout();
	}

	virtual bool keyboardEvent( int key, int scancode, int action, int modifiers )
	{
		if( Screen::keyboardEvent( key, scancode, action, modifiers ) )
			return true;
		if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
		{
			setVisible( false );
			return true;
		}
		return false;
	}

	virtual void draw( NVGcontext *ctx )
	{
		/* Draw the user interface */
		Screen::draw( ctx );
	}
private:
	MyGLCanvas *mCanvas;
};

int main( int /* argc */, char ** /* argv */ )
{
	try
	{
		nanogui::init();

		/* scoped variables */
		{
			nanogui::ref<ExampleApplication> app = new ExampleApplication();
			app->drawAll();
			app->setVisible( true );
			nanogui::mainloop();
		}

		nanogui::shutdown();
	}
	catch( const std::runtime_error &e )
	{
		std::string error_msg = std::string( "Caught a fatal error: " ) + std::string( e.what() );
	#if defined(_WIN32)
		MessageBoxA( nullptr, error_msg.c_str(), NULL, MB_ICONERROR | MB_OK );
	#else
		std::cerr << error_msg << endl;
	#endif
		return -1;
	}

	return 0;
}
