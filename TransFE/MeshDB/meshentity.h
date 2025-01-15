/***************************************************************************
 *   Copyright (C) 2005-2025 by T. C. Raymond                              *
 *   tcraymond@inductivereasoning.com                                      *
 *                                                                         *
 *   Use of this source code is governed by an MIT-style                   *
 *   license that can be found in the LICENSE.txt file or at               *
 *   https://opensource.org/licenses/MIT.                                  *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#pragma once

#include "node.h"
#include <vector>
#include <memory>
#include <optional>

#include "geomentity.h"

class Node;
class MeshEdge;

 /// This is the class that all mesh entity classes (Regions, Faces, etc.) inherit from.
 /// This class contains basic functions for getting and setting classifications, setting an
 /// integer ID, and assigning a node to the entity.  By allowing a node to be assigned to 
 /// any entity, we can begin to allow higher-order elements.  This would be further 
 /// facilitated by allowing more than one node per entity.  Note that all vertices must
 /// contain at least one node.

class MeshEntity {

public:

	/// Default constructor.  Currently empty. Not anymore...
	MeshEntity() {
		node = nullptr;
		ID = -1;
	}

	virtual ~MeshEntity() = default;

	/// Returns the number of dimensions for an entity.  This is useful in identifying the entity type.
	virtual int get_dimensions() const = 0;

	Node* getNode() const {
		return node;
	}

	void setNode(Node& in_node) {
		node = &in_node;
		node->setParent(this);
	}

	/// Returns the GeomEntity that this mesh entity is classified on
	GeomEntity* getClassification() const {
		return myClassification;
	}

	/// Sets the GeomEntity that this mesh entity is classified on
	void setClassification(GeomEntity& new_class) {
		myClassification = &new_class;
	}

	int getClassificationID() const {
		if (myClassification) {
			return myClassification->ID;
		}
		return -1;
	}

	/// Returns an ordered list of nodes for the mesh entity
	virtual std::vector<Node*> getNodes() const = 0;

	virtual std::vector<MeshEdge*> getEdges() const = 0;

	size_t getID() const {
		return ID;
	}

	void setID(size_t in_ID) {
		ID = in_ID;
	}

protected:

	/// Pointer to a node.  Required for vertices, optional for all others.
	Node* node;

private:

	/// An integer for assigned a unique ID to an entity.  This is not assigned unless done so explicitly.
	size_t ID;

	/// variable for number of dimensions
	int dimensions;

	/// GeomEntity representing the model object this mesh entity is classified on
	GeomEntity* myClassification;

};


