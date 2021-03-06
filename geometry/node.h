// ==============================================================================
// ICAP License
// ==============================================================================
// University of Illinois/NCSA
// Open Source License
// 
// Copyright (c) 2014-2016 University of Illinois at Urbana-Champaign.
// All rights reserved.
// 
// Developed by:
// 
//     Nils Oberg
//     Blake J. Landry, PhD
//     Arthur R. Schmidt, PhD
//     Ven Te Chow Hydrosystems Lab
// 
//     University of Illinois at Urbana-Champaign
// 
//     https://vtchl.illinois.edu
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal with
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is furnished to do
// so, subject to the following conditions:
// 
//     * Redistributions of source code must retain the above copyright notice,
//       this list of conditions and the following disclaimers.
// 
//     * Redistributions in binary form must reproduce the above copyright notice,
//       this list of conditions and the following disclaimers in the
//       documentation and/or other materials provided with the distribution.
// 
//     * Neither the names of the Ven Te Chow Hydrosystems Lab, University of
// 	  Illinois at Urbana-Champaign, nor the names of its contributors may be
// 	  used to endorse or promote products derived from this Software without
// 	  specific prior written permission.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS WITH THE
// SOFTWARE.

#ifndef NODE_H__
#define NODE_H__

#include <string>
#include <vector>
#include <map>

#include "../util/parseable.h"
#include "../model/modelElement.h"

#include "inflow.h"


namespace geometry
{

    enum NodeType
    {
        NodeType_Junction,
        NodeType_Storage,
    };

    class Link;

    class Node : public IModelElement, public Parseable
    {
    protected:
        std::vector<std::shared_ptr<Inflow>> inflows;

        std::vector<std::shared_ptr<Link>> usLinks;
        std::vector<std::shared_ptr<Link>> dsLinks;

        double computeLateralInflow(const DateTime& dateTime);

        std::shared_ptr<Model> theModel;

        std::string name;
        id_type id;
        bool canFlood;

        std::map<variables::Variables, double> simData;

        var_type invertElev;
        var_type maxDepth;
        var_type initDepth;
        NodeType nodeType;
        var_type xCoord, yCoord;

        /// <summary>
        /// The total inflow to this node: all upstream conduit inflows as well
        /// as any lateral inflows.
        /// </summary>
        //var_type nodeInflow;

        /// <summary>
        /// The lateral inflow to this node only; the sum of all inflows from external
        /// inflows for the given time ste.p    
        /// </summary>
        //var_type lateralInflow;
        //var_type depth;
        //var_type volume;

    protected:
        Node(const id_type& theId, const std::string& theName, NodeType theType);

        bool baseParseLine(const std::vector<std::string>& parts);
        

    public:
        ~Node();

        typedef std::vector<std::shared_ptr<Link>>::iterator LinkIter;

        LinkIter beginUpstreamLink();
        LinkIter endUpstreamLink();

        var_type getXCoord() { return this->xCoord; }
        var_type getYCoord() { return this->yCoord; }
        var_type getInvert() { return this->invertElev; }
        var_type getMaxDepth() { return this->maxDepth; }
        std::string getName() { return this->name; }
        id_type getId() { return this->id; }
        bool getCanFlood() { return this->canFlood; }
        NodeType getType() { return this->nodeType; }
        var_type getInitialDepth() { return this->initDepth; }
        void setInitialDepth(var_type value) { this->initDepth = value; }

        /// <summary>
        /// Returns the volume stored in this node for the given depth.
        /// </summary>
        virtual var_type lookupVolume(var_type depth) { return 0; }

        /// <summary>
        /// Returns 0 if there are not enough upstream links; returns the largest angle between
        /// the two upstream mainIdx and lateralIdx link indices.
        /// </summary>
        var_type computeUpstreamLinksAngle(int downIdx, int mainIdx, int lateralIdx);

        const std::vector<std::shared_ptr<Link>>& getUpstreamLinks() const;
        const std::vector<std::shared_ptr<Link>>& getDownstreamLinks() const;

        /// <summary>
        /// Add an upstream conduit to this node's connectivity.
        /// </summary>
        void addUpstreamLink(std::shared_ptr<Link> link) { this->usLinks.push_back(link); }

        /// <summary>
        /// Add a downstream conduit to this node's connectivity.
        /// </summary>
        void addDownstreamLink(std::shared_ptr<Link> link) { this->dsLinks.push_back(link); }
        
        /// <summary>
        /// Add an external inflow object to this node.
        /// </summary>
        void attachInflow(std::shared_ptr<Inflow> inflow);

        /// <summary>
        /// Clear all inflow objects.
        /// </summary>
        void clearInflowObjects();

        /// <summary>
        /// Modifies any attached inflow scale factors to the given factor.
        /// </summary>
        void setInflowFactor(var_type flowFactor);
        
        /// <summary>
        /// This function takes any external inflows at this node and propogates them downstream
        /// in a steady-state fashion.
        /// </summary>
        void startInflow(const DateTime& dateTime);
        
        /// <summary>
        /// This function takes an upstream pipe inflow and propogates it downstream to the next
        /// pipe(s).
        /// </summary>
        void propogateFlowDownstream(var_type flow);
        
        /// <summary>
        /// This function resets the inflow parameters (e.g. at the start of a timestep).
        /// </summary>
        virtual void resetFlow();

        /// <summary>
        /// This function resets the depth parameter.
        /// </summary>
        virtual void resetDepth();
        
        /// <summary>
        /// Parse a given line (in the form of a vector of individual line parts) and store
        /// the parameters into this current object.
        /// </summary>
        virtual bool parseLine(const std::vector<std::string>& parts) = 0;
        
        /// <summary>
        /// Parse a coordinate line (in the form of a vector of individual line parts).
        /// </summary>
        bool parseCoordLine(const std::vector<std::string>& parts);
        
        virtual var_type& variable(variables::Variables var);

        //virtual void propagateDepthUpstream(var_type depth);

        std::shared_ptr<Inflow> getInflow();

        var_type getDownstreamLinkMaxDepth();


        virtual void setErrorMessage(const std::string& msg) { Parseable::setErrorMessage("[" + this->name + "] " + msg); }
        virtual void appendErrorMessage(const std::string& msg) { Parseable::appendErrorMessage("[" + this->name + "] " + msg); }
    };

}


#endif//NODE_H__
