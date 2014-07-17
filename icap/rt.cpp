
#include "icap.h"


void ICAP::SetCurrentNodeInflow(const std::string& nodeId, var_type flow)
{
    m_model->setRealTimeInputFlow(nodeId, flow);
}


var_type ICAP::GetCurrentNodeHead(const std::string& nodeId)
{
    return m_model->getRealTimeNodeHead(nodeId);
}


void ICAP::AddSource(const std::string& nodeId)
{
    m_model->addRealTimeInput(nodeId);
}


void ICAP::SetFlowFactor(var_type flowFactor)
//
//  Input:   flow factor
//  Output:  none
//  Purpose: modifies scale factor on inflows
//
{
    for (geometry::Geometry::NodeIter iter = m_geometry->beginNode(); iter != m_geometry->endNode(); iter++)
    {
        iter->second->setInflowFactor(flowFactor);
    }
}