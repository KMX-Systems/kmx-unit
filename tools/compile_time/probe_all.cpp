#include <kmx/unit/all.hpp>

using namespace kmx::literals;

namespace kmx::unit::compile_time_probe
{
    [[nodiscard]] constexpr auto make_sample() noexcept
    {
        const auto base = convert<distance::meter<>>(300.0_yd);
        const auto offset = distance::meter<double> {2.5};
        return pow<2>(base + offset);
    }

    static_assert(make_sample().as_native() > 0.0);
}

int main()
{
    return kmx::unit::compile_time_probe::make_sample().as_native() > 0.0 ? 0 : 1;
}
