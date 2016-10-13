#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "openvdb.h"
#include "openvdb/tools/VolumeToMesh.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class hellovdbApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void hellovdbApp::setup()
{
	openvdb::initialize();
    
	typedef openvdb::tree::Tree4<float, 5, 4, 3>::Type FloatTreeType;
	typedef openvdb::Grid<FloatTreeType> FloatGridType;

	FloatGridType grid( 1.0f );

	// test voxel region meshing

	openvdb::CoordBBox bbox( openvdb::Coord( 1 ), openvdb::Coord( 6 ) );

	grid.tree().fill( bbox, -1.0f );

	std::vector<openvdb::Vec3s> points;
	std::vector<openvdb::Vec4I> quads;
	std::vector<openvdb::Vec3I> triangles;

	openvdb::tools::volumeToMesh( grid, points, quads );

	points.clear();
	quads.clear();
	triangles.clear();
	grid.clear();

	// test tile region meshing

	grid.tree().addTile( FloatTreeType::LeafNodeType::LEVEL + 1, openvdb::Coord( 0 ), -1.0f, true );

	openvdb::tools::volumeToMesh( grid, points, quads );

	points.clear();
	quads.clear();
	triangles.clear();
	grid.clear();

	// test tile region and bool volume meshing

	typedef FloatTreeType::ValueConverter<bool>::Type BoolTreeType;
	typedef openvdb::Grid<BoolTreeType> BoolGridType;

	BoolGridType maskGrid( false );

	maskGrid.tree().addTile( BoolTreeType::LeafNodeType::LEVEL + 1, openvdb::Coord( 0 ), true, true );

	openvdb::tools::volumeToMesh( maskGrid, points, quads );
}

void hellovdbApp::mouseDown( MouseEvent event )
{
}

void hellovdbApp::update()
{
}

void hellovdbApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
}

CINDER_APP( hellovdbApp, RendererGl )
