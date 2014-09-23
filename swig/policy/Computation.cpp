#include <Computation.h>

template <class Compute>
Computation<Compute>::Computation() {}

template <class Compute>
Computation<Compute>::~Computation() {}

template class Computation<ComputeFooFunctor>;
template class Computation<ComputeCubeFunctor>;
