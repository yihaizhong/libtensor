#ifndef BATCH_PROVIDER_NEW_H
#define BATCH_PROVIDER_NEW_H

#include "gen_sparse_btensor.h"
#include "batch_kernel_permute.h"
#include "../expr/dag/node_assign.h"
#include "../expr/dag/node_transform.h"
#include "../expr/dag/node_contract.h"
#include "../expr/metaprog.h"

namespace libtensor {

namespace expr {

//Can't do this with a function because C++03 won't let function templates have default template args
template<typename T,size_t NC=0,size_t NA=0,size_t NB=0>
class kernel_builder
{
private:
    const expr_tree& m_tree;
    const expr_tree::edge_list_t& m_edges;
    batch_kernel<T>** m_kern; //Have to cache in a member var to pass through dispatch_1
    std::vector<T*> m_ptrs;
public:
    std::vector<T*> get_ptrs() { return m_ptrs; }
    void build_kernel(batch_kernel<T>*& kern)
    {
        m_kern = &kern;
        size_t n_tensors_processed = (NC == 0 ? 0 : (NA == 0 ? 1 : (NB == 0 ? 2 : 3)));
        if(n_tensors_processed == m_edges.size() - 1)
        {
            const node& n_0 = m_tree.get_vertex(m_edges[0]);
            const node& op_node = m_tree.get_vertex(m_edges.back());
            if(op_node.check_type<node_transform_base>())
            {
                const node& n_1 = m_tree.get_vertex(m_edges[1]);
                const node_ident_any_tensor<NC,T>& n_0_concrete = dynamic_cast< const node_ident_any_tensor<NC,T>& >(n_0);
                const node_ident_any_tensor<NA,T>& n_1_concrete = dynamic_cast< const node_ident_any_tensor<NA,T>& >(n_1);
                gen_sparse_btensor<NC,T>& C = dynamic_cast< gen_sparse_btensor<NC,T>& >(n_0_concrete.get_tensor());
                gen_sparse_btensor<NA,T>& A = dynamic_cast< gen_sparse_btensor<NA,T>& >(n_1_concrete.get_tensor());
                m_ptrs[1] = (T*)A.get_data_ptr();

                const node_transform_base& n_tf = dynamic_cast< const node_transform_base& >(op_node);
                kern = new batch_kernel_permute<T>(C,A,n_tf.get_perm());
            }
        }
        else
        {
            size_t cur_node_idx = (NA != 0 ? 2 : (NC != 0 ? 1 : 0));
            const node& cur_tensor_node = m_tree.get_vertex(m_edges[cur_node_idx]);
            dispatch_1<1,4>::dispatch(*this,cur_tensor_node.get_n());
        }
    }

    kernel_builder(const expr_tree& tree,const expr_tree::edge_list_t& edges) : m_tree(tree),m_edges(edges),m_kern(NULL) 
    {
        m_ptrs.resize(m_edges.size() - 1);
    }

    template<size_t M>
    void dispatch()
    {
        if(NC == 0)
        {
            kernel_builder<T,M,0,0> kb(m_tree,m_edges);
            kb.build_kernel(*m_kern);
            m_ptrs = kb.get_ptrs();
        }
        else if(NA == 0)
        {
            kernel_builder<T,NC,M,0> kb(m_tree,m_edges);
            kb.build_kernel(*m_kern);
            m_ptrs = kb.get_ptrs();
        }
        else if(NB == 0)
        {
            kernel_builder<T,NC,NA,M> kb(m_tree,m_edges);
            kb.build_kernel(*m_kern);
            m_ptrs = kb.get_ptrs();
        }
    }
};





template<typename T>
batch_kernel<T>* dispatch_one(const node& n_one,const node& n_two,const node& op_node)
{
    switch(n_one.get_n())
    {
        case 3:
            const node_ident_any_tensor<3,T>& n_at = dynamic_cast< const node_ident_any_tensor<3,T>& >(n_one);
            return dispatch_two(dynamic_cast< const gen_sparse_btensor<3,T>& >(n_at.get_tensor()),n_two,op_node);
    }
}

template<size_t M,typename T>
batch_kernel<T>* dispatch_two(const gen_sparse_btensor<M,T>& input_one,const node& n_two,const node& op_node)
{
    switch(n_two.get_n())
    {
        case 3:
            const node_ident_any_tensor<3,T>& n_at = dynamic_cast< const node_ident_any_tensor<3,T>& >(n_two);
            return dispatch_op(input_one,dynamic_cast< const gen_sparse_btensor<3,T>& >(n_at.get_tensor()),op_node);
    }
}

template<size_t M,size_t N,typename T>
batch_kernel<T>* dispatch_op(const gen_sparse_btensor<M,T>& input_one,const gen_sparse_btensor<N,T>& input_two,const node& op_node)
{
    if(op_node.check_type<node_transform_base>())
    {
        const node_transform_base& n_tf = dynamic_cast< const node_transform_base& >(op_node);
        return new batch_kernel_permute<T>(input_one,input_two,n_tf.get_perm());
    }
    else
    {
        throw bad_parameter(g_ns,"dispatch_op","",__FILE__, __LINE__,
            "Unsupported op node type");
    }
}

template<typename T>
T* get_data_ptr_from_node(const node& n_one)
{
    switch(n_one.get_n())
    {
        case 3:
            const node_ident_any_tensor<3,T>& n_at = dynamic_cast< const node_ident_any_tensor<3,T>& >(n_one);
            return (T*)static_cast< gen_sparse_btensor<3,T>& >(n_at.get_tensor()).get_data_ptr();
    }
}

} // namespace expr

template<typename T>
class batch_provider_new
{
private:
    batch_kernel<T>* m_kern;
    std::vector<T*> m_ptrs;
public:
    static const char* k_clazz; //!< Class name
    batch_provider_new(const expr::expr_tree& tree); 
    void get_batch(T* output_ptr); 
};

template<typename T>
const char* batch_provider_new<T>::k_clazz = "batch_provider<T>";

template<typename T>
batch_provider_new<T>::batch_provider_new(const expr::expr_tree& tree)
{
    using namespace expr;
    expr_tree::node_id_t root_id = tree.get_root();
    const node& root = tree.get_vertex(root_id);

    if(!root.check_type<node_assign>())
    {
        throw bad_parameter(g_ns,k_clazz,"batch_provider(...)",__FILE__, __LINE__,
            "Invalid root node");
    }

    const expr_tree::edge_list_t& children = tree.get_edges_out(root_id);
    if(children.size() != 2)
    {
        throw bad_parameter(g_ns,k_clazz,"batch_provider(...)",__FILE__, __LINE__,
            "Root node does not have right number of children");
    }

    expr_tree::edge_list_t edges = expr_tree::edge_list_t(1,children[0]);
    expr_tree::edge_list_t input_edges = tree.get_edges_out(children[1]);
    edges.insert(edges.end(),input_edges.begin(),input_edges.end());
    edges.push_back(children[1]);
    kernel_builder<T> kb(tree,edges);
    kb.build_kernel(m_kern);
    m_ptrs = kb.get_ptrs();
}

template<typename T>
void batch_provider_new<T>::get_batch(T* output_ptr)
{ 
    m_ptrs[0] = output_ptr; 
    m_kern->generate_batch(m_ptrs,bispace_batch_map()); 
}


} // namespace libtensor

#endif /* BATCH_PROVIDER_NEW_H */
