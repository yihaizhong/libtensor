#include "tod_set_cuda.h"
#include "kernels/cuda_kern_set.h"

namespace libtensor {

template<size_t N>
tod_set_cuda<N>::tod_set_cuda(const double v) :

    m_v(v) {

}


template<size_t N>
void tod_set_cuda<N>::perform(cpu_pool &cpus, dense_tensor_i<N, double> &t) {

    dense_tensor_ctrl<N, double> ctrl(t);
    double *d = ctrl.req_dataptr();
    double *p = d;

    {
         auto_cpu_lock cpu(cpus);
         size_t sz = t.get_dims().get_size();
         size_t grid, threads;
         threads = 65535;
         size_t max_threads_per_kernel = 1024 * threads;
         for(size_t i = 0; i < sz; i+= max_threads_per_kernel) {
        	 size_t remaining = 0;
        	//Do max possible number of blocks if possible
         	if (i + max_threads_per_kernel <= sz) {
         		grid = max_threads_per_kernel/threads;
         		cuda::kern_set<<<grid,threads>>>(p, m_v);
         	//if not do remaining number
         	} else {
         		grid = (sz - i)/threads;
         		cuda::kern_set<<<grid,threads>>>(p, m_v);
         		remaining = (sz - i)%threads;
         		if (remaining) {
         			cuda::kern_set<<<1, remaining>>>(p + grid*threads, m_v);
         		}
         	}
         	p += max_threads_per_kernel;
         }
     }

    ctrl.ret_dataptr(d);
}

template class tod_set_cuda<1>;
template class tod_set_cuda<2>;
template class tod_set_cuda<3>;
template class tod_set_cuda<4>;
template class tod_set_cuda<5>;
template class tod_set_cuda<6>;



} // namespace libtensor

