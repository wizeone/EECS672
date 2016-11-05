// project2.c++: Starter for EECS 672 Project 2

#include "GLFWController.h"
#include "PhongMaterial.h"
#include "Barbell.h"

void createRoom(Controller& c, ShaderIF* sIF)
{
	PhongMaterial barbellMatl(0.1, 0.1, 0.1, 0.2, 0.5, 9.5, 55.0, 1.0);
	double barR = 0.25, bellR = 1.0;
	c.addModel( new Barbell(sIF, barbellMatl, cryph::AffPoint(-9.0, 7.0, bellR),
		cryph::AffPoint(-6.5, 9.5, bellR), barR, bellR) );
	c.addModel( new Barbell(sIF, barbellMatl, cryph::AffPoint(8.0, -7.0, bellR),
		cryph::AffPoint(8.0, -3.5, bellR), barR, bellR) );

}

void set3DViewingInformation(double xyz[6])
{
	ModelView::setMCRegionOfInterest(xyz);

	// cryph::AffPoint eye, center;
	// cryph::AffVector up;

	// Set values for eye-center-up to produce a reasonable off-axis
	// view of your scene, then:
	// COMMON HEURISTIC FOR ESTABLISHING THE MC ==> EC TRANSFORMATION:
	// Create the line of sight through the center of the scene:
	// 1) Make the center of attention be the center of the bounding box.
	// 2) Move the eye away along some direction - here (0,0,1) - so that the
	//    distance between the eye and the center is (2*max scene dimension).
	// 3) Set the "up" direction vector to orient the 3D view
	// NOTE: ModelView::getMatrices - used during display callbacks -
	//       implicitly assumes the line of sight passes through what we want
	//       to have in the center of the display window.

	// 1:
	double xmid = 0.5 * (xyz[0] + xyz[1]);
	double ymid = 0.5 * (xyz[2] + xyz[3]);
	double zmid = 0.5 * (xyz[4] + xyz[5]);
	cryph::AffPoint center(xmid, ymid, zmid);

	// 2:
	double maxDelta = xyz[1] - xyz[0];
	double delta = xyz[3] - xyz[2];
	if (delta > maxDelta)
		maxDelta = delta;
	delta = xyz[5] - xyz[4];
	if (delta > maxDelta)
		maxDelta = delta;
	double distEyeCenter = 2.0 * maxDelta;


	//air view
	// cryph::AffPoint eye(xmid, ymid, zmid + 1.3*distEyeCenter);
	// cryph::AffVector up = cryph::AffVector::yu;
	// 3:

	// side View
	cryph::AffPoint eye(xmid-5, ymid-20, zmid + 0.5*distEyeCenter);
	cryph::AffVector up = cryph::AffVector::zu;


	// Notify the ModelView of our MC->EC viewing requests:
	ModelView::setEyeCenterUp(eye, center, up);

	double ecZpp, ecZmin, ecZmax;

	ecZpp = -(distEyeCenter - 0.5*maxDelta);
	ModelView::setProjectionPlaneZ(ecZpp);
	ecZmin = ecZpp - 1.5*maxDelta;
	// ecZmax = ecZpp + 0.5*maxDelta;
	// std::cout << "ecZmax: ======> " << ecZmax << "\n";
	ecZmax = ecZpp + 0.5*maxDelta;
	std::cout << "ZMIN: " << ecZmin << "\n";
	std::cout << "ZMAX: " << ecZmax << "\n";
	ModelView::setECZminZmax(ecZmin, ecZmax);

	// Set values for ecZpp, ecZmin, ecZmax that make sense in the context of
	// the EC system established above, then:

	ModelView::setProjection(PERSPECTIVE);
	ModelView::setProjectionPlaneZ(ecZpp);
	ModelView::setECZminZmax(ecZmin, ecZmax);
}

int main(int argc, char* argv[])
{
	GLFWController c("BEACH HOUSE", MVC_USE_DEPTH_BIT);
	c.reportVersions(std::cout);

	ShaderIF* sIF = new ShaderIF("shaders/basic.vsh", "shaders/phong.fsh");

	// create your scene, adding things to the Controller....
	createRoom(c, sIF);

	glClearColor(0.5, 0.5, 1.0, 1.0);

	double xyz[6];
	c.getOverallMCBoundingBox(xyz);
	set3DViewingInformation(xyz);

	c.run();

	delete sIF;

	return 0;
}
