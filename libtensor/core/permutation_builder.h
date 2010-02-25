#ifndef LIBTENSOR_PERMUTATION_BUILDER_H
#define LIBTENSOR_PERMUTATION_BUILDER_H

#include "../defs.h"
#include "../exception.h"
#include "permutation.h"

namespace libtensor {


/**	\brief Builds a %permutation using the original and a permuted
		sequences
	\tparam N Permutation order.

	Using two sequences of the same set of objects, permutation_builder
	constructs the reverse %permutation of the second sequence with
	respect to the first (reference) sequence. The produced %permutation
	when applied to the second sequence will yield the first sequence.

	The two sequences must represent the same set of unique objects as
	determined by operator==, otherwise an %exception will be thrown
	upon the construction of permutation_builder.

	Example:
	\code
	char ab[] = { 'a', 'b' };
	char ba[] = { 'b', 'a' };
	permutation_builder<2> pb(ab, ba);

	int seq[2]; seq[0] = 2; seq[1] = 3;
	pb.get_perm().apply(2, seq); // Permutes the elements of seq

	// seq is now seq[2] = { 3, 2 }
	\endcode

	\ingroup libtensor_core
 **/
template<size_t N>
class permutation_builder {
private:
	static const char *k_clazz; //!< Class name

private:
	permutation<N> m_perm; //!< Built %permutation

public:
	/**	\brief Constructs a %permutation using two sequences of
			objects
		\tparam T Object type (must support operator ==).
		\param seq1 First (reference) sequence.
		\param seq2 Second sequence.
	 **/
	template<typename T>
	permutation_builder(const T (&seq1)[N], const T (&seq2)[N]);

	/**	\brief Constructs a %permutation using two sequences of
			objects and an %index mapping law
		\tparam T Object type (must support operator ==).
		\param seq1 First (reference) sequence.
		\param seq2 Second sequence.
		\param perm Index mapping.
	 **/
	template<typename T>
	permutation_builder(const T (&seq1)[N], const T (&seq2)[N],
		const permutation<N> &perm);

	/**	\brief Returns the %permutation
	 **/
	const permutation<N> &get_perm() const {
		return m_perm;
	}

private:
	template<typename T>
	void build(const T (&seq1)[N], const T (&seq2)[N],
		const size_t (&map)[N]);

};


template<size_t N>
const char *permutation_builder<N>::k_clazz = "permutation_builder<N>";


template<size_t N> template<typename T>
permutation_builder<N>::permutation_builder(
	const T (&seq1)[N], const T (&seq2)[N]) {

	size_t map[N];
	for(size_t i = 0; i < N; i++) map[i] = i;
	build(seq1, seq2, map);
}


template<size_t N> template<typename T>
permutation_builder<N>::permutation_builder(
	const T (&seq1)[N], const T (&seq2)[N], const permutation<N> &perm) {

	size_t map[N];
	for(size_t i = 0; i < N; i++) map[i] = i;
	permutation<N> permi(perm, true);
	permi.apply(map);
	build(seq1, seq2, map);
}


template<size_t N> template<typename T>
void permutation_builder<N>::build(const T (&seq1)[N], const T (&seq2)[N],
	const size_t (&map)[N]) {

	static const char *method =
		"build(const T (&)[N], const T (&)[N], const size_t (&)[N])";

	size_t i, j, idx[N];

	for(i = 0; i < N; i++) {

		for(j = i + 1; j < N; j++) {
			if(seq1[i] == seq1[j]) {
				//	Duplicate object
				throw bad_parameter(g_ns, k_clazz, method,
					__FILE__, __LINE__, "seq1");
			}
		}
		for(j = 0; j < N; j++) {
			if(seq2[j] == seq1[i]) {
				idx[i] = j;
				break;
			}
		}
		if(j == N) {
			//	Object sets differ
			throw bad_parameter(g_ns, k_clazz, method,
				__FILE__, __LINE__, "seq2");
		}
	}

	i = 0;
	while(i < N) {
		if(i > idx[i]) {
			m_perm.permute(map[i], map[idx[i]]);
			j = idx[i];
			idx[i] = idx[j];
			idx[j] = j;
			i = 0;
		} else {
			i++;
		}
	}
	//	The loop above generates the inverse of the permutation
	m_perm.invert();
}


} // namespace libtensor

#endif // LIBTENSOR_PERMUTATION_BUILDER_H
