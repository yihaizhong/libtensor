#ifndef LIBTENSOR_GEN_BTO_AUX_CHSYM_H
#define LIBTENSOR_GEN_BTO_AUX_CHSYM_H

#include "gen_block_stream_i.h"

namespace libtensor {


/** \brief Transforms target blocks and relays them to a target stream with
        lower symmetry (auxiliary operation)
    \tparam N Tensor order.
    \tparam Traits Block tensor operation traits structure.

    This auxiliary block tensor operation accepts blocks and relays them
    into a target symmetry subgroup of the origin symmetry group.

    \sa gen_block_stream_i

    \ingroup libtensor_gen_bto
 **/
template<size_t N, typename Traits>
class gen_bto_aux_chsym :
    public gen_block_stream_i<N, typename Traits::bti_traits> {

public:
    static const char k_clazz[]; //!< Class name

public:
    //! Type of tensor elements
    typedef typename Traits::element_type element_type;

    //! Block tensor interface traits
    typedef typename Traits::bti_traits bti_traits;

    //! Type of read-only block
    typedef typename bti_traits::template rd_block_type<N>::type rd_block_type;

    //! Type of write-only block
    typedef typename bti_traits::template wr_block_type<N>::type wr_block_type;

    //! Type of symmetry
    typedef symmetry<N, element_type> symmetry_type;

    //! Type of tensor transformation
    typedef tensor_transf<N, element_type> tensor_transf_type;

private:
    symmetry_type m_syma; //!< Source symmetry
    symmetry_type m_symb; //!< Target symmetry
    gen_block_stream_i<N, bti_traits> &m_out; //!< Output stream
    bool m_open; //!< Open state

public:
    /** \brief Constructs the operation
        \brief syma Original symmetry.
        \brief symb Target symmetry.
        \brief out Output stream.
     **/
    gen_bto_aux_chsym(
        const symmetry_type &syma,
        const symmetry_type &symb,
        gen_block_stream_i<N, bti_traits> &out);

    /** \brief Virtual destructor
     **/
    virtual ~gen_bto_aux_chsym();

    /** \brief Implements bto_stream_i::open(). Prepares the operation
     **/
    virtual void open();

    /** \brief Implements bto_stream_i::close()
     **/
    virtual void close();

    /** \brief Implements bto_stream_i::put(). Relays transformed blocks to
            the output stream
     **/
    virtual void put(
        const index<N> &idx,
        rd_block_type &blk,
        const tensor_transf_type &tr);

};


} // namespace libtensor

#endif // LIBTENSOR_GEN_BTO_AUX_CHSYM_H
