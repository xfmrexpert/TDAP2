#pragma once

#include "ElementTransform.h"

class AxiElementTransform : public ElementTransform
{
public:
	AxiElementTransform(size_t nsd, size_t npd, int order = 1)
		: ElementTransform(nsd, npd, order)
	{
	};
	
	Matrix<double> Jacobian(const point& pt_ref, const MeshEntity& entity) const override {
		// [ds_dxi, ds_deta]
		// [dz_dxi, dz_deta]
		Matrix<double> dG_ds = shape_function->grad_N(pt_ref);

		// Initialize a nsd x npd Jacobian matrix with zeros
		Matrix<double> dXds = Matrix<double>::Zero(numSpatialDims, numParametricDims);

		auto nodes = entity.getNodes(); // Assumes nodes are ordered
		size_t nen = nodes.size(); // Number of element nodes

		for (size_t i = 0; i < nen; i++) { // Loop over shape functions (one per node)
			for (int k = 0; k < numParametricDims; k++) { // Loop over local (shape) dimension (xi, eta)
				dXds(0, k) += dG_ds(i, k) * (nodes[i]->pt().x * nodes[i]->pt().x); // Nodal point valus are in r, so need to square to get s
				dXds(1, k) += dG_ds(i, k) * nodes[i]->pt().y;
			}
		}

		return dXds;
	};

	point mapReferencePointToPhysical(const point& ptRef, const MeshEntity& entity) const override
	{
		point pt_trans; // ptRef in s, z coordinates
		const auto& N_ref = shape_function->N(ptRef);
		auto nodes = entity.getNodes();
		for (size_t i = 0; i < nodes.size(); i++) {
			pt_trans.x += N_ref(i) * nodes[i]->pt().x * nodes[i]->pt().x; // Nodal point valus are in r, so need to square to get s
			pt_trans.y += N_ref(i) * nodes[i]->pt().y;
		}
		
		return pt_trans;
	}
};

//struct UseAxiElementTransform {};
//
//// Specialize the traits with the tag
//template <>
//struct ElementTransformTraits<LagrangeShapeFunction, UseAxiElementTransform> {
//	using TransformType = AxiElementTransform;
//
//	static std::unique_ptr<TransformType> createTransform(size_t spatial_dim, size_t ref_dim, int order = 1) {
//		return std::make_unique<TransformType>(spatial_dim, ref_dim, order);
//	}
//};