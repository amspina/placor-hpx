#include "cor/cor.hpp"

#include "external/CpuTimer.h"
#include "external/Rand.h"

#include <vector>
#include <numeric>

std::vector<long> accepted;
long nsamples = 1000000000;
Rand R(999);

std::shared_ptr<cor::Operon_Client> operon;

extern "C"
{
    void Main(int argc);
}

void McPi(void *)
{
    unsigned long ct;
    double x, y;
    ct = 0;
    for (auto n = 0; n < nsamples; ++n) {
        x = R.draw();
        y = R.draw();
        if ((x*x+y*y) <= 1.0)
            ++ct;
    }
    auto rank = operon->GetRank();
    accepted[rank-1] = ct;
}

void Main(int argc)
{
    auto domain = cor::GetDomain();
    CpuTimer T;
    double pi, result = 0;
    std::size_t const& pool_size = 16;


    nsamples /= pool_size;
    accepted.resize(pool_size, 0L);

    operon = std::move(domain->CreateLocal<cor::Operon_Client>(domain->Idp(),  "", pool_size));

    T.Start();
    operon->Dispatch(McPi, nullptr);

    result = std::accumulate(accepted.begin(), accepted.end(), result);
    //result = accepted[0];
    pi = 4.0 * (double) (result/(pool_size*nsamples));
    T.Stop();

    T.Report();
    std::cout << "Value of PI = " << pi << std::endl;

}