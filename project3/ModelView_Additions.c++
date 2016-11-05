// Extracted from: ModelView.c++ - an Abstract Base Class for a combined Model and View for OpenGL

#include "ModelView.h"

void ModelView::addToGlobalPan(double dxInLDS, double dyInLDS, double dzInLDS)
{
	// TODO: Delete or comment out the following std::cout statement when
	//       everything else is done here.
	std::cout << "In project 3, you will implement ModelView::addToPan in ModelView_Additions.c++\n";
	// TODO: 1. UPDATE dynamic_view
	// TODO: 2. Use dynamic_view in ModelView::getMatrices
}

void ModelView::addToGlobalRotationDegrees(double rx, double ry, double rz)
{
	cryph::Matrix4x4 rotationX = cryph::Matrix4x4::xRotationDegrees(ry);
	cryph::Matrix4x4 rotationY = cryph::Matrix4x4::xRotationDegrees(rx);
	cryph::Matrix4x4 rotationZ = cryph::Matrix4x4::xRotationDegrees(rz);

	dynamic_view = rotationX * rotationY * rotationZ * dynamic_view;
}

void ModelView::getMatrices(cryph::Matrix4x4& mc_ec, cryph::Matrix4x4& ec_lds)
{
	// std::cout << "You must implement ModelView::getMatrices in ModelView_Additions.c++\n";
	// TODO:
	// 1. Create the mc_ec matrix:
	//    Create a local variable of type Matria4x4 calledx M_ECu from the eye,
	//    center, and up. Recall that those values are set in the main program
	//    and are stored in protected class variables that you directly access
	//    (ModelView::eye, ModelView::center, and ModelView::up). You can use the
	//    following utility from Matrix4x4:
	//
	//    cryph::Matrix4x4 cryph::Matrix4x4::lookAt(
	//          const cryph::AffPoint& eye, const cryph::AffPoint& center,
	//          const cryph::AffVector& up);
	//
	//    NOTE: eye, center, and up are specified in MODEL COORDINATES (MC)
	//
	//    a) For project 2: mc_ec = M_ECu
	//    b) For project 3: mc_ec = dynamic_view * M_ECu
	//

	cryph::Matrix4x4 M_ECu = cryph::Matrix4x4::lookAt(eye, center, up);
	mc_ec = dynamic_view * M_ECu;

	// 2. Create the ec_lds matrix:
	//    Use the mcRegionOfInterest (again, set in main; accessible to you via
	//    the protected ModelView class variable ModelView::mcRegionOfInterest) to
	//    determine the radius of the circumscribing sphere. Recall that this radius
	//    is used to determine an initial ecXmin, ecXmax, ecYmin, and ecYmax.
	//      i) Adjust the width in the x OR y direction to match the viewport aspect ratio;
	//     ii) Project 3 and 4 only: Scale both widths by dynamic_zoom;
	//    iii) Use the adjusted widths along with the ecZmin, ecZmax, and ecZpp set
	//         in main and stored in the ModelView class variables to create the
	//         ec_lds matrix. Use the class variable ModelView::projType to determine
	//         which of the routines below to use. (Also use ModelView::obliqueProjectionDir
	//         if the projType is OBLIQUE.https://people.eecs.ku.edu/~miller/Courses/672/Fall2016/Projects/Project02.html
	//
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// !!!!!   All coordinate data in the parameter lists below are specified      !!!!!!
	// !!!!!   in EYE COORDINATES (EC)! Be VERY sure you understand what that      !!!!!!
	// !!!!!   means!  (This is why I emphasized "WIDTHS" above.)                  !!!!!!
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	/* The three ec_lds matrix generation choices:

	cryph::Matrix4x4 cryph::Matrix4x4::orthogonal(double ecXmin, double ecXmax,
		double ecYmin, double ecYmax, double ecZmin, double ecZmax);

	cryph::Matrix4x4 cryph::Matrix4x4::perspective(double ecZpp, double ecXmin, double ecXmax,
		double ecYmin, double ecYmax, double ecZmin, double ecZmax);

	cryph::Matrix4x4 cryph::	//    For project 3: mc_ec = dynamic_view * M_ECu& projDir);

	*/

	// THEN IN THE CALLER OF THIS METHOD:
	//
	// float mat[16];
	// glUniformMatrix4fv(shaderIF->ppuLoc("mc_ec"), 1, false, mc_ec.extractColMajor(mat));
	// glUniformMatrix4fv(shaderIF->ppuLoc("ec_lds"), 1, false, ec_lds.extractColMajor(mat));
	//
	// (The extractColMajor method copies the elements of the matrix into the given
	// array which is assumed to be of length 16. It then returns the array pointer
	// so it can be used as indicated in the two calls. Since the array is immediately
	// copied by glUniformMatrix to the GPU, "mat" can be reused as indicated.)

	// double maxDelta = 0.0; // TODO: compute this as just described.
	// double halfWidth = 0.5 * maxDelta;

	double xmin = mcRegionOfInterest[0];
	double xmax = mcRegionOfInterest[1];
	double ymin = mcRegionOfInterest[2];
	double ymax = mcRegionOfInterest[3];
	double zmin = mcRegionOfInterest[4];
	double zmax = mcRegionOfInterest[5];

	double xmid = (xmin + xmax) / 2;
	double ymid = (ymin + ymax) / 2;
	double zmid = (zmin + zmax) / 2;

	double rad = sqrt(pow((xmax - xmin), 2) + pow((ymax - ymin), 2) + pow((zmax - zmin), 2))
		* dynamic_zoomScale;

	double ecXmin = xmid - rad;
	double ecXmax = xmid + rad;
	double ecYmin = ymid - rad;
	double ecYmax = ymid + rad;

	double vAR = Controller::getCurrentController()->getViewportAspectRatio();
	ModelView::matchAspectRatio(ecXmin, ecXmax, ecYmin, ecYmax, vAR);

	if (ModelView::projType == ORTHOGONAL)
		ec_lds = cryph::Matrix4x4::orthogonal(ecXmin, ecXmax, ecYmin, ecYmax,
				ModelView::ecZmin, ModelView::ecZmax);
	else if (ModelView::projType == PERSPECTIVE)
		ec_lds = cryph::Matrix4x4::perspective(ModelView::ecZpp,
				ecXmin, ecXmax, ecYmin, ecYmax, ModelView::ecZmin, ModelView::ecZmax);
	else // Must be OBLIQUE
		ec_lds = cryph::Matrix4x4::oblique(ModelView::ecZpp,
				ecXmin, ecXmax, ecYmin, ecYmax, ModelView::ecZmin, ModelView::ecZmax,
				ModelView::obliqueProjectionDir);
}

void ModelView::scaleGlobalZoom(double multiplier)
{
	if (multiplier != 0)
		dynamic_zoomScale *= multiplier;
	else
		std::cout << "Max zoom reached.\n";

}
