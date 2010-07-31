#ifndef LIBTENSOR_SO_PROJ_DOWN_IMPL_LABEL_H
#define LIBTENSOR_SO_PROJ_DOWN_IMPL_LABEL_H

#include "../defs.h"
#include "../exception.h"
#include "symmetry_element_set_adapter.h"
#include "symmetry_operation_impl_base.h"
#include "so_proj_down.h"
#include "se_label.h"

namespace libtensor {


/**	\brief Implementation of so_proj_down<N, T> for se_label<N, T>
	\tparam N Tensor order.
	\tparam T Tensor element type.

	This implementation sets the target label to all labels.

	\ingroup libtensor_symmetry
 **/
template<size_t N, size_t M, typename T>
class symmetry_operation_impl< so_proj_down<N, M, T>, se_label<N, T> > :
	public symmetry_operation_impl_base< so_proj_down<N, M, T>, se_label<N, T> > {

public:
	static const char *k_clazz; //!< Class name

public:
	typedef so_proj_down<N, M, T> operation_t;
	typedef se_label<N, T> element_t;
	typedef symmetry_operation_params<operation_t>
		symmetry_operation_params_t;

protected:
	virtual void do_perform(symmetry_operation_params_t &params) const;
};


template<size_t N, size_t M, typename T>
const char *symmetry_operation_impl< so_proj_down<N, M, T>, se_label<N, T> >::k_clazz =
	"symmetry_operation_impl< so_proj_down<N, M, T>, se_label<N, T> >";


template<size_t N, size_t M, typename T>
void symmetry_operation_impl< so_proj_down<N, M, T>, se_label<N, T> >::do_perform(
	symmetry_operation_params_t &params) const {

	static const char *method =
		"do_perform(const symmetry_operation_params_t&)";

	typedef symmetry_element_set_adapter< N, T, se_label<N, T> > adapter_t;

	//	Verify that the projection mask is correct
	//
	const mask<N> &m = params.msk;
	size_t map[N];
	size_t nm = 0;
	for(size_t i = 0; i < N; i++) if(m[i]) nm++;
	if(nm != N - M) {
		throw bad_parameter(g_ns, k_clazz, method, __FILE__, __LINE__,
			"params.msk");
	}

	adapter_t adapter1(params.grp1);

	params.grp2.clear();

	// create block index dimensions of result se_label
	typename adapter_t::iterator it1 = adapter1.begin();

	if (it1 == adapter1.end()) return;

	const dimensions<N> &bidims1 =
			adapter1.get_elem(it1).get_block_index_dims();

	index<N - M> idx1, idx2;
	for (size_t i = 0, j = 0; i < N; i++)
		if (m[i]) idx2[j++] = bidims1[i] - 1;

	dimensions<N - M> bidims2(index_range<N - M>(idx1, idx2));

	for (; it1 != adapter1.end(); it1++) {

#ifdef LIBTENSOR_DEBUG
		if (bidims1 != adapter1.get_elem(it1).get_block_index_dims())
			throw bad_symmetry(g_ns, k_clazz, method,
					__FILE__, __LINE__, "Incompatible se_labels in input.");
#endif

		// create new se_label element
		const se_label<N, T> &se1 = adapter1.get_elem(it1);

		se_label<N - M, T> se2(bidims2, se1.get_table_id());
		for (size_t i = 0, j = 0; i < N; i++) {
			if (! m[i]) continue;

			mask<N - M> msk;
			msk[j] = true;
			size_t itype = se1.get_dim_type(i);
			size_t jtype = se2.get_dim_type(j);
			for (size_t k = 0; k < se2.get_dim(jtype); k++)
				se2.assign(msk, k, se1.get_label(itype, k));
			j++;
		}
		se2.match_labels();

		for (size_t i = 0; i < se1.get_n_labels(); i++)
			se2.add_target(i);

		params.grp2.insert(se2);
	}
}


} // namespace libtensor

#endif // LIBTENSOR_SO_PROJ_DOWN_IMPL_PERM_H