%module modcomputation

%{
#include "Computation.h"
#include "ComputeFoo.h"
#include "ComputeCube.h"
%}

%include "Computation.h"
%include "ComputeFoo.h"
%include "ComputeCube.h"

%template(ComputeFoo) Computation<ComputeFooFunctor>;
%template(ComputeCube) Computation<ComputeCubeFunctor>;
