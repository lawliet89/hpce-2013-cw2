#include "fourier_transform.hpp"
#include "tbb/parallel_for.h"
#include "tbb/task_group.h"

#include <cmath>
#include <cassert>

#define SPLIT_THRESHOLD 1024
#define K_SIZE 1024

namespace hpce
{
namespace ywc110
{
class fast_fourier_transform_opt
	: public fourier_transform
{
protected:
	/* Standard radix-2 FFT only supports binary power lengths */
	virtual size_t calc_padded_size(size_t n) const
	{
		assert(n!=0);

		size_t ret=1;
		while(ret<n){
			ret<<=1;
		}

		return ret;
	}

	virtual void forwards_impl(
		size_t n,	const std::complex<double> &wn,
		const std::complex<double> *pIn, size_t sIn,
		std::complex<double> *pOut, size_t sOut
	) const
	{
		assert(n>0);

		if (n == 1){
			pOut[0] = pIn[0];
		}else if (n == 2){
			pOut[0] = pIn[0]+pIn[sIn];
			pOut[sOut] = pIn[0]-pIn[sIn];
		}else{
			size_t m = n/2;

			auto split1 = [=](){ forwards_impl(m,wn*wn,pIn,2*sIn,pOut,sOut); };
			auto split2 = [=](){
				forwards_impl(m,wn*wn,pIn+sIn,2*sIn,pOut+sOut*m,sOut);
			};

			if (m >= SPLIT_THRESHOLD) {
				tbb::task_group group;
				group.run(split1);
				group.run(split2);
				group.wait();
			} else {
				split1();
				split2();
			}


			size_t K = std::min((size_t) K_SIZE, m);
			size_t j0End = m/K;

			if (m % K) {
				++j0End;
			}

			tbb::parallel_for(size_t(0), j0End, [=](size_t j0) {
				size_t j = j0 * K;
				std::complex<double> w = std::pow(wn, j);

				for (size_t j1 = 0; j1 < K && j < m; ++j1, ++j) {
					std::complex<double> t1 = w*pOut[m+j];
					std::complex<double> t2 = pOut[j]-t1;
					pOut[j] = pOut[j]+t1;        /*  pOut[j] = pOut[j] + w^i pOut[m+j] */
					pOut[j+m] = t2;              /*  pOut[j] = pOut[j] - w^i pOut[m+j] */
			  		w = w*wn;
				}
			});
		}
	}

	virtual void backwards_impl(
		size_t n,	const std::complex<double> &wn,
		const std::complex<double> *pIn, size_t sIn,
		std::complex<double> *pOut, size_t sOut
	) const
	{
		complex_t reverse_wn=1.0/wn;
		forwards_impl(n, reverse_wn, pIn, sIn, pOut, sOut);

		double scale=1.0/n;
		for(size_t i=0;i<n;i++){
			pOut[i]=pOut[i]*scale;
		}
	}

public:
	virtual std::string name() const
	{ return "hpce.ywc110.fast_fourier_transform_opt"; }

	virtual bool is_quadratic() const
	{ return false; }
};

std::shared_ptr<fourier_transform> Create_fast_fourier_transform_opt()
{
	return std::make_shared<fast_fourier_transform_opt>();
}

}; // namespace ywc110
}; // namespace hpce
