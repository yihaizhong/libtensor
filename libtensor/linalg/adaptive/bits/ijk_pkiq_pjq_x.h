#ifndef LIBTENSOR_LINALG_ADAPTIVE_IJK_PKIQ_PJQ_X_H
#define LIBTENSOR_LINALG_ADAPTIVE_IJK_PKIQ_PJQ_X_H

#include "trp_ijk_jik.h"
#include "trp_ijkl_kjil.h"

namespace libtensor {


template<typename M, typename L1, typename L2, typename L3>
void linalg_base_level5_adaptive<M, L1, L2, L3>::ijk_pkiq_pjq_x(
	size_t ni, size_t nj, size_t nk, size_t np, size_t nq,
	const double *a, const double *b, double *c, double d) {

	size_t npq = np * nq;
	size_t nik = ni * nk;
	size_t npq1 = (npq % 4 == 0) ? npq : npq + 4 - npq % 4;
	size_t nik1 = (nik % 4 == 0) ? nik : nik + 4 - nik % 4;

	double *a1 = M::allocate(nik * npq1);
	double *b1 = M::allocate(nj * npq1);
	double *c1 = M::allocate(nj * nik1);

	//	a1_ikpq <- a_pkiq
	trp_ijkl_kjil::transpose(ni, nk, np, nq, a, ni * nq, a1, npq1);

	//	b1_jpq <- b_pjq
	trp_ijk_jik::transpose(nj, np, nq, b, nj * nq, b1, npq1);

	//	c1_jik <- c_ijk
	trp_ijk_jik::transpose(nj, ni, nk, c, nj * nk, c1, nik1);

	//	c1_jik += d * b1_jpq a1_ikpq
	L3::ij_ip_jp_x(nj, nik, npq, b1, npq1, a1, npq1, c1, nik1, d);

	//	c_ijk <- c1_jik
	trp_ijk_jik::transpose(ni, nj, nk, c1, nik1, c, nj * nk);

	M::deallocate(c1);
	M::deallocate(b1);
	M::deallocate(a1);
}


} // namespace libtensor

#endif // LIBTENSOR_LINALG_ADAPTIVE_IJK_PKIQ_PJQ_X_H