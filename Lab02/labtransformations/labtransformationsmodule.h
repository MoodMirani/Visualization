/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Thursday, October 12, 2017 - 11:10:04
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#pragma once

#include <labtransformations/labtransformationsmoduledefine.h>
#include <inviwo/core/common/inviwomodule.h>

namespace inviwo
{

class IVW_MODULE_LABTRANSFORMATIONS_API LabTransformationsModule : public InviwoModule
{
public:
    LabTransformationsModule(InviwoApplication* app);
    virtual ~LabTransformationsModule() = default;
};

} // namespace
