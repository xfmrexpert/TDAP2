/***************************************************************************
 *   Copyright (C) 2005-2024 by T. C. Raymond                              *
 *   tcraymond@inductivereasoning.com                                      *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#pragma once

#include "meshentity.h"
#include <vector>
#include "meshface.h"
#include "meshedge.h"
#include "meshvertex.h"
#include "node.h"

using namespace std;

class MeshFace;


class MeshRegion : public MeshEntity {

public:

	MeshRegion() {};

	~MeshRegion() {};

	void addFace(MeshFace* new_face);

	static const int dimensions = 3;

	int get_dimensions() const override {
		return 3;
	}

	vector<Node*> getNodes() const override;

protected:

private:

	vector<MeshFace*> MeshFaces;

};

