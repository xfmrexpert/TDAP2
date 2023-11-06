/************************************************************************
			meshregion.h - Copyright T. C. Raymond


**************************************************************************/

#ifndef MESHREGION_H
#define MESHREGION_H

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

	vector< pair<shared_ptr<MeshFace>, bool> >::iterator getFirstFace();
	vector< pair<shared_ptr<MeshFace>, bool> >::iterator getLastFace();

	void addFace(pair<shared_ptr<MeshFace>, bool> new_face);

	static const int dimensions = 3;

	int get_dimensions() const {
		return 3;
	}

	virtual vector<shared_ptr<Node>> getNodes();

protected:

private:

	vector<pair<shared_ptr<MeshFace>, bool>> MeshFaces;

};
#endif //MESHREGION_H

