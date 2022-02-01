/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Thursday, October 12, 2017 - 11:10:04
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <labtransformations/labtransformationsmodule.h>
#include <labtransformations/cubeanimator.h>

namespace inviwo
{

LabTransformationsModule::LabTransformationsModule(InviwoApplication* app) : InviwoModule(app, "LabTransformations")

{
	registerProcessor<CubeAnimator>();
}

} // namespace
