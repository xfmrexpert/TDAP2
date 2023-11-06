/***************************************************************************
 *   Copyright (C) 2005 by T. C. Raymond                                   *
 *   tc.raymond@ieee.org                                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#ifndef MESHENTITY_H
#define MESHENTITY_H

#include "node.h"
#include <vector>
#include <memory>

#include "geomentity.h"

 /// This is the class that all mesh entity classes (Regions, Faces, etc.) inherit from.
 /// This class contains basic functions for getting and setting classifications, setting an
 /// integer ID, and assigning a node to the entity.  By allowing a node to be assigned to 
 /// any entity, we can begin to allow higher-order elements.  This would be further 
 /// facilitated by allowing more than one node per entity.  Note that all vertices must
 /// contain at least one node.

class MeshEntity {

public:

	/// An integer for assigned a unique ID to an entity.  This is not assigned unless done so explicitly.
	long long ID;

	/// Default constructor.  Currently empty. Not anymore...
	MeshEntity();

	virtual ~MeshEntity() = default;

	/// Returns the number of dimensions for an entity.  This is useful in identifying the entity type.
	virtual int get_dimensions() const = 0;

	/// Pointer to a node.  Required for vertices, optional for all others.
	shared_ptr<Node> node;

	/// Returns the GeomEntity that this mesh entity is classified on
	shared_ptr<GeomEntity> getClassification();

	/// Sets the GeomEntity that this mesh entity is classified on
	void setClassification(shared_ptr<GeomEntity>);

	/// Returns an ordered list of nodes for the mesh entity
	virtual vector<shared_ptr<Node>> getNodes() = 0;

protected:

private:

	/// variable for number of dimensions
	int dimensions;

	/// GeomEntity representing the model object this mesh entity is classified on
	shared_ptr<GeomEntity> myClassification;

};
#endif //MESHENTITY_H

